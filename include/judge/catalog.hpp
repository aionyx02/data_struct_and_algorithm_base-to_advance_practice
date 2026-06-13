#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "judge/problem.hpp"

namespace judge {

class ProblemCatalog {
public:
    explicit ProblemCatalog(std::filesystem::path projectRoot);

    [[nodiscard]] const std::filesystem::path& projectRoot() const noexcept;
    [[nodiscard]] std::vector<Problem> list() const;
    [[nodiscard]] Problem findById(const std::string& id) const;

private:
    std::filesystem::path projectRoot_;
    std::filesystem::path problemsRoot_;
};

}  // namespace judge
