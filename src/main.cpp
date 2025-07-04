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
        graph.addEdge("a", "b", 10);
        graph.addEdge("a", "e", 10);
        graph.addEdge("b", "c", 15);
        graph.addEdge("b", "d", 10);
        graph.addEdge("c", "a", 10);
        graph.addEdge("d", "a", 10);
        // graph.addEdge("d", "a", 10);
        graph.dump();
        // graph.removeEdge("a", "b");
        for (auto node : graph.RPO("a")) {
            std::cout << node->getId() << " ";
        }
        // graph.dump();
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}