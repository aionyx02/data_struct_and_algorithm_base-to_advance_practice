#pragma once

#include "judge/case_generator.hpp"

namespace judge::generators {

// Each domain translation unit registers its problem generators into the shared
// registry. Add a new registrar here when introducing a new domain file.
void registerLinearGenerators(CaseGeneratorRegistry& registry);
void registerTreeGenerators(CaseGeneratorRegistry& registry);
void registerBstGenerators(CaseGeneratorRegistry& registry);
void registerForestDsuGenerators(CaseGeneratorRegistry& registry);
void registerGraphGenerators(CaseGeneratorRegistry& registry);
void registerHashingGenerators(CaseGeneratorRegistry& registry);
void registerAdvancedGenerators(CaseGeneratorRegistry& registry);
void registerSegmentTreeGenerators(CaseGeneratorRegistry& registry);
void registerStaticRangeGenerators(CaseGeneratorRegistry& registry);
void registerUnionFindGenerators(CaseGeneratorRegistry& registry);
void registerBalancedTreeGenerators(CaseGeneratorRegistry& registry);
void registerTreeQueryGenerators(CaseGeneratorRegistry& registry);
void registerStringIndexGenerators(CaseGeneratorRegistry& registry);

}  // namespace judge::generators
