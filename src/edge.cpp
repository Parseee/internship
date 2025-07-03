#include "edge.h"
#include <memory>

size_t Edge::getWeight() const { return this->weight; }

const std::shared_ptr<Node> Edge::getFrom() const noexcept {
    return from.lock();
}

const std::shared_ptr<Node> Edge::getTo() const noexcept { return to.lock(); }

bool Edge::isEqual(const std::string& from,
                   const std::string& to) const noexcept {
    if (this->from.lock()->getId() == from && this->to.lock()->getId() == to) {
        return true;
    }
    return false;
}