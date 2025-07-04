#include <cstdint>
#include <iostream>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "node.h"

std::vector<std::shared_ptr<Node>> Graph::RPO(std::string id) const noexcept {
    auto start_node_ptr = getNode(id);

    if (start_node_ptr == nullptr) {
        std::cout << "Unknown node " + id;
    }

    std::vector<std::shared_ptr<Node>> traversal;
    std::stack<std::shared_ptr<Node>> dfs;
    enum vertex_state { NIL, IN, OUT };
    std::unordered_map<std::shared_ptr<Node>, enum vertex_state> visited;
    std::unordered_map<std::shared_ptr<Node>, bool> rec_stack;

    dfs.push(start_node_ptr);
    traversal.push_back(start_node_ptr);

    while (!dfs.empty()) {
        auto cur_node = dfs.top();
        dfs.pop();

        if (visited[cur_node] != OUT) {
            visited[cur_node] = IN;
        }

        for (auto edge_it = cur_node->getOutEdges().rbegin();
             edge_it != cur_node->getOutEdges().rend(); ++edge_it) {
            auto edge = *edge_it;
            if (visited[edge->getTo()] == NIL) {
                dfs.push(edge->getTo());
                traversal.push_back(edge->getTo());
            } else {
                std::cout << "Found loop " + start_node_ptr->getId() + " -> " +
                                 cur_node->getId()
                          << std::endl;
            }
        }

        visited[cur_node] = OUT;
    }

    return traversal;
}

std::unordered_map<std::shared_ptr<Node>, uint64_t>
Graph::shortestPaths(std::string id) const noexcept {
    auto start_node_ptr = getNode(id);

    if (start_node_ptr == nullptr) {
        std::cout << "Unknown node " + id << std::endl;
    }

    std::unordered_map<std::shared_ptr<Node>, uint64_t> dist;
    for (auto node : nodes) {
        dist[node.second] = INT64_MAX;
    }
    dist[start_node_ptr] = 0;
    std::set<std::pair<uint64_t, std::shared_ptr<Node>>> priority;

    priority.insert({0, start_node_ptr});

    while (!priority.empty()) {
        auto cur_node_ptr = priority.begin()->second;
        priority.erase(priority.begin());

        for (auto edge : cur_node_ptr->getOutEdges()) {
            if (dist[edge->getFrom()] != INT64_MAX &&
                dist[edge->getTo()] >
                    dist[edge->getFrom()] + edge->getWeight()) {
                priority.erase({dist[edge->getTo()], edge->getTo()});
                dist[edge->getTo()] = dist[edge->getFrom()] + edge->getWeight();
                priority.insert({dist[edge->getTo()], edge->getTo()});
            }
        }
    }

    return dist;
}

