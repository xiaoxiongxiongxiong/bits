#include <iostream>
#include <inttypes.h>
#include "bits.h"

typedef struct _bits_kv_t
{
    size_t bits;
    union
    {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
    } val;
} bits_kv_t;

int main(int argc, char * argv[])
{
    srand(static_cast<uint32_t>(time(nullptr)));
    auto writer = CBitsStreamWriter();
    std::vector<bits_kv_t> kvs;
    for (int i = 0; i < 20; ++i)
    {
        bits_kv_t kv = {};
        kv.bits = static_cast<size_t>(rand()) % 63 + 1;
        auto val = rand() % (1 << kv.bits);
        if (kv.bits <= 8)
            kv.val.u8 = static_cast<uint8_t>(val);
        else if (kv.bits <= 16)
            kv.val.u16 = static_cast<uint16_t>(val);
        else if (kv.bits <= 32)
            kv.val.u32 = static_cast<uint32_t>(val);
        else
            kv.val.u64 = static_cast<uint64_t>(val);
        kvs.emplace_back(kv);
        writer.WriteBits(kv.bits, kv.val.u64);
    }

    auto * data = writer.GetData();
    auto len = writer.GetLength();

    printf("%-8s %-8s %-8s\n", "bits", "w-val", "r-val");

    auto reader = CBitsStreamReader(data, len);
    for (const auto & kv : kvs)
    {
        uint64_t val = 0;
        if (reader.ReadBits(kv.bits, val))
            printf("%-8zu %-8" PRIu64 " %-8" PRIu64 "\n", kv.bits, kv.val.u64, val);
        else
            printf("Read %zu bits failed\n", kv.bits);
    }

    return 0;
}
