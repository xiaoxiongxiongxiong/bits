#ifndef _OS_BITS_H_
#define _OS_BITS_H_

#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <vector>

class CBitsStreamWriter
{
public:
    CBitsStreamWriter() = default;
    ~CBitsStreamWriter() = default;

    // 获取当前位置
    void GetCurOffset(size_t & bytes, size_t & bits);
    // 设置当前位置
    void SetCurOffset(size_t bytes, size_t bits);

    // 获取数据
    const uint8_t * GetData();
    // 获取数据长度
    size_t GetLength();

    // 写入uint64_t数据，bits范围1~64
    void WriteBits(size_t bits, uint64_t num);
    // 写入uint8_t数据
    void WriteBitsU8(uint8_t num);
    // 写入uint16_t数据
    void WriteBitsU16(uint16_t num);
    // 写入uint32_t数据
    void WriteBitsU32(uint32_t num);
    // 写入uint64_t数据
    void WriteBitsU64(uint64_t num);

private:
    // 消耗bits
    void ConsumeBits(size_t bits);

private:
    // 数据
    std::vector<uint8_t> _buff;

    // 当前bytes
    size_t _cur_bytes = 0;
    // 当前bits
    size_t _cur_bits = 0;
    // 剩余bits
    size_t _left_bits = 8;
    // 可用位数
    size_t _avail_bits = 0;

    // 总字节数
    size_t _total_bytes = 0;
    // 总位数
    size_t _total_bits = 0;
};

class CBitsStreamReader
{
public:
    CBitsStreamReader(const uint8_t * data, size_t len);
    ~CBitsStreamReader();

    // 获取当前位置
    void GetCurOffset(size_t & bytes, size_t & bits);
    // 设置当前位置
    bool SetCurOffset(size_t bytes, size_t bits);

    // 读取uint64_t数据，bits范围1~64
    bool ReadBits(size_t bits, uint64_t & num);
    // 读取uint8_t数据
    bool ReadBitsU8(uint8_t & num);
    // 读取uint16_t数据
    bool ReadBitsU16(uint16_t & num);
    // 读取uint32_t数据
    bool ReadBitsU32(uint32_t & num);
    // 读取uint64_t数据
    bool ReadBitsU64(uint64_t & num);

private:
    // 消耗bits
    void ConsumeBits(size_t bits);

private:
    // 数据长度
    size_t _total_bytes = 0;
    // 数据
    uint8_t * _data = nullptr;

    // 可用位数
    size_t _avail_bits = 0;
    //剩余字节数
    size_t _left_bits = 8;
    // 当前bytes
    size_t _cur_bytes = 0;
    // 当前bits
    size_t _cur_bits = 0;
};

#endif
