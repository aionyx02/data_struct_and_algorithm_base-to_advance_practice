#include "judge/case_generator.hpp"

#include <stdexcept>

#include "generators/generator_registry.hpp"

namespace judge {

const CaseGeneratorRegistry& caseGenerators() {
    static const CaseGeneratorRegistry registry = [] {
        CaseGeneratorRegistry assembled;
        generators::registerLinearGenerators(assembled);
        generators::registerTreeGenerators(assembled);
        generators::registerBstGenerators(assembled);
        generators::registerForestDsuGenerators(assembled);
        generators::registerGraphGenerators(assembled);
        generators::registerHashingGenerators(assembled);
        generators::registerAdvancedGenerators(assembled);
        generators::registerSegmentTreeGenerators(assembled);
        generators::registerStaticRangeGenerators(assembled);
        generators::registerUnionFindGenerators(assembled);
        return assembled;
    }();
    return registry;
}

GeneratedCase generateCase(
    const std::string& problemId,
    std::uint64_t seed,
    int operationCount
) {
    std::mt19937_64 random(seed);
    const CaseGeneratorRegistry& registry = caseGenerators();
    const auto found = registry.find(problemId);
    if (found == registry.end()) {
        throw std::runtime_error(
            "stress generator is not available for problem: " + problemId
        );
    }
    return found->second(random, operationCount);
}

}  // namespace judge
