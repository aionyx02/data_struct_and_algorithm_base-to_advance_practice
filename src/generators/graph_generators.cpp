#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace judge::generators {
namespace {
struct StressGraph {
    int vertexCount;
    int capacity;
    bool directed;
    std::vector<unsigned char> adjacency;
    int edgeCount = 0;

    StressGraph(int vertices, int edgeCapacity, bool isDirected)
        : vertexCount(vertices),
          capacity(edgeCapacity),
          directed(isDirected),
          adjacency(
              static_cast<std::size_t>(vertices * vertices),
              static_cast<unsigned char>(0)
          ) {}

    bool valid(int vertex) const {
        return vertex >= 0 && vertex < vertexCount;
    }

    bool has(int from, int to) const {
        return adjacency[
            static_cast<std::size_t>(from * vertexCount + to)
        ] != 0;
    }

    void add(int from, int to) {
        adjacency[
            static_cast<std::size_t>(from * vertexCount + to)
        ] = 1;
        if (!directed) {
            adjacency[
                static_cast<std::size_t>(to * vertexCount + from)
            ] = 1;
        }
        ++edgeCount;
    }

    void remove(int from, int to) {
        adjacency[
            static_cast<std::size_t>(from * vertexCount + to)
        ] = 0;
        if (!directed) {
            adjacency[
                static_cast<std::size_t>(to * vertexCount + from)
            ] = 0;
        }
        --edgeCount;
    }

    void clear() {
        std::fill(adjacency.begin(), adjacency.end(), 0);
        edgeCount = 0;
    }

    int degree(int vertex, bool incoming = false) const {
        int result = 0;
        for (int neighbor = 0; neighbor < vertexCount; ++neighbor) {
            if (incoming ? has(neighbor, vertex) : has(vertex, neighbor)) {
                ++result;
            }
        }
        return result;
    }
};

void appendGraphNeighbors(
    std::ostringstream& output,
    const StressGraph& graph,
    int vertex,
    bool incoming = false
) {
    std::vector<int> neighbors;
    for (int candidate = 0; candidate < graph.vertexCount; ++candidate) {
        if (incoming
                ? graph.has(candidate, vertex)
                : graph.has(vertex, candidate)) {
            neighbors.push_back(candidate);
        }
    }
    appendIntList(output, neighbors);
}

void appendGraphRow(
    std::ostringstream& output,
    const StressGraph& graph,
    int vertex
) {
    for (int candidate = 0; candidate < graph.vertexCount; ++candidate) {
        if (candidate != 0) output << ' ';
        output << (graph.has(vertex, candidate) ? 1 : 0);
    }
    output << '\n';
}

void applyGraphPairOperation(
    std::ostringstream& output,
    StressGraph& graph,
    const std::string& command,
    int from,
    int to
) {
    if (!graph.valid(from) || !graph.valid(to)) {
        appendLine(output, "OUT_OF_RANGE");
    } else if ((command == "add" || command == "remove") && from == to) {
        appendLine(output, "SELF_LOOP");
    } else if (command == "has") {
        appendLine(output, graph.has(from, to) ? "true" : "false");
    } else if (command == "add") {
        if (graph.has(from, to)) {
            appendLine(output, "DUPLICATE");
        } else if (graph.capacity >= 0 &&
                   graph.edgeCount == graph.capacity) {
            appendLine(output, "FULL");
        } else {
            graph.add(from, to);
        }
    } else if (!graph.has(from, to)) {
        appendLine(output, "NOT_FOUND");
    } else {
        graph.remove(from, to);
        appendLine(output, "REMOVED");
    }
}

GeneratedCase generateUndirectedAdjacencyMatrix(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F47-undirected-adjacency-matrix operation_limit is too small"
        );
    }
    StressGraph graph(6, -1, false);
    std::ostringstream input;
    std::ostringstream output;
    input << "6 " << operationCount
          << "\nadd 0 2\nadd 1 2\nhas 2 0\nneighbors 2\nrow 2\n"
          << "degree 2\nadd 2 0\nremove 0 2\n";
    graph.add(0, 2);
    graph.add(1, 2);
    output << "true\n0 1\n1 1 0 0 0 0\n2\nDUPLICATE\n";
    graph.remove(0, 2);
    output << "REMOVED\n";

    for (int operation = 8; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 7);
        if (kind <= 2) {
            const int from = randomInt(random, -1, graph.vertexCount);
            const int to = randomInt(random, -1, graph.vertexCount);
            const char* command =
                kind == 0 ? "add" : (kind == 1 ? "remove" : "has");
            input << command << ' ' << from << ' ' << to << '\n';
            applyGraphPairOperation(output, graph, command, from, to);
        } else if (kind <= 5) {
            const int vertex = randomInt(random, -1, graph.vertexCount);
            const char* command =
                kind == 3 ? "degree" : (kind == 4 ? "neighbors" : "row");
            input << command << ' ' << vertex << '\n';
            if (!graph.valid(vertex)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (kind == 3) {
                appendLine(output, graph.degree(vertex));
            } else if (kind == 4) {
                appendGraphNeighbors(output, graph, vertex);
            } else {
                appendGraphRow(output, graph, vertex);
            }
        } else if (kind == 6) {
            input << "edge_count\n";
            appendLine(output, graph.edgeCount);
        } else {
            input << "clear\n";
            graph.clear();
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateDirectedAdjacencyMatrix(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F48-directed-adjacency-matrix operation_limit is too small"
        );
    }
    StressGraph graph(6, -1, true);
    std::ostringstream input;
    std::ostringstream output;
    input << "6 " << operationCount
          << "\nadd 0 1\nadd 0 1\nadd 2 1\nout_degree 0\n"
          << "in_degree 1\nout_neighbors 0\nin_neighbors 1\nedge_count\n";
    graph.add(0, 1);
    output << "DUPLICATE\n";
    graph.add(2, 1);
    output << "1\n2\n1\n0 2\n2\n";

    for (int operation = 8; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 9);
        if (kind <= 2) {
            const int from = randomInt(random, -1, graph.vertexCount);
            const int to = randomInt(random, -1, graph.vertexCount);
            const char* command =
                kind == 0 ? "add" : (kind == 1 ? "remove" : "has");
            input << command << ' ' << from << ' ' << to << '\n';
            applyGraphPairOperation(output, graph, command, from, to);
        } else if (kind <= 7) {
            const int vertex = randomInt(random, -1, graph.vertexCount);
            const char* command = kind == 3
                ? "out_degree"
                : (kind == 4
                    ? "in_degree"
                    : (kind == 5
                        ? "out_neighbors"
                        : (kind == 6 ? "in_neighbors" : "row")));
            input << command << ' ' << vertex << '\n';
            if (!graph.valid(vertex)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (kind == 3) {
                appendLine(output, graph.degree(vertex));
            } else if (kind == 4) {
                appendLine(output, graph.degree(vertex, true));
            } else if (kind == 5) {
                appendGraphNeighbors(output, graph, vertex);
            } else if (kind == 6) {
                appendGraphNeighbors(output, graph, vertex, true);
            } else {
                appendGraphRow(output, graph, vertex);
            }
        } else if (kind == 8) {
            input << "edge_count\n";
            appendLine(output, graph.edgeCount);
        } else {
            input << "clear\n";
            graph.clear();
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateUndirectedAdjacencyList(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F49-undirected-adjacency-list operation_limit is too small"
        );
    }
    StressGraph graph(6, 8, false);
    std::ostringstream input;
    std::ostringstream output;
    input << "6 8 " << operationCount
          << "\nadd 3 5\nadd 3 1\nadd 3 4\nadd 3 0\nadd 3 2\n"
          << "neighbors 3\ndegree 3\nedge_count\n";
    graph.add(3, 5);
    graph.add(3, 1);
    graph.add(3, 4);
    graph.add(3, 0);
    graph.add(3, 2);
    output << "0 1 2 4 5\n5\n5\n";

    for (int operation = 8; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 7);
        if (kind <= 2) {
            const int from = randomInt(random, -1, graph.vertexCount);
            const int to = randomInt(random, -1, graph.vertexCount);
            const char* command =
                kind == 0 ? "add" : (kind == 1 ? "remove" : "has");
            input << command << ' ' << from << ' ' << to << '\n';
            applyGraphPairOperation(output, graph, command, from, to);
        } else if (kind <= 4) {
            const int vertex = randomInt(random, -1, graph.vertexCount);
            input << (kind == 3 ? "degree " : "neighbors ") << vertex << '\n';
            if (!graph.valid(vertex)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (kind == 3) {
                appendLine(output, graph.degree(vertex));
            } else {
                appendGraphNeighbors(output, graph, vertex);
            }
        } else if (kind == 5) {
            input << "edge_count\n";
            appendLine(output, graph.edgeCount);
        } else if (kind == 6) {
            input << "capacity\n";
            appendLine(output, graph.capacity);
        } else {
            input << "clear\n";
            graph.clear();
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateDirectedAdjacencyList(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F50-directed-adjacency-list operation_limit is too small"
        );
    }
    StressGraph graph(6, 8, true);
    std::ostringstream input;
    std::ostringstream output;
    input << "6 8 " << operationCount
          << "\nadd 0 2\nadd 1 2\nremove 1 2\nin_degree 2\n"
          << "in_neighbors 2\nout_degree 1\nadd 1 2\nedge_count\n";
    graph.add(0, 2);
    graph.add(1, 2);
    graph.remove(1, 2);
    output << "REMOVED\n1\n0\n0\n";
    graph.add(1, 2);
    output << "2\n";

    for (int operation = 8; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 9);
        if (kind <= 2) {
            const int from = randomInt(random, -1, graph.vertexCount);
            const int to = randomInt(random, -1, graph.vertexCount);
            const char* command =
                kind == 0 ? "add" : (kind == 1 ? "remove" : "has");
            input << command << ' ' << from << ' ' << to << '\n';
            applyGraphPairOperation(output, graph, command, from, to);
        } else if (kind <= 6) {
            const int vertex = randomInt(random, -1, graph.vertexCount);
            const char* command = kind == 3
                ? "out_degree"
                : (kind == 4
                    ? "in_degree"
                    : (kind == 5 ? "out_neighbors" : "in_neighbors"));
            input << command << ' ' << vertex << '\n';
            if (!graph.valid(vertex)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (kind == 3) {
                appendLine(output, graph.degree(vertex));
            } else if (kind == 4) {
                appendLine(output, graph.degree(vertex, true));
            } else if (kind == 5) {
                appendGraphNeighbors(output, graph, vertex);
            } else {
                appendGraphNeighbors(output, graph, vertex, true);
            }
        } else if (kind == 7) {
            input << "edge_count\n";
            appendLine(output, graph.edgeCount);
        } else if (kind == 8) {
            input << "capacity\n";
            appendLine(output, graph.capacity);
        } else {
            input << "clear\n";
            graph.clear();
        }
    }
    return {input.str(), output.str()};
}

struct StressEdgeRecord {
    int id = 0;
    int left = 0;
    int right = 0;
    bool active = false;
};

int findStressEdgeById(
    const std::vector<StressEdgeRecord>& edges,
    int id
) {
    for (int slot = 0; slot < static_cast<int>(edges.size()); ++slot) {
        if (edges[static_cast<std::size_t>(slot)].active &&
            edges[static_cast<std::size_t>(slot)].id == id) {
            return slot;
        }
    }
    return -1;
}

bool stressEdgeHasPair(
    const StressEdgeRecord& edge,
    int left,
    int right
) {
    return edge.active &&
        ((edge.left == left && edge.right == right) ||
         (edge.left == right && edge.right == left));
}

GeneratedCase generateFixedEdgeTableGraph(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 12) {
        throw std::runtime_error(
            "F51-fixed-edge-table-graph operation_limit is too small"
        );
    }
    const int vertexCount = 6;
    const int capacity = 3;
    std::vector<StressEdgeRecord> edges(
        static_cast<std::size_t>(capacity)
    );
    int edgeCount = 0;
    auto addEdge = [&](int id, int left, int right) {
        int slot = 0;
        while (edges[static_cast<std::size_t>(slot)].active) ++slot;
        edges[static_cast<std::size_t>(slot)] = {id, left, right, true};
        ++edgeCount;
        return slot;
    };
    std::ostringstream input;
    std::ostringstream output;
    input << "6 3 " << operationCount
          << "\nadd 10 0 1\nadd 20 1 2\nadd 30 2 3\nerase 20\n"
          << "edge_at 1\nadd 40 4 5\nslot 40\nedge_at 1\n"
          << "degree 4\nneighbors 5\nfree_slots\nedge_count\n";
    appendLine(output, addEdge(10, 0, 1));
    appendLine(output, addEdge(20, 1, 2));
    appendLine(output, addEdge(30, 2, 3));
    edges[1].active = false;
    --edgeCount;
    output << "1\nEMPTY\n";
    appendLine(output, addEdge(40, 4, 5));
    output << "1\n40 4 5\n1\n4\n0\n3\n";

    for (int operation = 12; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 9);
        if (kind == 0) {
            const int id = randomInt(random, 0, 8);
            const int left = randomInt(random, -1, vertexCount);
            const int right = randomInt(random, -1, vertexCount);
            input << "add " << id << ' ' << left << ' ' << right << '\n';
            if (left < 0 || left >= vertexCount ||
                right < 0 || right >= vertexCount) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (left == right) {
                appendLine(output, "SELF_LOOP");
            } else {
                bool duplicate = findStressEdgeById(edges, id) >= 0;
                for (const StressEdgeRecord& edge : edges) {
                    duplicate =
                        duplicate || stressEdgeHasPair(edge, left, right);
                }
                if (duplicate) {
                    appendLine(output, "DUPLICATE");
                } else if (edgeCount == capacity) {
                    appendLine(output, "FULL");
                } else {
                    appendLine(output, addEdge(id, left, right));
                }
            }
        } else if (kind == 1) {
            const int id = randomInt(random, 0, 8);
            input << "erase " << id << '\n';
            const int slot = findStressEdgeById(edges, id);
            if (slot < 0) {
                appendLine(output, "NOT_FOUND");
            } else {
                edges[static_cast<std::size_t>(slot)].active = false;
                --edgeCount;
                appendLine(output, slot);
            }
        } else if (kind == 2 || kind == 3) {
            const int id = randomInt(random, 0, 8);
            input << (kind == 2 ? "slot " : "endpoints ") << id << '\n';
            const int slot = findStressEdgeById(edges, id);
            if (slot < 0) {
                appendLine(output, "NOT_FOUND");
            } else if (kind == 2) {
                appendLine(output, slot);
            } else {
                const StressEdgeRecord& edge =
                    edges[static_cast<std::size_t>(slot)];
                output << edge.left << ' ' << edge.right << '\n';
            }
        } else if (kind == 4) {
            const int slot = randomInt(random, -1, capacity);
            input << "edge_at " << slot << '\n';
            if (slot < 0 || slot >= capacity) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                const StressEdgeRecord& edge =
                    edges[static_cast<std::size_t>(slot)];
                if (!edge.active) {
                    appendLine(output, "EMPTY");
                } else {
                    output << edge.id << ' ' << edge.left << ' '
                           << edge.right << '\n';
                }
            }
        } else if (kind == 5 || kind == 6) {
            const int vertex = randomInt(random, -1, vertexCount);
            input << (kind == 5 ? "degree " : "neighbors ")
                  << vertex << '\n';
            if (vertex < 0 || vertex >= vertexCount) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (kind == 5) {
                int degree = 0;
                for (const StressEdgeRecord& edge : edges) {
                    if (edge.active &&
                        (edge.left == vertex || edge.right == vertex)) {
                        ++degree;
                    }
                }
                appendLine(output, degree);
            } else {
                std::vector<int> neighbors;
                for (int candidate = 0;
                     candidate < vertexCount;
                     ++candidate) {
                    for (const StressEdgeRecord& edge : edges) {
                        if (stressEdgeHasPair(edge, vertex, candidate)) {
                            neighbors.push_back(candidate);
                            break;
                        }
                    }
                }
                appendIntList(output, neighbors);
            }
        } else if (kind == 7) {
            input << "edge_count\n";
            appendLine(output, edgeCount);
        } else if (kind == 8) {
            input << "free_slots\n";
            appendLine(output, capacity - edgeCount);
        } else {
            input << "clear\n";
            for (StressEdgeRecord& edge : edges) edge.active = false;
            edgeCount = 0;
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerGraphGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F47-undirected-adjacency-matrix", generateUndirectedAdjacencyMatrix);
    registry.emplace("F48-directed-adjacency-matrix", generateDirectedAdjacencyMatrix);
    registry.emplace("F49-undirected-adjacency-list", generateUndirectedAdjacencyList);
    registry.emplace("F50-directed-adjacency-list", generateDirectedAdjacencyList);
    registry.emplace("F51-fixed-edge-table-graph", generateFixedEdgeTableGraph);
}

}  // namespace judge::generators
