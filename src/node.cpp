#include <algorithm>
#include <memory>

#include "edge.h"
#include "node.h"

Node::Node(std::string id) : id(id) {}

const std::string& Node::getId() const { return id; }

const std::vector<std::shared_ptr<Edge>>& Node::getInEdges() const {
    return in_edges;
}

const std::vector<std::shared_ptr<Edge>>& Node::getOutEdges() const {
    return out_edges;
}

void Node::addInEdge(std::shared_ptr<Edge> edge) {
    this->in_edges.push_back(edge);
}

void Node::addOutEdge(std::shared_ptr<Edge> edge) {
    this->out_edges.push_back(edge);
}

void Node::removeInEdge(const std::shared_ptr<Edge> edge) {
    in_edges.erase(std::remove(in_edges.begin(), in_edges.end(), edge),
                   in_edges.end());
}

void Node::removeOutEdge(const std::shared_ptr<Edge> edge) {
    out_edges.erase(std::remove(out_edges.begin(), out_edges.end(), edge),
                    out_edges.end());
}

void Node::clearEdges() {
    auto in_copy = std::move(in_edges);
    auto out_copy = std::move(out_edges);

    in_edges.clear();
    out_edges.clear();

    for (auto& edge : in_copy) {
        if (auto from = edge->getFrom()) {
            from->removeOutEdge(edge);
        }
    }

    for (auto& edge : out_copy) {
        if (auto to = edge->getTo()) {
            to->removeInEdge(edge);
        }
    }
}