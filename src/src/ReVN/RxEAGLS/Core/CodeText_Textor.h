#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    class Textor
    {
    private:
        struct msg_info_t
        {
            enum class type_t
            {
                NameText,
                MsgText,
                SelectText,
                ChapterText,
            };

            type_t type;
            std::string_view text;
        };

    private:
        std::vector<msg_info_t> m_MsgVec;
        std::unordered_map<std::string, std::string> m_NameMap;

    public:
        auto Scan(const std::string_view msCodeText) -> void;

    public:
        auto Export(const std::string_view msSavePath) -> void;

    public:
        auto Clear() -> void;
        auto SetNameMap(const std::string_view msKey, const std::string_view msName) -> bool;
    };
}
