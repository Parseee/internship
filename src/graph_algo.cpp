#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

#include "edge.h"
#include "graph.h"
#include "node.h"

void Graph::dfs(
    std::shared_ptr<Node> node,
    std::unordered_map<std::shared_ptr<Node>, enum vertex_state>& used,
    std::vector<std::shared_ptr<Node>>& traversal,
    std::shared_ptr<Node>& cycled) const {
    traversal.push_back(node);

    used[node] = IN;

    for (auto edge_it = node->getOutEdges().rbegin();
         edge_it != node->getOutEdges().rend(); ++edge_it) {
        auto next_node = (*edge_it)->getTo();
        if (used[next_node] == NIL) {
            dfs(next_node, used, traversal, cycled);
        } else if (used[next_node] == IN) {
            cycled = node;
            // return;
        }
    }

    used[node] = OUT;

    return;
}

std::vector<std::shared_ptr<Node>>
Graph::RPO(const std::string& id) const noexcept {
    auto start_node = getNode(id);

    if (start_node == nullptr) {
        std::cout << "Unknown node " + id << std::endl;
        return {};
    }

    std::vector<std::shared_ptr<Node>> traversal;
    std::unordered_map<std::shared_ptr<Node>, enum vertex_state> used;

    std::shared_ptr<Node> cycle_node = nullptr;
    dfs(start_node, used, traversal, cycle_node);
    if (cycle_node) {
        std::cout << "Found loop " + start_node->getId() + "->" +
                         cycle_node->getId()
                  << std::endl;
    }

    return traversal;
}

std::unordered_map<std::shared_ptr<Node>, uint64_t>
Graph::shortestPaths(const std::string& id) const noexcept {
    auto start_node_ptr = getNode(id);

    if (start_node_ptr == nullptr) {
        std::cout << "Unknown node " + id << std::endl;
        return {};
    }

    std::unordered_map<std::shared_ptr<Node>, uint64_t> dist;
    for (const auto& node : nodes) {
        dist[node.second] = std::numeric_limits<uint64_t>::max();
    }
    dist[start_node_ptr] = 0;
    std::set<std::pair<uint64_t, std::shared_ptr<Node>>> priority;

    priority.insert({0, start_node_ptr});

    while (!priority.empty()) {
        auto cur_node_ptr = priority.begin()->second;
        priority.erase(priority.begin());

        for (const auto& edge : cur_node_ptr->getOutEdges()) {
            if (dist[edge->getFrom()] != std::numeric_limits<uint64_t>::max() &&
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

uint64_t Graph::maxFlow(const std::string& start_id,
                        const std::string& end_id) {
    auto start_node = getNode(start_id);
    auto end_node = getNode(end_id);

    if (!start_node || !end_node) {
        std::cout << "Unknown source or sink" << std::endl;
        return 0;
    }

    std::unordered_map<std::shared_ptr<Edge>, uint64_t> flow;
    for (const auto& edge : edges) { // no need
        if (auto e = edge.lock()) {
            flow[e] = 0;
        }
    }

    uint64_t max_flow = 0;

    while (true) {
        std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Edge>> parent;
        parent[start_node] = nullptr;
        std::unordered_map<std::shared_ptr<Node>, bool> used;
        used[start_node] = true;

        std::queue<std::shared_ptr<Node>> q;
        q.push(start_node);

        bool found_path = false;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            for (const auto& edge : current->getOutEdges()) {
                if (edge->getWeight() > flow[edge] && !used[edge->getTo()]) {
                    parent[edge->getTo()] = edge;
                    used[edge->getTo()] = true;
                    q.push(edge->getTo());

                    if (edge->getTo() == end_node) {
                        found_path = true;
                        break;
                    }
                }
            }
            for (const auto& edge : current->getInEdges()) {
                if (flow[edge] > 0 && !used[edge->getFrom()]) {
                    auto reversed_edge = std::make_shared<Edge>(
                        edge->getTo(), edge->getFrom(), 0);

                    flow[reversed_edge] = flow[edge];
                    parent[reversed_edge->getTo()] = edge;
                    used[reversed_edge->getTo()] = true;
                    q.push(reversed_edge->getTo());

                    if (reversed_edge->getTo() == end_node) {
                        found_path = true;
                        break;
                    }
                }
            }
        }

        if (!found_path) {
            break;
        }

        uint64_t path_flow = std::numeric_limits<uint64_t>::max();
        for (auto node = end_node; node != start_node;) {
            auto edge = parent[node];
            if (edge->getTo() == node) { // forward edge
                path_flow = std::min(path_flow, edge->getWeight() - flow[edge]);
                node = edge->getFrom();
            } else { // reversed edge
                path_flow = std::min(path_flow, flow[edge]);
                node = edge->getTo();
            }
        }
        for (auto node = end_node; node != start_node;) {
            auto edge = parent[node];
            if (edge->getTo() == node) { // forward edge
                flow[edge] += path_flow;
                node = edge->getFrom();
            } else { // reversed edge
                flow[edge] -= path_flow;
                node = edge->getTo();
            }
        }

        max_flow += path_flow;
    }

    return max_flow;
}

void Graph::t_dfs(std::shared_ptr<Node> node,
                  std::unordered_map<std::shared_ptr<Node>, bool>& used,
                  uint64_t& time,
                  std::unordered_map<std::shared_ptr<Node>, uint64_t>& index,
                  std::unordered_map<std::shared_ptr<Node>, uint64_t>& low_id,
                  std::stack<std::shared_ptr<Node>>& node_stack,
                  std::unordered_map<std::shared_ptr<Node>, bool>& on_stack,
                  size_t& scc_count) {
    used[node] = true;
    index[node] = low_id[node] = time++;
    node_stack.push(node);
    on_stack[node] = true;

    for (const auto& edge : node->getOutEdges()) {
        auto next_node = edge->getTo();

        if (!used[next_node]) {
            t_dfs(next_node, used, time, index, low_id, node_stack, on_stack,
                  scc_count);
            low_id[node] = std::min(low_id[next_node], low_id[node]);
        } else if (on_stack[next_node]) {
            low_id[node] = std::min(index[next_node], low_id[node]);
        }
    }

    if (index[node] == low_id[node]) {
        for (auto cur_node = node_stack.top();; cur_node = node_stack.top()) {
            on_stack[cur_node] = false;
            node_stack.pop();
            low_id[cur_node] = index[node];
            if (node == cur_node) {
                break;
            }
        }
        ++scc_count;
    }
}

std::unordered_map<uint64_t, std::vector<std::shared_ptr<Node>>>
Graph::findSCC() {
    std::unordered_map<std::shared_ptr<Node>, uint64_t> low_id;
    std::unordered_map<std::shared_ptr<Node>, uint64_t>
        indicies; // current id for each node
    std::unordered_map<std::shared_ptr<Node>, bool> used;
    uint64_t time = 0;

    std::stack<std::shared_ptr<Node>> stack;
    std::unordered_map<std::shared_ptr<Node>, bool> on_stack;

    size_t scc_count = 0;
    for (const auto& [id, node] : nodes) {
        if (!used[node]) {
            t_dfs(node, used, time, indicies, low_id, stack, on_stack,
                  scc_count);
        }
    }

    std::unordered_map<uint64_t, std::vector<std::shared_ptr<Node>>> scc;

    for (const auto& [node, low] : low_id) {
        scc[low].push_back(node);
    }

    return scc;
}