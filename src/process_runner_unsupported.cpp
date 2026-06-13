#include "judge/process_runner.hpp"

namespace judge {

ProcessResult ProcessRunner::run(const ProcessRequest&) const {
    return ProcessResult{
        .launched = false,
        .launchError =
            "the process runner is not implemented for this operating system",
    };
}

}  // namespace judge
