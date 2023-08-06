#include "bits.h"

void CBitsStreamWriter::GetCurOffset(size_t & bytes, size_t & bits)
{
    bytes = _cur_bytes;
    bits = _cur_bits;
}

void CBitsStreamWriter::SetCurOffset(size_t bytes, size_t bits)
{
    _cur_bytes = _total_bytes < bytes ? _total_bytes : bytes;
    _cur_bits = bits < 8 ? bits : 8;
}

const uint8_t * CBitsStreamWriter::GetData()
{
    return _buff.data();
}

size_t CBitsStreamWriter::GetLength()
{
    return _total_bytes;
}

void CBitsStreamWriter::WriteBits(size_t bits, uint64_t num)
{
    if (_avail_bits < bits)
    {
        const auto total_bits = _total_bits + bits;
        _total_bytes = static_cast<size_t>(ceil(static_cast<double>(total_bits) / 8.0));
        _buff.resize(_total_bytes);
        _avail_bits = _total_bytes * 8 - _total_bits;
    }

    while (bits > 0)
    {
        size_t consume_bits = 0;
        if (_left_bits >= bits)
        {
            _buff[_cur_bytes] |= (num << (_left_bits - bits));
            consume_bits = bits;
        }
        else
        {
            _buff[_cur_bytes] |= (num >> (bits - _left_bits));
            consume_bits = _left_bits;
        }
        ConsumeBits(consume_bits);
        bits -= consume_bits;
    }
}

void CBitsStreamWriter::WriteBitsU8(uint8_t num)
{
    WriteBits(8, static_cast<uint64_t>(num));
}

void CBitsStreamWriter::WriteBitsU16(uint16_t num)
{
    WriteBits(16, static_cast<uint64_t>(num));
}

void CBitsStreamWriter::WriteBitsU32(uint32_t num)
{
    WriteBits(32, static_cast<uint64_t>(num));
}

void CBitsStreamWriter::WriteBitsU64(uint64_t num)
{
    WriteBits(64, num);
}

void CBitsStreamWriter::ConsumeBits(size_t bits)
{
    _total_bits += bits;
    _cur_bytes += (_cur_bits + bits) / 8;
    _cur_bits = (_cur_bits + bits) % 8;
    _avail_bits -= bits;
    _left_bits = 8 - _cur_bits;
}

CBitsStreamReader::CBitsStreamReader(const uint8_t * data, size_t len)
{
    if (nullptr != data && len > 0)
    {
        _data = new(std::nothrow) uint8_t[len];
        if (nullptr != _data)
        {
            memcpy(_data, data, len);
            _total_bytes = len;
            _avail_bits = _total_bytes * 8;
        }
    }
}

CBitsStreamReader::~CBitsStreamReader()
{
    if (nullptr != _data)
    {
        delete[] _data;
        _data = nullptr;
        _total_bytes = 0;
    }

    _left_bits = 0;
    _avail_bits = 0;
    _cur_bits = 0;
    _cur_bytes = 0;
}

void CBitsStreamReader::GetCurOffset(size_t & bytes, size_t & bits)
{
    bytes = _cur_bytes;
    bits = _cur_bits;
}

bool CBitsStreamReader::SetCurOffset(size_t bytes, size_t bits)
{
    if (bytes > _total_bytes || bits > 8)
        return false;
    _cur_bytes = bytes;
    _cur_bits = bits;
    return true;
}

bool CBitsStreamReader::ReadBits(size_t bits, uint64_t & num)
{
    if (bits > 64 || _avail_bits < bits)
        return false;
    if (0 == bits)
    {
        num = 0;
        return true;
    }

    while (bits > 0)
    {
        size_t consume_bits = 0;
        uint64_t val = _data[_cur_bytes] & (0xff >> _cur_bits);
        if (0 == _cur_bits)
        {
            if (bits < 8)
            {
                val >>= (8 - bits);
                num = val | (num <<= bits);
            }
            else
                num = val | (num << 8);
            consume_bits = bits < 8 ? bits : 8;
        }
        else
        {
            if (bits < _left_bits)
                val >>= (_left_bits - bits);
            num = val | num << bits;
            consume_bits = _left_bits > bits ? bits : _left_bits;
        }
        ConsumeBits(consume_bits);
        bits -= consume_bits;
    }

    return true;
}

bool CBitsStreamReader::ReadBitsU8(uint8_t & num)
{
    uint64_t val = 0;
    auto res = ReadBits(8, val);
    num = static_cast<uint8_t>(val);
    return res;
}

bool CBitsStreamReader::ReadBitsU16(uint16_t & num)
{
    uint64_t val = 0;
    auto res = ReadBits(16, val);
    num = static_cast<uint16_t>(val);
    return res;
}

bool CBitsStreamReader::ReadBitsU32(uint32_t & num)
{
    uint64_t val = 0;
    auto res = ReadBits(32, val);
    num = static_cast<uint32_t>(val);
    return res;
}

bool CBitsStreamReader::ReadBitsU64(uint64_t & num)
{
    return ReadBits(64, num);
}

void CBitsStreamReader::ConsumeBits(size_t bits)
{
    _cur_bytes += (_cur_bits + bits) / 8;
    _cur_bits = (_cur_bits + bits) % 8;
    _left_bits = 8 - _cur_bits;
    _avail_bits -= bits;
}
