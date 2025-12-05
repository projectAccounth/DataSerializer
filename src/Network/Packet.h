#ifndef PACKET_H
#define PACKET_H

#include "PacketCodec.h"
#include <cstdint>

namespace DataSerializer {

//
// Base Packet class
//
class Packet {
public:
    virtual ~Packet() = default;

    virtual uint16_t GetPacketID() const = 0;

    virtual void Encode(PacketCodec& codec) const = 0;
    virtual void Decode(PacketCodec& codec) = 0;
};

}

#endif