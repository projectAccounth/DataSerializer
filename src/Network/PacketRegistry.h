#ifndef PACKET_REGISTRY_H
#define PACKET_REGISTRY_H

#include "Packet.h"
#include <limits>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <functional>
#include <cstdint>
#include <stdexcept>

namespace DataSerializer {

//
// PacketRegistry — handles mapping IDs to packet constructors.
//
class PacketRegistry {
private:
    using CreateFunc = std::function<std::unique_ptr<Packet>()>;

    std::unordered_map<uint16_t, CreateFunc> factories;
    std::unordered_map<std::type_index, uint16_t> type_to_id;

public:
    template<typename T>
    void Register(uint16_t id) {
        static_assert(std::is_base_of_v<Packet, T>, "T must inherit from Packet.");
        if (factories.find(id) != factories.end()) {
            return;
        }
        factories[id] = []() -> std::unique_ptr<Packet> { return std::make_unique<T>(); };
        type_to_id[typeid(T)] = id;
    }

    std::unique_ptr<Packet> Create(uint16_t id) const {
        auto it = factories.find(id);
        if (it == factories.end()) {
            throw std::runtime_error("Unknown packet ID: " + std::to_string(id));
        }
        return it->second();
    }

    template<typename T>
    uint16_t GetID() const {
        auto it = type_to_id.find(typeid(T));
        if (it == type_to_id.end()) {
            return std::numeric_limits<uint16_t>::max();
        }
        return it->second;
    }
};

}

#endif