#pragma once

#include <memory>
#include <string>
// #include <type_traits>
// #include <vector>

#include "node.h"

#ifdef TRAITS
template <typename Iterator>
concept edge_type_iterator = requires {
    requires std::is_same_v<typename std::iterator_traits<Iterator>::value_type,
                            Edge>;
};
#endif /* TRAITS */

class Edge {
  public:
    Edge() {};
    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to,
         uint64_t weight = 0)
        : from(from), to(to), weight(weight) {}

    const std::shared_ptr<Node> getFrom() const noexcept;
    const std::shared_ptr<Node> getTo() const noexcept;
    uint64_t getWeight() const;

    bool isEqual(const std::string& from, const std::string& to) const noexcept;

#ifdef TRAITS
    friend edge_type_iterator auto findInRange(edge_type_iterator auto begin,
                                               edge_type_iterator auto end,
                                               std::string from_id,
                                               std::string to_id);
#endif /* TRAITS */

  private:
    std::weak_ptr<Node> from;
    std::weak_ptr<Node> to;
    uint64_t weight;
};
