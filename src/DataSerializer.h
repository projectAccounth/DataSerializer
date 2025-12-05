#ifndef DATA_SERIALIZER_H
#define DATA_SERIALIZER_H

#include "Common/Connection.h"
#include "Network/ByteBuffer.h"
#include "Network/CodecBuilder.h"
#include "Network/Packet.h"
#include "Network/PacketCodec.h"
#include "Network/PacketDispatcher.h"
#include "Network/PacketIO.h"
#include "Network/PacketRegistry.h"
#include "Network/VarIntEndian.h"
#include "ProtocolRegistry.h"

namespace DataSerializer {

class Serializer {
private:
    PacketDispatcher dispatcher;
    std::unique_ptr<PacketRegistry> reg = ProtocolRegistry::Create();
    PacketIO pIO = PacketIO(*reg);
public:
    Serializer() = default;

    template <typename T>
    void RegisterPacket() {
        reg->Register<T>(T().GetPacketID());
        dispatcher.Register<T>();
    }

    template <typename T>
    Connection RegisterPacketListener(const std::function<void(const T&)>& callback) {
        return dispatcher.GetSignal<T>().ConnectPersistent(callback);
    }

    std::vector<uint8_t> Serialize(const Packet& packet) const {
        return pIO.EncodePacket(packet);
    }

    std::unique_ptr<Packet> Deserialize(const std::vector<uint8_t>& data) const {
        return pIO.DecodePacket(data);
    }

    void Dispatch(Packet& packet) {
        dispatcher.Dispatch(packet);
    }

    PacketCodec CreatePacketCodec() const {
        return PacketCodec();
    }
};

}

#endif