#include "graph.h"
#include "node.h"

#include <iostream>


int main() {
    // Node node;
    Graph graph;

    std::string input;
    while (std::cin >> input) {
        if (input== "node") {
            std::string ctx;
            std::cin >> ctx;
            graph.addNode(ctx);
        } else if (input == "edge") {
            std::string from, to;
            std::cin >> from >> to;
            size_t weight;
            std::cin >> weight;
            graph.addEdge(from, to, weight);
        } else if (input == "remove_node") {
            std::string ctx;
            std::cin >> ctx;
            graph.removeNode(ctx);
        }
        graph.dump();
    }
}