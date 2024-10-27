#pragma once
#include <string>
#include <Zut/ZxMem.h>



namespace ZQF::ReVN::RxEAGLS::CodeText
{
    class Formater
    {
    public:
        class FormatedBuf
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
            auto GetSpan() const->std::span<const char>;
        };

    private:
        ZxMem m_CodeTextMem;
        FormatedBuf m_FormatedBuf;

    public:
        auto FromPath(const std::string_view msCodeTextPath) -> void;
        auto SaveFormated(const std::string_view msSavePath) -> void;
    };

}
