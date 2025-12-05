#ifndef PACKET_REGISTRY_H
#define PACKET_REGISTRY_H

#include "Packet.h"
#include <cassert>
#include <limits>
#include <unordered_map>
#include <memory>
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
public:
    template<typename T>
    void Register(uint16_t id) {
        static_assert(std::is_base_of_v<Packet, T>, "T must inherit from Packet.");
        if (factories.find(id) != factories.end()) {
            assert(false && "Duplicate registration");
            return;
        }
        factories[id] = []() -> std::unique_ptr<Packet> { return std::make_unique<T>(); };
    }

    std::unique_ptr<Packet> Create(uint16_t id) const {
        auto it = factories.find(id);
        if (it == factories.end()) {
            assert(false && ("Unknown packet ID: " + std::to_string(id)).c_str());
        }
        return it->second();
    }
};

}

#endif