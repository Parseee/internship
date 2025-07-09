#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "edge.h"
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
    std::shared_ptr<Edge> getEdge(const std::string& from_id,
                                  const std::string& to_id) const noexcept;
    bool containsEdge(const std::string& from_id,
                      const std::string& to_id) const noexcept;

    std::vector<std::shared_ptr<Node>>
    RPO(const std::string& id) const noexcept;
    std::unordered_map<std::shared_ptr<Node>, uint64_t>
    shortestPaths(const std::string& id) const noexcept;
    uint64_t maxFlow(const std::string& start_id, const std::string& end_id);
    std::unordered_map<uint64_t, std::vector<std::shared_ptr<Node>>> findSCC();

    void dump();

  private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    std::list<std::weak_ptr<Edge>> edges;
    enum vertex_state { NIL, IN, OUT };

    void graphPrint(std::ofstream& graph_dump) const noexcept;
    void dfs(std::shared_ptr<Node> node,
             std::unordered_map<std::shared_ptr<Node>, enum vertex_state>& used,
             std::vector<std::shared_ptr<Node>>& traversal,
             std::shared_ptr<Node>& cycled) const;
    bool searchPath(
        std::shared_ptr<Node> start, std::shared_ptr<Node> end,
        std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Edge>>& path,
        std::unordered_map<std::shared_ptr<Edge>, uint64_t>& flow);

    void t_dfs(std::shared_ptr<Node> node,
               std::unordered_map<std::shared_ptr<Node>, bool>& used,
               uint64_t& id,
               std::unordered_map<std::shared_ptr<Node>, uint64_t>& ids,
               std::unordered_map<std::shared_ptr<Node>, uint64_t>& low_id,
               std::stack<std::shared_ptr<Node>>& tstack,
               std::unordered_map<std::shared_ptr<Node>, bool>& on_stack,
               size_t& scc_count);
};