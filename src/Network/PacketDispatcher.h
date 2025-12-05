#ifndef PACKET_DISPATCHER_H
#define PACKET_DISPATCHER_H

#include "../Common/Connection.h"
#include "Packet.h"
#include <cassert>
#include <unordered_map>
#include <functional>

namespace DataSerializer {

using PacketTypeID = uint16_t;

class PacketDispatcher {
private:
    std::unordered_map<PacketTypeID, std::function<void(Packet&)>> handlers;
    std::unordered_map<PacketTypeID, std::shared_ptr<void>> signals;

public:
    template<typename T>
    void Register() {
        static_assert(std::is_base_of_v<Packet, T>);
        
        PacketTypeID id = T().GetPacketID();

        auto signal = std::make_shared<Signal<T&>>();
        signals[id] = signal;

        handlers[id] = [signal](Packet& pkt) {
            signal->Fire(static_cast<T&>(pkt));
        };
    }

    template<typename T>
    Signal<T&>& GetSignal() {
        PacketTypeID id = T().GetPacketID();
        auto it = signals.find(id);

        // task: handle
        if (it == signals.end()) {
            assert(false && "Signal not registered");
        }

        return *std::static_pointer_cast<Signal<T&>>(it->second);
    }

    void Dispatch(Packet& packet) {
        PacketTypeID id = packet.GetPacketID();
        auto it = handlers.find(id);
        if (it != handlers.end()) {
            it->second(packet);
        }
    }
};


}

#endif