#ifndef PROTOCOL_REGISTRY_H
#define PROTOCOL_REGISTRY_H

#include "Network/PacketRegistry.h"
#include "Packets/ExamplePacket.h"

namespace DataSerializer {

class ProtocolRegistry {
public:
    static std::unique_ptr<PacketRegistry> Create() {
        auto reg = std::make_unique<PacketRegistry>();
        return reg;
    }
};

}

#endif