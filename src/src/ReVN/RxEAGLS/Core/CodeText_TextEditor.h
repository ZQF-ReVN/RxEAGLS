#pragma once
#include <vector>
#include <unordered_map>
#include <Zut/ZxMem.h>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    class TextEditor
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
        ZxMem m_CodeTextMem;
        std::vector<msg_info_t> m_MsgVec;
        std::unordered_map<std::string, std::string> m_NameMap;

    public:
        auto ScanViaPath(const std::string_view msCodeTextPath) -> void;

    public:
        auto ExportViaJson(const std::string_view msSavePath) -> void;

    public:
        auto Clear() -> void;
        auto SetNameMap(const std::string_view msKey, const std::string_view msName) -> bool;
    };
}
