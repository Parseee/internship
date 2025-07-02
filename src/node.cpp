#include "node.h"
#include <memory>

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