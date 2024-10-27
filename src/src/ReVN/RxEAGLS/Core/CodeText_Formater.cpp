#include "CodeText_Formater.h"
#include "CodeText_Lexical.h"


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

    auto Formater::FormatedBuf::GetStr() const -> std::string_view
    {
        return m_msBuf;
    }


    auto Formater::Format(const std::string_view msCodeText) -> std::string_view
    {
        for (CodeText::Lexical lexical{ msCodeText }; lexical.ParseNext();)
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

        return this->GetFormatedStr();
    }

    auto Formater::Clear() -> void
    {
        m_FormatedBuf.Clear();
    }

    auto Formater::GetFormatedStr() const -> std::string_view
    {
        return m_FormatedBuf.GetStr();
    }
}
