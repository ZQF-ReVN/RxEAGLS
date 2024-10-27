#include "CodeText_Lexical.h"
#include <stdexcept>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    Lexical::Lexical(const std::string_view msText) : m_msText{ msText }
    {

    }

    auto Lexical::ParseNext() -> bool
    {
        this->SkipWhite();

        if (this->IsEnd())
        {
            m_eCurType = TokenType::End;
            return false;
        }

        const auto cur_char = this->GetCurChar();
        if (cur_char == '$')
        {
            m_eCurType = TokenType::Label;
            m_nCurTypeBytes = this->SkipChar() + this->SkipText();
        }
        else if (cur_char == '_' || cur_char == '@' || cur_char == ':')
        {
            m_eCurType = TokenType::Expression;
            m_nCurTypeBytes = this->SkipChar() + this->SkipText();
        }
        else if (cur_char == '&')
        {
            m_eCurType = TokenType::ShowText;
            m_nCurTypeBytes = this->SkipChar() + this->SkipNumber() + this->SkipQuotedString();
        }
        else if (cur_char == '#')
        {
            m_eCurType = TokenType::ShowName;
            m_nCurTypeBytes = this->SkipChar() + this->SkipText();
        }
        else if (cur_char == '{')
        {
            m_eCurType = TokenType::BlockBeg;
            m_nCurTypeBytes = this->SkipChar();
        }
        else if (cur_char == '}')
        {
            m_eCurType = TokenType::BlockEnd;
            m_nCurTypeBytes = this->SkipChar() + this->SkipNumber() + this->SkipChar();
        }
        else if (('0' <= cur_char) && ('9' >= cur_char))
        {
            const auto num_bytes = this->SkipNumber();

            if (this->GetCurChar() == '(')
            {
                m_eCurType = TokenType::Invoke;
                m_nCurTypeBytes = num_bytes + this->SkipRoundBraket();
            }
            else if (this->GetCurChar() == ',')
            {
                m_eCurType = TokenType::BlockHeader;
                m_nCurTypeBytes = num_bytes + this->SkipChar() + this->SkipNumber();

                if (this->GetCurChar() == '(')
                {
                    m_nCurTypeBytes += this->SkipRoundBraket();
                }
            }
            else
            {
                throw std::runtime_error("RxEAGLS::CodeText::Lexical::ParseNext(): parse block error!");
            }
        }
        else
        {
            throw std::runtime_error("RxEAGLS::CodeText::Lexical::ParseNext(): error token!");
        }

        return true;
    }

    auto Lexical::GetCurTokenType() const -> TokenType
    {
        return m_eCurType;
    }

    auto Lexical::GetCurTokenTypeAsStr() -> std::string_view
    {
        switch (m_eCurType)
        {
        case TokenType::Label: return "Label";
        case TokenType::Invoke: return "Invoke";
        case TokenType::Expression: return "Expression";
        case TokenType::ShowText: return "ShowText";
        case TokenType::ShowName: return "ShowName";
        case TokenType::BlockHeader: return "BlockHeader";
        case TokenType::BlockBeg: return "BlockBeg";
        case TokenType::BlockEnd: return "BlockEnd";
        case TokenType::End:  return "End";
        default: return "";
        }
    }

    auto Lexical::GetCurContain() -> std::string_view
    {
        return { m_msText.data() + m_nReadBytes - m_nCurTypeBytes, m_nCurTypeBytes };
    }


    auto Lexical::SkipWhite() -> std::size_t
    {
        std::size_t skip_chars{};
        for (std::size_t idx = m_nReadBytes; idx < m_msText.size(); idx++)
        {
            const auto c = m_msText[idx];
            if (c == '\n' || c == '\r' || c == '\t' || c == ' ')
            {
                skip_chars++;
                continue;
            }
            break;
        }

        m_nReadBytes += skip_chars;

        return skip_chars;
    }

    auto Lexical::SkipText() -> std::size_t
    {
        std::size_t ite_chars{};
        for (std::size_t idx = m_nReadBytes; idx < m_msText.size(); idx++)
        {
            const auto c = m_msText[idx];
            if (c == '\n' || c == '\r' || c == '\t')
            {
                break;
            }
            ite_chars++;
        }

        m_nReadBytes += ite_chars;

        return ite_chars;
    }

    auto Lexical::SkipNumber() -> std::size_t
    {
        std::size_t ite_chars{};
        for (std::size_t idx = m_nReadBytes; idx < m_msText.size(); idx++)
        {
            const auto c = m_msText[idx];
            if (('0' <= c) && ('9' >= c))
            {
                ite_chars++;
                continue;
            }
            break;
        }

        m_nReadBytes += ite_chars;

        return ite_chars;
    }

    auto Lexical::ReadNumber() -> std::string_view
    {
        const auto beg_pos = m_nReadBytes;
        const auto bytes = this->SkipNumber();
        return { m_msText.data() + beg_pos, bytes };
    }

    auto Lexical::SkipRoundBraket() -> std::size_t
    {
        std::size_t ite_chars{};
        for (std::size_t idx = m_nReadBytes; idx < m_msText.size(); idx++)
        {
            const auto c = m_msText[idx];
            if (c == ')')
            {
                ite_chars = idx - m_nReadBytes + 1;
                break;
            }
            else if (c == '"')
            {
                idx++;
                for (; idx < m_msText.size(); idx++)
                {
                    const auto cc = m_msText[idx];
                    if (cc == '"')
                    {
                        break;
                    }
                }
            }
        }

        m_nReadBytes += ite_chars;

        return ite_chars;
    }

    auto Lexical::ReadRoundBraket() -> std::string_view
    {
        return this->GetCurStr().substr(0, this->SkipRoundBraket());
    }

    auto Lexical::SkipQuotedString() -> std::size_t
    {
        const auto cur_text = this->GetCurStr();
        const auto quote_str_chars = cur_text.find('"', 1) + 1;
        m_nReadBytes += quote_str_chars;
        return quote_str_chars;
    }

    auto Lexical::IsEnd() const -> bool
    {
        return m_nReadBytes >= m_msText.size();
    }

    auto Lexical::ReadChar() -> char
    {
        return m_msText[m_nReadBytes++];
    }

    auto Lexical::SkipChar() -> std::size_t
    {
        m_nReadBytes++;
        return 1;
    }

    auto Lexical::GetCurChar() const -> char
    {
        return m_msText[m_nReadBytes];
    }

    auto Lexical::GetCurStr() const -> std::string_view
    {
        return m_msText.substr(m_nReadBytes);
    }

}
