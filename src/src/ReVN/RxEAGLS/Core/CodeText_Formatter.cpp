#include "CodeText_Formatter.h"
#include "CodeText_Lexical.h"


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    auto Formatter::FormattedBuf::PushIndent() -> void
    {
        for (std::size_t idx{}; idx < m_nScopeLevel; idx++)
        {
            m_msBuf.append(1, '\t');
        }
    }

    auto Formatter::FormattedBuf::PushContainln(const std::string_view msContain) -> void
    {
        m_msBuf.append(msContain).append(1, '\n');
    }

    auto Formatter::FormattedBuf::Clear() -> void
    {
        m_msBuf.clear();
    }

    auto Formatter::FormattedBuf::IncScope() -> void
    {
        m_nScopeLevel++;
    }

    auto Formatter::FormattedBuf::SubScope() -> void
    {
        m_nScopeLevel--;
    }

    auto Formatter::FormattedBuf::GetStr() const -> std::string_view
    {
        return m_msBuf;
    }


    auto Formatter::Format(const std::string_view msCodeText) -> std::string_view
    {
        for (CodeText::Lexical lexical{ msCodeText }; lexical.ParseNext();)
        {
            if (lexical.GetCurTokenType() == CodeText::TokenType::BlockBeg)
            {
                m_FormattedBuf.PushIndent();
                m_FormattedBuf.PushContainln(lexical.GetCurContain());
                m_FormattedBuf.IncScope();
            }
            else if (lexical.GetCurTokenType() == CodeText::TokenType::BlockEnd)
            {
                m_FormattedBuf.SubScope();
                m_FormattedBuf.PushIndent();
                m_FormattedBuf.PushContainln(lexical.GetCurContain());
            }
            else
            {
                m_FormattedBuf.PushIndent();
                m_FormattedBuf.PushContainln(lexical.GetCurContain());
            }
        }

        return this->GetFormatedStr();
    }

    auto Formatter::Clear() -> void
    {
        m_FormattedBuf.Clear();
    }

    auto Formatter::GetFormatedStr() const -> std::string_view
    {
        return m_FormattedBuf.GetStr();
    }
}
