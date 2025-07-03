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
        graph.addEdge("a", "b", 10);
        graph.removeEdge("a", "b");
        graph.dump();
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}