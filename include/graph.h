#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "node.h"

class Graph {
  public:
  private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
};