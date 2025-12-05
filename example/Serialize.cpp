#include "../src/DataSerializer.h"
#include <cassert>
#include <iostream>

class DataPacket : public DataSerializer::Packet {
public:
    std::string data0, data1;

    static constexpr uint16_t PACKET_ID = 4;

    uint16_t GetPacketID() const override { return PACKET_ID; }

    void Encode(DataSerializer::PacketCodec& codec) const override {
        codec.WriteString(data0);
        codec.WriteString(data1);
    }

    void Decode(DataSerializer::PacketCodec& codec) override {
        data0 = codec.ReadString();
        data1 = codec.ReadString();
    }

    bool operator==(const DataPacket& other) const {
        return data0 == other.data0 && data1 == other.data1;
    }
};

int main() {
    DataSerializer::Serializer serializer;

    serializer.RegisterPacket<DataPacket>();

    DataPacket packet;
    packet.data0 = "Hello World!";
    packet.data1 = "From ESP-NOW!";

    serializer.RegisterPacketListener<DataPacket>([&packet](const DataPacket& pkt) {
        std::cout << pkt.data0 << '\n' << pkt.data1 << '\n';

        assert(pkt == packet);
    });

    auto serialized = serializer.Serialize(packet);
    auto deserialized = serializer.Deserialize(serialized);

    serializer.Dispatch(*deserialized);

    std::cout << "Test successful!\n";
    return 0;
}