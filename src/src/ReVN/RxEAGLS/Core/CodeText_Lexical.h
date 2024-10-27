#pragma once
#include <string>
#include <string_view>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    enum class TokenType
    {
        Label,
        Invoke,
        Expression,
        ShowText,
        ShowName,
        BlockHeader,
        BlockBeg,
        BlockEnd,
        End
    };

    class Lexical
    {
    private:
        std::size_t m_nReadBytes{};
        std::string_view m_msText;

    private:
        TokenType m_eCurType{};
        std::size_t m_nCurTypeBytes{};

    public:
        Lexical(const std::string_view msText);

    public:
        auto ParseNext() -> bool;
        auto GetCurTokenType() const -> TokenType;
        auto GetCurTokenTypeAsStr() -> std::string_view;
        auto GetCurContain() -> std::string_view;

    private:
        auto SkipWhite() -> std::size_t;
        auto SkipText() -> std::size_t;
        auto SkipNumber() -> std::size_t;
        auto ReadNumber() -> std::string_view;
        auto SkipRoundBraket() -> std::size_t;
        auto ReadRoundBraket() -> std::string_view;
        auto SkipQuotedString() -> std::size_t;
        auto IsEnd() const -> bool;
        auto ReadChar() -> char;
        auto SkipChar() -> std::size_t;
        auto GetCurChar() const -> char;
        auto GetCurStr() const -> std::string_view;
    };
}
