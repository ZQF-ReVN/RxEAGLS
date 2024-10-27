#include <print>
#include <iostream>
#include <Zut/ZxFS.h>
#include <Zut/ZxMem.h>
#include <Zut/ZxCvt.h>
#include <Zut/ZxFile.h>
#include <ReVN/RxEAGLS/Core/Script_Dat.h>
#include <ReVN/RxEAGLS/Core/CodeText_Textor.h>
#include <ReVN/RxEAGLS/Core/CodeText_Formatter.h>


namespace RxEAGLS
{
    using namespace ZQF::ReVN::RxEAGLS;
}


[[maybe_unused]] static auto FormatCodeText(const std::string_view msCodeTextDir, const std::string_view msFormatedDir) -> void
{
    ZxFS::DirMakeRecursive(msFormatedDir);

    ZxMem code_text_mem;
    RxEAGLS::CodeText::Formatter formatter;
    for (ZxFS::Walker walker{ msCodeTextDir }; walker.NextFile(); formatter.Clear())
    {
        code_text_mem.Load(walker.GetPath());
        const auto code_text{ std::string_view{ code_text_mem.Ptr<const char*>(), code_text_mem.SizeBytes() } };
        const auto formated{ formatter.Format(code_text) };
        const auto save_path{ std::string{ msFormatedDir }.append(walker.GetName()) };
        ZxFile::SaveDataViaPath(save_path, std::span{ formated }, true, true);
    }
}

[[maybe_unused]] static auto ExportCodeText(const std::string_view msScriptDir, const std::string_view msCodeTextDir) -> void
{
    ZxFS::DirMakeRecursive(msCodeTextDir);

    ZxCvt cvt;
    RxEAGLS::Script::Dat dat;
    for (ZxFS::Walker walker{ msScriptDir }; walker.NextFile(); dat.Clear())
    {
        dat.Load(walker.GetPath());
        const auto code_text{ cvt.MBCSToUTF8(dat.GetCodeText(), 932) };
        const auto save_path{ std::string{ msCodeTextDir }.append(walker.GetNameStem()).append(".txt") };
        ZxFile{ save_path, ZxFile::OpenMod::WriteForce }.Write(std::span{ code_text });
    }
}

[[maybe_unused]] static auto ExportJsonText(const std::string_view msCodeTextDir, const std::string_view msJsonDir) -> void
{
    ZxFS::DirMakeRecursive(msJsonDir);

    ZxMem code_text_mem;
    RxEAGLS::CodeText::Textor textor;

    textor.SetNameMap(":NameSuffix", "和也");

    for (ZxFS::Walker walker{ msCodeTextDir }; walker.NextFile(); textor.Clear())
    {
        code_text_mem.Load(walker.GetPath());
        const auto code_text{ std::string_view{ code_text_mem.Ptr<const char*>(), code_text_mem.SizeBytes() } };
        textor.Scan(code_text);
        const auto save_path{ std::string{ msJsonDir }.append(walker.GetNameStem()).append(".json") };
        textor.Export(save_path);
    }
}


auto main() -> int
{
    try
    {
        ::ExportCodeText("script/", "script_text/code_text/");
        ::FormatCodeText("script_text/code_text/", "script_text/code_text_formated/");
        ::ExportJsonText("script_text/code_text_formated/", "script_text/json/");
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
