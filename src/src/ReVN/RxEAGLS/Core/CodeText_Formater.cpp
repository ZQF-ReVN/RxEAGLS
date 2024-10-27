#include "CodeText_Formater.h"
#include "CodeText_Lexical.h"
#include <Zut/ZxFile.h>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    auto Formater::FormatedBuf::PushIndent() -> void
    {
        for (std::size_t idx{}; idx < m_nScopeLevel; idx++)
        {
            m_msBuf.append(1, '\t');
        }
    }

    auto Formater::FormatedBuf::PushContainln(const std::string_view msContain) -> void
    {
        m_msBuf.append(msContain).append(1, '\n');
    }

    auto Formater::FormatedBuf::Clear() -> void
    {
        m_msBuf.clear();
    }

    auto Formater::FormatedBuf::IncScope() -> void
    {
        m_nScopeLevel++;
    }

    auto Formater::FormatedBuf::SubScope() -> void
    {
        m_nScopeLevel--;
    }

    auto Formater::FormatedBuf::GetSpan() const -> std::span<const char>
    {
        return std::span{ m_msBuf.data(), m_msBuf.size() };
    }


    auto Formater::FromPath(const std::string_view msCodeTextPath) -> void
    {
        m_CodeTextMem.Load(msCodeTextPath);

        for (CodeText::Lexical lexical{ std::string_view{ m_CodeTextMem.Ptr<const char*>(), m_CodeTextMem.SizeBytes() } }; lexical.ParseNext();)
        {
            if (lexical.GetCurTokenType() == CodeText::TokenType::BlockBeg)
            {
                m_FormatedBuf.PushIndent();
                m_FormatedBuf.PushContainln(lexical.GetCurContain());
                m_FormatedBuf.IncScope();
            }
            else if (lexical.GetCurTokenType() == CodeText::TokenType::BlockEnd)
            {
                m_FormatedBuf.SubScope();
                m_FormatedBuf.PushIndent();
                m_FormatedBuf.PushContainln(lexical.GetCurContain());
            }
            else
            {
                m_FormatedBuf.PushIndent();
                m_FormatedBuf.PushContainln(lexical.GetCurContain());
            }
        }
    }

    auto Formater::SaveFormated(const std::string_view msSavePath) -> void
    {
        ZxFile::SaveDataViaPath(msSavePath, m_FormatedBuf.GetSpan(), true, true);
    }
}
