#ifndef EXAMPLE_PACKET_H
#define EXAMPLE_PACKET_H

#include "../Network/Packet.h"

struct AssignPlayerPacket : public DataSerializer::Packet {
    int data1 = 0, data2 = 0;

    AssignPlayerPacket() = default;
    AssignPlayerPacket(int d1, int d2):
        data1(d1), data2(d2) {}

    constexpr static uint16_t PACKET_ID = 16;

    uint16_t GetPacketID() const override { return PACKET_ID; }

    void Encode(DataSerializer::PacketCodec& codec) const override {
        codec.Write(data1);
        codec.Write(data2);
    }

    void Decode(DataSerializer::PacketCodec& codec) override {
        data1 = codec.Read<int>();
        data2 = codec.Read<int>();
    }
};

#endif