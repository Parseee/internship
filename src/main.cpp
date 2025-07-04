#include "graph.h"

#include <exception>
#include <iostream>

int main() {
    Graph graph;

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

    try {
        graph.addNode("a");
        graph.addNode("b");
        graph.addNode("c");
        graph.addNode("d");
        graph.addNode("e");
        graph.addNode("f");
        graph.addEdge("a", "b", 7);
        graph.addEdge("a", "c", 4);
        graph.addEdge("b", "c", 4);
        graph.addEdge("b", "e", 2);
        graph.addEdge("c", "e", 8);
        graph.addEdge("c", "d", 4);
        graph.addEdge("d", "f", 12);
        graph.addEdge("e", "d", 4);
        graph.addEdge("e", "f", 5);
        for (auto [node, dist] : graph.shortestPaths("a")) {
            std::cout << node->getId() << " " << dist << std::endl;
        }
        graph.dump();
        for (auto node : graph.RPO("a")) {
            std::cout << node->getId() << " ";
        }
        std::cout << graph.maxFlow("a", "f") << std::endl;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}