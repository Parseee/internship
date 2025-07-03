#pragma once

#include <memory>
#include <string>

#include "node.h"

class Edge {
  public:
    Edge() {};
    explicit Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to,
                  size_t weight)
        : from(from), to(to), weight(weight) {}

    const std::shared_ptr<Node> getFrom() const noexcept;
    const std::shared_ptr<Node> getTo() const noexcept;
    size_t getWeight() const;

    bool isEqual(const std::string& from, const std::string& to) const noexcept;

  private:
    std::weak_ptr<Node> from;
    std::weak_ptr<Node> to;
    size_t weight;
};