#pragma once

#include <memory>
#include <string>
#include <vector>

class Edge;

class Node {
  public:
    explicit Node(std::string id);
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    const std::string& getId() const;
    const std::vector<std::shared_ptr<Edge>>& getInEdges() const;
    const std::vector<std::shared_ptr<Edge>>& getOutEdges() const;

    void addInEdge(const std::shared_ptr<Edge> edge);
    void addOutEdge(const std::shared_ptr<Edge> edge);

    friend bool hasInEdgeFrom(const std::string& node_id);
    friend bool hasOutEdgeTo(const std::string& node_id);

  private:
    std::string id;
    std::vector<std::shared_ptr<Edge>> in_edges;
    std::vector<std::shared_ptr<Edge>> out_edges;
};