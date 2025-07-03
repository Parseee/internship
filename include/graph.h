#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <set>

#include "node.h"

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
    // std::shared_ptr<Edge> getEdge(const std::string& from,
    //                               const std::string& to) const noexcept;

    // const std::unordered_map<std::string, std::shared_ptr<Node>>&
    // getNodes() const;
    // const std::unordered_map<std::string, std::weak_ptr<Edge>>&
    // getEdges() const;

    void dump();

  private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    std::set<std::weak_ptr<Edge>> edges;

    void graphPrint(std::ofstream& graph_dump) const noexcept;
};