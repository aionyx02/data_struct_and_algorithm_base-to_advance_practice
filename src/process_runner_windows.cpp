#include "judge/process_runner.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace judge {
namespace {

class UniqueHandle {
public:
    UniqueHandle() = default;
    explicit UniqueHandle(HANDLE handle) : handle_(handle) {}

    ~UniqueHandle() {
        reset();
    }

    UniqueHandle(const UniqueHandle&) = delete;
    UniqueHandle& operator=(const UniqueHandle&) = delete;

    UniqueHandle(UniqueHandle&& other) noexcept
        : handle_(std::exchange(other.handle_, nullptr)) {}

    UniqueHandle& operator=(UniqueHandle&& other) noexcept {
        if (this != &other) {
            reset(std::exchange(other.handle_, nullptr));
        }
        return *this;
    }

    [[nodiscard]] HANDLE get() const noexcept {
        return handle_;
    }

    [[nodiscard]] bool valid() const noexcept {
        return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE;
    }

    HANDLE release() noexcept {
        return std::exchange(handle_, nullptr);
    }

    void reset(HANDLE handle = nullptr) noexcept {
        if (valid()) {
            CloseHandle(handle_);
        }
        handle_ = handle;
    }

private:
    HANDLE handle_ = nullptr;
};

std::wstring utf8ToWide(std::string_view value) {
    if (value.empty()) {
        return {};
    }
    const int required = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        value.data(),
        static_cast<int>(value.size()),
        nullptr,
        0
    );
    if (required <= 0) {
        throw std::runtime_error("invalid UTF-8 process argument");
    }
    std::wstring result(static_cast<std::size_t>(required), L'\0');
    if (MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            value.data(),
            static_cast<int>(value.size()),
            result.data(),
            required
        ) != required) {
        throw std::runtime_error("failed to convert process argument to UTF-16");
    }
    return result;
}

std::string wideToUtf8(std::wstring_view value) {
    if (value.empty()) {
        return {};
    }
    const int required = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );
    if (required <= 0) {
        return "unknown Windows error";
    }
    std::string result(static_cast<std::size_t>(required), '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        result.data(),
        required,
        nullptr,
        nullptr
    );
    return result;
}

std::string windowsErrorMessage(DWORD errorCode) {
    wchar_t* buffer = nullptr;
    const DWORD length = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        0,
        reinterpret_cast<wchar_t*>(&buffer),
        0,
        nullptr
    );
    if (length == 0 || buffer == nullptr) {
        return "Windows error " + std::to_string(errorCode);
    }
    std::wstring message(buffer, length);
    LocalFree(buffer);
    while (!message.empty() &&
           (message.back() == L'\r' || message.back() == L'\n' ||
            message.back() == L' ')) {
        message.pop_back();
    }
    return wideToUtf8(message);
}

std::wstring quoteArgument(const std::wstring& argument) {
    if (argument.empty()) {
        return L"\"\"";
    }
    const bool needsQuotes =
        argument.find_first_of(L" \t\n\v\"") != std::wstring::npos;
    if (!needsQuotes) {
        return argument;
    }

    std::wstring quoted = L"\"";
    std::size_t backslashes = 0;
    for (const wchar_t character : argument) {
        if (character == L'\\') {
            ++backslashes;
            continue;
        }
        if (character == L'"') {
            quoted.append(backslashes * 2 + 1, L'\\');
            quoted.push_back(L'"');
            backslashes = 0;
            continue;
        }
        quoted.append(backslashes, L'\\');
        backslashes = 0;
        quoted.push_back(character);
    }
    quoted.append(backslashes * 2, L'\\');
    quoted.push_back(L'"');
    return quoted;
}

std::wstring buildCommandLine(const ProcessRequest& request) {
    std::wstring commandLine = quoteArgument(utf8ToWide(request.executable));
    for (const std::string& argument : request.arguments) {
        commandLine.push_back(L' ');
        commandLine += quoteArgument(utf8ToWide(argument));
    }
    return commandLine;
}

bool createPipePair(UniqueHandle& readHandle, UniqueHandle& writeHandle) {
    SECURITY_ATTRIBUTES attributes{
        .nLength = sizeof(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = nullptr,
        .bInheritHandle = TRUE,
    };
    HANDLE rawRead = nullptr;
    HANDLE rawWrite = nullptr;
    if (!CreatePipe(&rawRead, &rawWrite, &attributes, 0)) {
        return false;
    }
    readHandle.reset(rawRead);
    writeHandle.reset(rawWrite);
    return true;
}

bool writeStandardInput(HANDLE handle, std::string_view input) {
    std::size_t offset = 0;
    while (offset < input.size()) {
        const DWORD chunk = static_cast<DWORD>(
            std::min<std::size_t>(input.size() - offset, 16 * 1024)
        );
        DWORD written = 0;
        if (!WriteFile(handle, input.data() + offset, chunk, &written, nullptr)) {
            return GetLastError() == ERROR_BROKEN_PIPE;
        }
        offset += written;
    }
    return true;
}

bool drainPipe(
    HANDLE handle,
    std::string& destination,
    std::size_t& totalBytes,
    std::size_t outputLimit
) {
    std::array<char, 4096> buffer{};
    while (true) {
        DWORD available = 0;
        if (!PeekNamedPipe(handle, nullptr, 0, nullptr, &available, nullptr)) {
            const DWORD error = GetLastError();
            return error == ERROR_BROKEN_PIPE || error == ERROR_NO_DATA;
        }
        if (available == 0) {
            return true;
        }

        DWORD bytesRead = 0;
        const DWORD requested =
            std::min<DWORD>(available, static_cast<DWORD>(buffer.size()));
        if (!ReadFile(handle, buffer.data(), requested, &bytesRead, nullptr)) {
            const DWORD error = GetLastError();
            return error == ERROR_BROKEN_PIPE || error == ERROR_NO_DATA;
        }

        const std::size_t remaining =
            totalBytes < outputLimit ? outputLimit - totalBytes : 0;
        const std::size_t appendCount =
            std::min<std::size_t>(remaining, bytesRead);
        destination.append(buffer.data(), appendCount);
        totalBytes += bytesRead;
        if (totalBytes > outputLimit) {
            return false;
        }
    }
}

void terminateJob(HANDLE job, HANDLE process) {
    if (job != nullptr) {
        TerminateJobObject(job, 1);
    } else if (process != nullptr) {
        TerminateProcess(process, 1);
    }
}

}  // namespace

ProcessResult ProcessRunner::run(const ProcessRequest& request) const {
    ProcessResult result;
    if (request.executable.empty()) {
        result.launchError = "process executable is empty";
        return result;
    }
    if (!std::filesystem::is_directory(request.workingDirectory)) {
        result.launchError =
            "working directory does not exist: " +
            request.workingDirectory.string();
        return result;
    }

    UniqueHandle standardInputRead;
    UniqueHandle standardInputWrite;
    UniqueHandle standardOutputRead;
    UniqueHandle standardOutputWrite;
    UniqueHandle standardErrorRead;
    UniqueHandle standardErrorWrite;
    if (!createPipePair(standardInputRead, standardInputWrite) ||
        !createPipePair(standardOutputRead, standardOutputWrite) ||
        !createPipePair(standardErrorRead, standardErrorWrite)) {
        result.launchError =
            "failed to create process pipes: " +
            windowsErrorMessage(GetLastError());
        return result;
    }

    SetHandleInformation(
        standardInputWrite.get(),
        HANDLE_FLAG_INHERIT,
        0
    );
    SetHandleInformation(
        standardOutputRead.get(),
        HANDLE_FLAG_INHERIT,
        0
    );
    SetHandleInformation(
        standardErrorRead.get(),
        HANDLE_FLAG_INHERIT,
        0
    );

    STARTUPINFOW startupInfo{};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = standardInputRead.get();
    startupInfo.hStdOutput = standardOutputWrite.get();
    startupInfo.hStdError = standardErrorWrite.get();

    PROCESS_INFORMATION processInfo{};
    std::wstring commandLine;
    try {
        commandLine = buildCommandLine(request);
    } catch (const std::exception& error) {
        result.launchError = error.what();
        return result;
    }
    std::vector<wchar_t> mutableCommandLine(
        commandLine.begin(),
        commandLine.end()
    );
    mutableCommandLine.push_back(L'\0');
    const std::wstring workingDirectory = request.workingDirectory.wstring();

    const auto start = std::chrono::steady_clock::now();
    if (!CreateProcessW(
            nullptr,
            mutableCommandLine.data(),
            nullptr,
            nullptr,
            TRUE,
            CREATE_NO_WINDOW | CREATE_SUSPENDED,
            nullptr,
            workingDirectory.c_str(),
            &startupInfo,
            &processInfo
        )) {
        result.launchError = windowsErrorMessage(GetLastError());
        return result;
    }
    result.launched = true;

    UniqueHandle process(processInfo.hProcess);
    UniqueHandle thread(processInfo.hThread);
    standardInputRead.reset();
    standardOutputWrite.reset();
    standardErrorWrite.reset();

    UniqueHandle job(CreateJobObjectW(nullptr, nullptr));
    if (job.valid()) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
        limits.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (!SetInformationJobObject(
                job.get(),
                JobObjectExtendedLimitInformation,
                &limits,
                sizeof(limits)
            ) ||
            !AssignProcessToJobObject(job.get(), process.get())) {
            job.reset();
        }
    }

    if (ResumeThread(thread.get()) == static_cast<DWORD>(-1)) {
        result.launchError =
            "failed to start process: " + windowsErrorMessage(GetLastError());
        terminateJob(job.get(), process.get());
        result.launched = false;
        return result;
    }

    if (!writeStandardInput(standardInputWrite.get(), request.standardInput)) {
        terminateJob(job.get(), process.get());
    }
    standardInputWrite.reset();

    std::size_t totalOutputBytes = 0;
    bool processFinished = false;
    while (!processFinished) {
        const bool stdoutWithinLimit = drainPipe(
            standardOutputRead.get(),
            result.standardOutput,
            totalOutputBytes,
            request.outputLimitBytes
        );
        const bool stderrWithinLimit = drainPipe(
            standardErrorRead.get(),
            result.standardError,
            totalOutputBytes,
            request.outputLimitBytes
        );
        if (!stdoutWithinLimit || !stderrWithinLimit) {
            result.outputLimitExceeded = true;
            terminateJob(job.get(), process.get());
        }

        const DWORD waitResult = WaitForSingleObject(process.get(), 5);
        processFinished = waitResult == WAIT_OBJECT_0;
        if (waitResult == WAIT_FAILED) {
            result.launchError =
                "failed while waiting for process: " +
                windowsErrorMessage(GetLastError());
            terminateJob(job.get(), process.get());
            processFinished = true;
        }

        const auto now = std::chrono::steady_clock::now();
        if (!processFinished && now - start >= request.timeout) {
            result.timedOut = true;
            terminateJob(job.get(), process.get());
            WaitForSingleObject(process.get(), 1000);
            processFinished = true;
        }
    }

    drainPipe(
        standardOutputRead.get(),
        result.standardOutput,
        totalOutputBytes,
        request.outputLimitBytes
    );
    drainPipe(
        standardErrorRead.get(),
        result.standardError,
        totalOutputBytes,
        request.outputLimitBytes
    );

    DWORD exitCode = 1;
    if (GetExitCodeProcess(process.get(), &exitCode)) {
        result.exitCode = static_cast<int>(exitCode);
    }
    result.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start
    );
    return result;
}

}  // namespace judge
