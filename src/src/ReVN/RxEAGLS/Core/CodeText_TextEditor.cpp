#include "CodeText_TextEditor.h"
#include "CodeText_Lexical.h"
#include <Zut/ZxJson.h>


namespace ZQF::ReVN::RxEAGLS::CodeText
{
    auto TextEditor::SetNameMap(const std::string_view msKey, const std::string_view msName) -> bool
    {
        const auto [ite, status] = m_NameMap.try_emplace(std::string{ msKey }, std::string{ msName });
        return status;
    }

    auto TextEditor::ScanViaPath(const std::string_view msCodeTextPath) -> void
    {
        m_CodeTextMem.Load(msCodeTextPath);

        for (CodeText::Lexical lexical{ std::string_view{ m_CodeTextMem.Ptr<const char*>(), m_CodeTextMem.SizeBytes() } }; lexical.ParseNext();)
        {
            switch (lexical.GetCurTokenType())
            {
            case CodeText::TokenType::ShowName:
            {
                std::string_view name;
                const auto name_text_raw = lexical.GetCurContain().substr(1);
                if (const auto pos1 = name_text_raw.find(','); pos1 != std::string_view::npos)
                {
                    name = name_text_raw.substr(0, pos1);
                }
                else if (const auto pos2 = name_text_raw.find('='); pos2 != std::string_view::npos)
                {
                    name = name_text_raw.substr(0, pos2);
                }
                else if (name_text_raw.starts_with(":NameSuffix"))
                {
                    const auto ite = m_NameMap.find(std::string{ name_text_raw });
                    if (ite != m_NameMap.end())
                    {
                        name = ite->second;
                    }
                    else
                    {
                        name = name_text_raw;
                    }
                }
                else
                {
                    name = name_text_raw;
                }

                m_MsgVec.emplace_back(msg_info_t{ .type = msg_info_t::type_t::NameText,.text = name });
            }
            break;

            case CodeText::TokenType::ShowText:
            {
                auto msg_text_raw = lexical.GetCurContain();
                msg_text_raw = msg_text_raw.substr(0, msg_text_raw.size() - 1);
                const auto msg_text = msg_text_raw.substr(msg_text_raw.find('\"') + 1);
                m_MsgVec.emplace_back(msg_info_t{ .type = msg_info_t::type_t::MsgText,.text = msg_text });
            }
            break;

            case CodeText::TokenType::Invoke:
            {
                if (const auto invoke_text_raw = lexical.GetCurContain(); invoke_text_raw.starts_with("52(\"_SelStr"))
                {
                    const auto beg_pos = invoke_text_raw.find(",\"") + 2;
                    const auto end_pos = invoke_text_raw.find("\")");
                    if (const auto select_text = invoke_text_raw.substr(beg_pos, end_pos - beg_pos); select_text.size())
                    {
                        m_MsgVec.emplace_back(msg_info_t{ .type = msg_info_t::type_t::SelectText,.text = select_text });
                    }
                }
                else if (invoke_text_raw.starts_with("47(\""))
                {
                    if (const auto chapter_text = invoke_text_raw.substr(4, invoke_text_raw.size() - 6); chapter_text.size())
                    {
                        m_MsgVec.emplace_back(msg_info_t{ .type = msg_info_t::type_t::ChapterText,.text = chapter_text });
                    }
                }
            }
            break;
            }
        }
    }

    auto TextEditor::ExportViaJson(const std::string_view msSavePath) -> void
    {
        if (m_MsgVec.empty()) { return; }

        ZxJson::JValue msg_info_json;
        auto& msg_info_json_vec = msg_info_json.ToArray();

        std::string_view name;
        for (const auto& msg_info : m_MsgVec)
        {
            switch (msg_info.type)
            {
            case msg_info_t::type_t::NameText:
            {
                name = msg_info.text;
            }
            break;

            case msg_info_t::type_t::MsgText:
            {
                if (name.size())
                {
                    msg_info_json_vec.emplace_back
                    (
                        ZxJson::JObject_t
                        {
                            { "Chr", name },
                            { "Msg", msg_info.text },
                            { "Tra", msg_info.text }
                        }
                    );
                }
                else
                {
                    msg_info_json_vec.emplace_back
                    (
                        ZxJson::JObject_t
                        {
                            { "Msg", msg_info.text },
                            { "Tra", msg_info.text }
                        }
                    );
                }
                name = {};
            }
            break;

            case msg_info_t::type_t::SelectText:
            {
                msg_info_json_vec.emplace_back
                (
                    ZxJson::JObject_t
                    {
                        { "Sel", msg_info.text },
                        { "Tra", msg_info.text }
                    }
                );
            }
            break;

            case msg_info_t::type_t::ChapterText:
            {
                msg_info_json_vec.emplace_back
                (
                    ZxJson::JObject_t
                    {
                        { "Chp", msg_info.text },
                        { "Tra", msg_info.text }
                    }
                );
            }
            break;

            }
        }


        ZxJson::StoreViaFile(msSavePath, msg_info_json, true, true);
    }

    auto TextEditor::Clear() -> void
    {
        m_MsgVec.clear();
    }
}
