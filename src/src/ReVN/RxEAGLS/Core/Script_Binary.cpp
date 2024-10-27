#include "Script_Binary.h"
#include <Zut/ZxMem.h>


namespace ZQF::ReVN::RxEAGLS::Script
{
    auto Binary::Load(const std::string_view msPath) -> Binary&
    {
        ZxMem script_mem{ msPath };

        for (auto& lable : m_aLableTable)
        {
            script_mem >> lable;
        }

        const auto code_text_bytes = script_mem.SizeBytesCur() - 2;
        m_msCodeText.resize_and_overwrite(code_text_bytes, [&code_text_bytes](auto, auto) { return code_text_bytes; });
        script_mem >> std::span{ m_msCodeText };

        script_mem.PosInc(1); // skip null char

        script_mem >> m_ucSeed;

        return *this;
    }

    auto Binary::GetCodeText() -> std::string_view
    {
        return m_msCodeText;
    }

    auto Binary::Clear() -> void
    {
        m_aLableTable = {};
        m_msCodeText.clear();
        m_ucSeed = {};
    }
}
