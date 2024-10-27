#pragma once
#include <string>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    class Formatter
    {
    public:
        class FormattedBuf
        {
        private:
            std::string m_msBuf;
            std::size_t m_nScopeLevel{};

        public:
            auto PushIndent() -> void;
            auto PushContainln(const std::string_view msContain) -> void;
            auto Clear() -> void;
            auto IncScope() -> void;
            auto SubScope() -> void;
            auto GetStr() const -> std::string_view;
        };

    private:
        FormattedBuf m_FormattedBuf;

    public:
        Formatter() {};

    public:
        auto Format(const std::string_view msCodeTextPath) -> std::string_view;

    public:
        auto Clear() -> void;
        auto GetFormatedStr() const -> std::string_view;
    };

}
