#include <algorithm>
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
    
    return true;
}

bool Graph::removeNode(const std::string& id) {
    auto node_it = nodes.find(id);

    if (node_it == nodes.end()) {
        std::cout << "Unknown node " + id << std::endl;
        return false;
    }

    node_it->second->clearEdges();

    nodes.erase(node_it);

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

    edges.push_back(edge);

    return true;
}

bool Graph::removeEdge(const std::string& from_id, const std::string& to_id) {
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

    std::shared_ptr<Node> from_node = from_it->second;
    std::shared_ptr<Node> to_node = to_it->second;

    auto from_edge_it = std::find_if(
        from_node->getOutEdges().begin(), from_node->getOutEdges().end(),
        [&](const std::shared_ptr<Edge>& edge_ptr) {
            return edge_ptr->isEqual(from_id, to_id);
        });
    auto to_edge_it =
        std::find_if(to_node->getInEdges().begin(), to_node->getInEdges().end(),
                     [&](const std::shared_ptr<Edge>& edge_ptr) {
                         return edge_ptr->isEqual(from_id, to_id);
                     });

    if (from_edge_it == from_node->getOutEdges().end() ||
        to_edge_it == to_node->getInEdges().end()) {
        return false;
    }
    edges.erase(std::remove_if(edges.begin(), edges.end(), [&](auto edge) {
        return edge.lock()->isEqual(from_id, to_id);
    }));
    from_node->removeOutEdge(*from_edge_it);
    to_node->removeInEdge(*to_edge_it);

    return true;
}

std::shared_ptr<Edge> Graph::getEdge(const std::string& from_id,
                                     const std::string& to_id) const noexcept {
    auto edge_it = std::find_if(edges.begin(), edges.end(), [&](auto edge) {
        return edge.lock()->isEqual(from_id, to_id);
    });

    return (edge_it != edges.end()) ? edge_it->lock() : nullptr;
}

bool Graph::containsEdge(const std::string& from_id,
                         const std::string& to_id) const noexcept {
    auto edge_it = std::find_if(edges.begin(), edges.end(), [&](auto edge) {
        return edge.lock()->isEqual(from_id, to_id);
    });

    return edge_it != edges.end();
}