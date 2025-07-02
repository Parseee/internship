#pragma once

#include <memory>

#include "node.h"

class Edge {
  public:
    explicit Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to,
                  size_t weight)
        : from(from), to(to), weight(weight) {}

    size_t getWeight() const;

  private:
    std::weak_ptr<Node> from;
    std::weak_ptr<Node> to;
    size_t weight;
};