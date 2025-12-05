#ifndef PACKET_CODEC_H
#define PACKET_CODEC_H

#include <vector>
#include <string>
#include <cstring>
#include <type_traits>
#include <stdexcept>
#include <cstdint>

namespace DataSerializer {

//
// PacketCodec — handles encoding/decoding primitives and custom types.
//
class PacketCodec {
private:
    std::vector<uint8_t> buffer;
    size_t read_pos = 0;

public:
    PacketCodec() = default;
    explicit PacketCodec(std::vector<uint8_t> data) : buffer(std::move(data)) {}

    const std::vector<uint8_t>& Data() const { return buffer; }
    size_t Size() const { return buffer.size(); }
    void Reset() { buffer.clear(); read_pos = 0; }
    void ResetRead() { read_pos = 0; }

    const std::vector<uint8_t>& GetBuffer() const { return buffer; }
    void SetBuffer(const std::vector<uint8_t>& buf) { buffer = buf; }

    //
    // Encode
    //
    template<typename T>
    void Write(const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable.");
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
        buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
    }

    // String (length-prefixed)
    void WriteString(const std::string& s) {
        uint32_t len = static_cast<uint32_t>(s.size());
        Write(len);
        buffer.insert(buffer.end(), s.begin(), s.end());
    }

    // Byte array
    void WriteBytes(const std::vector<uint8_t>& data) {
        uint32_t len = static_cast<uint32_t>(data.size());
        Write(len);
        buffer.insert(buffer.end(), data.begin(), data.end());
    }

    void WriteFloat(float value) {
        Write(value);
    }

    void WriteDouble(double value) {
        Write(value);
    }

    void WriteStringArray(const std::vector<std::string>& vec) {
        Write<size_t>(vec.size());
        for (auto& str : vec) 
            WriteString(str);
    }

    //
    // Decode
    //
    template<typename T>
    T Read() {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable.");
        if (read_pos + sizeof(T) > buffer.size()) {
            throw std::runtime_error("PacketCodec: insufficient data for read.");
        }
        T value;
        std::memcpy(&value, buffer.data() + read_pos, sizeof(T));
        read_pos += sizeof(T);
        return value;
    }

    std::string ReadString() {
        uint32_t len = Read<uint32_t>();
        if (read_pos + len > buffer.size()) {
            throw std::runtime_error("PacketCodec: string length overflow.");
        }
        std::string s(reinterpret_cast<const char*>(buffer.data() + read_pos), len);
        read_pos += len;
        return s;
    }

    std::vector<uint8_t> ReadBytes() {
        uint32_t len = Read<uint32_t>();
        if (read_pos + len > buffer.size()) {
            throw std::runtime_error("PacketCodec: byte array length overflow.");
        }
        std::vector<uint8_t> data(buffer.begin() + read_pos, buffer.begin() + read_pos + len);
        read_pos += len;
        return data;
    }

    std::vector<std::string> ReadStringArray() {
        auto size = Read<size_t>();
        std::vector<std::string> out(size);

        for (std::string& str : out) 
            str = ReadString();

        return out;
    }
};

}

#endif