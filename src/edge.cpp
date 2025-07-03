#include "edge.h"
#include <memory>

size_t Edge::getWeight() const { return this->weight; }

const std::shared_ptr<Node> Edge::getFrom() const noexcept {
    return from.lock();
}

const std::shared_ptr<Node> Edge::getTo() const noexcept {
    return to.lock();
}

bool Edge::isEqual(const std::string& from, const std::string& to) const noexcept {
    if (this->from.lock()->getId() == from && this->to.lock()->getId() == to) {
        return true;
    }
    return false;
}

bool Node::removeInEdge(const std::string& from, const std::string& to) {
    in_edges.erase(std::remove_if(in_edges.begin(), in_edges.end(), [](std::shared_ptr<Edge> edge){
        return edge
    }))
}