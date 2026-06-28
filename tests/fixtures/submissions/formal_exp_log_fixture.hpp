#include <iostream>
#include <string>
#include <vector>

#ifndef FPS_EXP_MISSING_INDEX_FACTOR
#define FPS_EXP_MISSING_INDEX_FACTOR 0
#endif
#ifndef FPS_LOG_WRONG_SIGN
#define FPS_LOG_WRONG_SIGN 0
#endif

using fps64 = long long;
constexpr fps64 FPS_MOD = 1'000'000'007;

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; int degree; std::vector<fps64> series; };
    std::vector<Query> queries(queryCount);
    int maximum = 0;
    for (Query& query : queries) {
        std::cin >> query.operation >> query.degree;
        query.series.resize(query.degree + 1);
        for (fps64& value : query.series) std::cin >> value;
        if (query.degree > maximum) maximum = query.degree;
    }
    std::vector<fps64> inverse(maximum + 1, 0);
    if (maximum >= 1) inverse[1] = 1;
    for (int i = 2; i <= maximum; ++i) {
        inverse[i] = FPS_MOD - (FPS_MOD / i) * inverse[FPS_MOD % i] % FPS_MOD;
    }

    for (const Query& query : queries) {
        std::vector<fps64> answer(query.degree + 1, 0);
        if (query.operation == "exp") {
            answer[0] = 1;
            for (int degree = 1; degree <= query.degree; ++degree) {
                fps64 sum = 0;
                for (int i = 1; i <= degree; ++i) {
                    fps64 factor = FPS_EXP_MISSING_INDEX_FACTOR ? 1 : i;
                    sum = (sum + factor * query.series[i] % FPS_MOD
                        * answer[degree - i]) % FPS_MOD;
                }
                answer[degree] = sum * inverse[degree] % FPS_MOD;
            }
        } else {
            for (int degree = 1; degree <= query.degree; ++degree) {
                fps64 convolution = 0;
                for (int i = 1; i < degree; ++i) {
                    convolution = (convolution + static_cast<fps64>(i)
                        * answer[i] % FPS_MOD * query.series[degree - i]) % FPS_MOD;
                }
                fps64 correction = convolution * inverse[degree] % FPS_MOD;
                answer[degree] = FPS_LOG_WRONG_SIGN
                    ? (query.series[degree] + correction) % FPS_MOD
                    : (query.series[degree] - correction + FPS_MOD) % FPS_MOD;
            }
        }
        for (int i = 0; i <= query.degree; ++i) {
            if (i) std::cout << ' ';
            std::cout << answer[i];
        }
        std::cout << '\n';
    }
    return 0;
}
