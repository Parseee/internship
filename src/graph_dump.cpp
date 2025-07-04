#include <fstream>

#include "graph.h"

void Graph::dump() {

    std::ofstream graph_dump("graph.dot");

    // TODO: make directed
    graph_dump << "strict digraph GG {" << std::endl
               << "node [shape=circle];" << std::endl;

    graphPrint(graph_dump);

    graph_dump << "}" << std::endl;

    system("dot -Tpdf graph.dot -o graph.pdf");
}

void Graph::graphPrint(std::ofstream& graph_dump) const noexcept {
    for (auto node : nodes) {
        graph_dump << node.first << std::endl;

        for (auto edge : node.second->getOutEdges()) {
            graph_dump << "  \"" + edge->getFrom()->getId() + "\" -> \"" +
                              edge->getTo()->getId() + "\"[label=\"" +
                              std::to_string(edge->getWeight()) + "\"];"
                       << std::endl;
        }
    }
}