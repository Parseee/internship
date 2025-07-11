#include "graph.h"
#include "node.h"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <sys/types.h>
#include <utility>
#include <vector>

void processCommands(Graph& graph) {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        try {
            if (command == "NODE") {
                std::string nodeId;
                iss >> nodeId;
                graph.addNode(nodeId);
            } else if (command == "EDGE") {
                std::string from, to;
                int weight;
                iss >> from >> to >> weight;
                graph.addEdge(from, to, weight);
            } else if (command == "REMOVE") {
                std::string subcmd;
                iss >> subcmd;

                if (subcmd == "NODE") {
                    std::string nodeId;
                    iss >> nodeId;
                    graph.removeNode(nodeId);
                } else if (subcmd == "EDGE") {
                    std::string from, to;
                    iss >> from >> to;
                    graph.removeEdge(from, to);
                }
            } else if (command == "RPO_NUMBERING") {
                std::string startNode;
                iss >> startNode;

                auto traversal = graph.RPO(startNode);
                std::sort(traversal.begin(), traversal.end(),
                          [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
                              return a->getId() < b->getId();
                          });
                for (const auto& node : traversal) {
                    std::cout << node->getId() << " ";
                }
                std::cout << std::endl;
            } else if (command == "DIJKSTRA") {
                std::string startNode;
                iss >> startNode;
                std::vector<std::pair<std::shared_ptr<Node>, uint64_t>>
                    distances(graph.shortestPaths(startNode).begin(),
                              graph.shortestPaths(startNode).end());

                std::sort(distances.begin(), distances.end(),
                          [](auto a, auto b) {
                              return a.first->getId() < b.first->getId();
                          });

                for (const auto& [node, dist] : distances) {
                    if (dist != std::numeric_limits<uint64_t>::max()) {
                        std::cout << node->getId() << " " << dist << std::endl;
                    } else {
                        std::cout << node->getId() << " is unreachable"
                                  << std::endl;
                    }
                }
            } else if (command == "MAX") {
                std::string subcmd;
                iss >> subcmd;

                if (subcmd == "FLOW") {
                    std::string source, sink;
                    iss >> source >> sink;
                    uint64_t maxFlow = graph.maxFlow(source, sink);
                    if (maxFlow != std::numeric_limits<uint64_t>::max()) {
                        std::cout << maxFlow << std::endl;
                    }
                }
            } else if (command == "TARJAN") {
                std::string startNode;
                iss >> startNode;

                using vectorOfComponents = std::vector<
                    std::pair<uint64_t, std::vector<std::shared_ptr<Node>>>>;

                vectorOfComponents sccs(graph.findSCC().begin(),
                                        graph.findSCC().end());

                std::sort(sccs.begin(), sccs.end(), [](auto l, auto r) {
                    return l.second.size() < r.second.size();
                });

                for (const auto& [id, component] : graph.findSCC()) {
                    if (component.size() > 1) {
                        std::vector<std::shared_ptr<Node>> tmp = component;
                        std::sort(tmp.begin(), tmp.end(), [](auto l, auto r) {
                            return l->getId() < r->getId();
                        });
                        for (const auto& node : tmp) {
                            std::cout << node->getId() << " ";
                        }
                        std::cout << std::endl;
                    }
                }
            } else if (command == "DUMP") {
                graph.dump();
            } else {
                std::cout << "Unknown command: " << command << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

int main() {
    Graph graph;

    processCommands(graph);
}