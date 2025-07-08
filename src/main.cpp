#include "graph.h"

#include <exception>
#include <iostream>
#include <sstream>

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

                for (const auto& node : graph.RPO(startNode)) {
                    std::cout << node->getId() << " ";
                }
                std::cout << std::endl;
            } else if (command == "DIJKSTRA") {
                std::string startNode;
                iss >> startNode;
                auto distances = graph.shortestPaths(startNode);

                for (const auto& [node, dist] : distances) {
                    std::cout << node->getId() << " " << dist << std::endl;
                }
            } else if (command == "MAX") {
                std::string subcmd;
                iss >> subcmd;

                if (subcmd == "FLOW") {
                    std::string source, sink;
                    iss >> source >> sink;
                    int maxFlow = graph.maxFlow(source, sink);
                    std::cout << maxFlow << std::endl;
                }
            } else if (command == "TARJAN") {
                std::string startNode;
                iss >> startNode;

                for (const auto& [id, component] : graph.findSCC()) {
                    if (component.size() > 1) {
                        for (const auto& node : component) {
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

    // std::string input;
    // while (std::cin >> input) {
    //     if (input == "node") {
    //         std::string ctx;
    //         std::cin >> ctx;
    //         graph.addNode(ctx);
    //     } else if (input == "edge") {
    //         std::string from, to;
    //         std::cin >> from >> to;
    //         size_t weight;
    //         std::cin >> weight;
    //         graph.addEdge(from, to, weight);
    //     } else if (input == "remove_node") {
    //         std::string ctx;
    //         std::cin >> ctx;
    //         graph.removeNode(ctx);
    //     } else if (input == "remove_edge") {
    //         std::string from, to;
    //         std::cin >> from >> to;
    //         graph.removeEdge(from, to);
    //     }
    //     graph.dump();
    // }

    // try {
    // graph.addNode("a");
    // graph.addNode("b");
    // graph.addNode("c");
    // graph.addNode("d");
    // graph.addNode("e");
    // graph.addNode("f");
    // graph.addEdge("a", "b", 7);
    // graph.addEdge("a", "c", 4);
    // graph.addEdge("b", "c", 4);
    // graph.addEdge("b", "e", 2);
    // graph.addEdge("c", "e", 8);
    // graph.addEdge("c", "d", 4);
    // graph.addEdge("d", "f", 12);
    // graph.addEdge("e", "d", 4);
    // graph.addEdge("e", "f", 5);

    // graph.addNode("a");
    // graph.addNode("b");
    // graph.addNode("c");
    // graph.addNode("d");
    // graph.addNode("e");
    // graph.addNode("f");
    // graph.addNode("g");
    // graph.addNode("h");
    // graph.addEdge("a", "b", 3);
    // graph.addEdge("b", "a", 3);
    // graph.addEdge("a", "c", 5);
    // graph.addEdge("b", "d", 5);
    // graph.addEdge("c", "d", 7);
    // graph.addEdge("d", "e", 2);
    // graph.addEdge("e", "c", 6);
    // graph.addEdge("d", "f", 3);
    // graph.addEdge("e", "f", 3);
    // graph.addEdge("e", "g", 3);
    // graph.addEdge("f", "h", 3);
    // graph.addEdge("h", "g", 3);
    // graph.addEdge("g", "f", 3);

    // for (auto [node, dist] : graph.shortestPaths("a")) {
    //     std::cout << node->getId() << " " << dist << std::endl;
    // }
    // graph.dump();
    // for (auto node : graph.RPO("a")) {
    //     std::cout << node->getId() << " ";
    // }
    // std::cout << std::endl;
    // std::cout << graph.maxFlow("a", "f") << std::endl;
    // std::cout << graph.maxFlow("a", "f") << std::endl;

    // for (const auto& [id, component] : graph.findSCC()) {
    //     // if (component.size() > 1) {
    //     for (const auto& node : component) {
    //         std::cout << node->getId() << " ";
    //     }
    //     std::cout << std::endl;
    //     // }
    // }
    // } catch (const std::exception& e) {
    //    std::cout << e.what() << std::endl;
    // }
}