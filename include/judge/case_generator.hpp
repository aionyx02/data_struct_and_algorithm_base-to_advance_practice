#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <random>
#include <string>

namespace judge {

// A single generated stress case: the program input and the expected output
// produced by the trusted brute-force model.
struct GeneratedCase {
    std::string input;
    std::string expectedOutput;
};

// A case generator turns a seeded RNG and an operation budget into one case.
using CaseGenerator =
    std::function<GeneratedCase(std::mt19937_64&, int)>;

// Problem ID to generator lookup, assembled once from every domain registry.
using CaseGeneratorRegistry = std::map<std::string, CaseGenerator>;

// Returns the shared registry of all problem case generators.
const CaseGeneratorRegistry& caseGenerators();

// Builds an RNG from the seed, dispatches to the registered generator, and
// throws std::runtime_error when no generator exists for the problem.
GeneratedCase generateCase(
    const std::string& problemId,
    std::uint64_t seed,
    int operationCount
);

}  // namespace judge
