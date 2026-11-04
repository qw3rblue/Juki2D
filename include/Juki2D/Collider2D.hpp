#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <limits>

namespace Juki2D {

using CollisionBits = std::uint32_t;
using ColliderId = std::uint64_t;

struct CollisionFilter {
    CollisionBits category = 1u;
    CollisionBits mask = std::numeric_limits<CollisionBits>::max();
};

constexpr bool canInteract(CollisionFilter a, CollisionFilter b) {
    return (a.mask & b.category) != 0u && (b.mask & a.category) != 0u;
}

struct Collider2D {
    ColliderId id;
    sf::FloatRect bounds;
    CollisionFilter filter{};
    bool isTrigger = false;
};

enum class TriggerPhase { Enter, Stay, Exit };

struct TriggerEvent {
    ColliderId colliderId;
    TriggerPhase phase;
};
}
