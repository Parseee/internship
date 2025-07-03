#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "edge.h"
#include "graph.h"
#include "node.h"

bool Graph::addNode(const std::string& id) {
    if (nodes.find(id) != nodes.end()) {
        return false;
    }
    nodes[id] = std::make_shared<Node>(id);

    for (auto u : nodes) {
        std::cout << u.first << std::endl;
    }
    return true;
}

bool Graph::removeNode(const std::string& id) {
    auto node_it = nodes.find(id);

    if (node_it == nodes.end()) {
        std::cout << "Unknown node " + id << std::endl;
        return false;
    }

    for (auto in_edge : node_it->second->getInEdges()) {
        std::shared_ptr<Node> from_node = in_edge->getFrom();
    }

    return true;
}

std::shared_ptr<Node> Graph::getNode(const std::string& id) const noexcept {
    auto node_it = nodes.find(id);
    return (node_it != nodes.end()) ? node_it->second : nullptr;
}

bool Graph::containsNode(const std::string& id) const noexcept {
    return nodes.find(id) != nodes.end();
}

bool Graph::addEdge(const std::string& from_id, const std::string& to_id,
                    int weight) noexcept {
    auto from_it = nodes.find(from_id);
    auto to_it = nodes.find(to_id);

    if (from_it == nodes.end() && to_it == nodes.end()) {
        std::cout << "Unknown nodes " + from_id + " " + to_id << std::endl;
        return false;
    } else if (from_it == nodes.end()) {
        std::cout << "Unknown node " + from_id << std::endl;
        return false;
    } else if (to_it == nodes.end()) {
        std::cout << "Unknown node " + to_id << std::endl;
        return false;
    }

    auto edge = std::make_shared<Edge>(from_it->second, to_it->second, weight);

    from_it->second->addOutEdge(edge);
    to_it->second->addInEdge(edge);

    // edges.insert(edge);

    return true;
}

bool Graph::removeEdge(const std::string& from_id, const std::string& to_id)
{
    auto from_it = nodes.find(from_id);
    auto to_it = nodes.find(to_id);

    if (from_it == nodes.end() && to_it == nodes.end()) {
        std::cout << "Unknown nodes " + from_id + " " + to_id << std::endl;
        return false;
    } else if (from_it == nodes.end()) {
        std::cout << "Unknown node " + from_id << std::endl;
        return false;
    } else if (to_it == nodes.end()) {
        std::cout << "Unknown node " + to_id << std::endl;
        return false;
    }

    // may brake test system
    std::shared_ptr<Edge> edge = nullptr;
    // if ((edge = getEdge(from_id, to_id)) == nullptr) {
    //     std::cout << "Edge " + from_id + " -> " + to_id + " does not exist"
    //     << std::endl; return false;
    // }

    // TODO: remove
    // from_it->second->removeOutEdge(from_id, to_id);

    return true;
}

// std::shared_ptr<Edge> Graph::getEdge(const std::string& from,
//                                   const std::string& to) const noexcept {

//                                   }

void Graph::dump() {

    std::ofstream graph_dump("graph.dot");

    // TODO: make directed
    graph_dump << "strict graph GG {" << std::endl
               << "node [shape=circle];" << std::endl;

    graphPrint(graph_dump);

    graph_dump << "}" << std::endl;

    system("dot -Tpdf graph.dot -o graph.pdf");
}

void Graph::graphPrint(std::ofstream& graph_dump) const noexcept {
    for (auto node : nodes) {
        graph_dump << node.first << std::endl;

        for (auto edge : node.second->getOutEdges()) {
            graph_dump << "  \"" + edge->getFrom()->getId() + "\" -- \"" +
                              edge->getTo()->getId() + "\"[label=\"" +
                              std::to_string(edge->getWeight()) + "\"];"
                       << std::endl;
        }
    }
}