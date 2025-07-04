#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "node.h"
#include "edge.h"

class Graph {
  public:
    Graph() {}
    bool addNode(const std::string& id);
    bool removeNode(const std::string& id);
    std::shared_ptr<Node> getNode(const std::string& id) const noexcept;
    bool containsNode(const std::string& id) const noexcept;

    bool addEdge(const std::string& from, const std::string& to,
                 int weight) noexcept;
    bool removeEdge(const std::string& from, const std::string& to);
    std::shared_ptr<Edge> getEdge(const std::string& from_id,
                                  const std::string& to_id) const noexcept;
    bool containsEdge(const std::string& from_id,
                                       const std::string& to_id) const noexcept;

    std::vector<std::shared_ptr<Node>> RPO(std::string id) const noexcept;

    void dump();

  private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    std::list<std::weak_ptr<Edge>> edges;

    void graphPrint(std::ofstream& graph_dump) const noexcept;
};