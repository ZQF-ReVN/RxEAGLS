#pragma once
#include <array>
#include <string>
#include <string_view>
#include <ReVN/RxEAGLS/Core/EAGLS_Struct.h>


namespace ZQF::ReVN::RxEAGLS::Script
{
    class Dat
    {
    private:
        std::array<Struct::Script::Lable, 100> m_aLableTable{};
        std::string m_msCodeText;
        std::uint8_t m_ucSeed{};

    public:
        auto Load(const std::string_view msPath) -> Dat&;

    public:
        auto Clear() -> void;
        auto GetCodeText() -> std::string_view;
    };

}
