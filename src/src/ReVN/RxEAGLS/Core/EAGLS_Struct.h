#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <string>


namespace ZQF::ReVN::RxEAGLS
{
#pragma pack(push,1)
    struct Struct
    {
        struct Script
        {
            struct Lable
            {
                std::array<char, 32> lable_name;
                std::uint32_t code_text_offset;
            };
        };
    };
#pragma pack(pop)
}
