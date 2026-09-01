#pragma once

#include <vector>
#include <cstdint>
#include "../core/Math.hpp"
#include "../core/Random.hpp"

namespace gta2 {

struct RoadNode {
    uint32_t id = 0;
    Vec2 position;
    bool isIntersection = false;
    std::vector<uint32_t> connectedNodeIds;
};

struct TrafficLight {
    Vec2 position;
    enum class State : uint8_t { Green = 0, Yellow, Red } state = State::Green;
    float timer = 0.0f;
    float greenDuration = 8.0f;
    float yellowDuration = 2.0f;
    float redDuration = 8.0f;

    void update(float dt) {
        timer += dt;
        switch (state) {
            case State::Green:
                if (timer >= greenDuration) {
                    state = State::Yellow;
                    timer = 0.0f;
                }
                break;
            case State::Yellow:
                if (timer >= yellowDuration) {
                    state = State::Red;
                    timer = 0.0f;
                }
                break;
            case State::Red:
                if (timer >= redDuration) {
                    state = State::Green;
                    timer = 0.0f;
                }
                break;
        }
    }
};

class RoadGraph {
public:
    RoadGraph() = default;

    void clear() {
        m_nodes.clear();
        m_trafficLights.clear();
    }

    uint32_t addNode(const Vec2& pos, bool isIntersection = false) {
        uint32_t id = static_cast<uint32_t>(m_nodes.size());
        m_nodes.push_back(RoadNode{id, pos, isIntersection, {}});
        return id;
    }

    void connectNodes(uint32_t fromId, uint32_t toId, bool bidirectional = true) {
        if (fromId < m_nodes.size() && toId < m_nodes.size()) {
            m_nodes[fromId].connectedNodeIds.push_back(toId);
            if (bidirectional) {
                m_nodes[toId].connectedNodeIds.push_back(fromId);
            }
        }
    }

    void addTrafficLight(const Vec2& pos) {
        m_trafficLights.push_back(TrafficLight{pos});
    }

    void update(float dt) {
        for (auto& light : m_trafficLights) {
            light.update(dt);
        }
    }

    [[nodiscard]] const std::vector<RoadNode>& getNodes() const noexcept { return m_nodes; }
    [[nodiscard]] const std::vector<TrafficLight>& getTrafficLights() const noexcept { return m_trafficLights; }

    [[nodiscard]] uint32_t findNearestNode(const Vec2& pos, float maxDist = 500.0f) const {
        uint32_t bestId = 0;
        float bestDistSq = maxDist * maxDist;
        for (const auto& node : m_nodes) {
            float distSq = node.position.distanceSqTo(pos);
            if (distSq < bestDistSq) {
                bestDistSq = distSq;
                bestId = node.id;
            }
        }
        return bestId;
    }

    [[nodiscard]] uint32_t getRandomConnectedNode(uint32_t currentNodeId, FastRandom& rng) const {
        if (currentNodeId >= m_nodes.size() || m_nodes[currentNodeId].connectedNodeIds.empty()) {
            return currentNodeId;
        }
        const auto& connections = m_nodes[currentNodeId].connectedNodeIds;
        int idx = rng.nextInt(0, static_cast<int>(connections.size()) - 1);
        return connections[static_cast<size_t>(idx)];
    }

private:
    std::vector<RoadNode> m_nodes;
    std::vector<TrafficLight> m_trafficLights;
};

} // namespace gta2
