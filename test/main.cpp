#include <print>
#include <iostream>
#include <Zut/ZxFS.h>
#include <Zut/ZxCvt.h>
#include <Zut/ZxFile.h>
#include <ReVN/RxEAGLS/Core/Script_Binary.h>
#include <ReVN/RxEAGLS/Core/CodeText_Formater.h>
#include <ReVN/RxEAGLS/Core/CodeText_TextEditor.h>


namespace RxEAGLS
{
    using namespace ZQF::ReVN::RxEAGLS;
}


[[maybe_unused]] static auto FormatCodeText(const std::string_view msCodeTextDir, const std::string_view msFormatedDir) -> void
{
    ZxFS::DirMake(msFormatedDir, false);

    RxEAGLS::CodeText::Formater formater;
    for (ZxFS::Walker walker{ msCodeTextDir }; walker.NextFile();)
    {
        formater.FromPath(walker.GetPath());
        formater.SaveFormated(std::string{ msFormatedDir }.append(walker.GetName()));
    }
}

[[maybe_unused]] static auto ExportCodeText(const std::string_view msScriptDir, const std::string_view msCodeTextDir) -> void
{
    ZxFS::DirMake(msCodeTextDir, false);

    ZxCvt cvt;
    RxEAGLS::Script::Binary parser;
    for (ZxFS::Walker walker{ msScriptDir }; walker.NextFile(); parser.Clear())
    {
        const auto code_text{ cvt.MBCSToUTF8(parser.Load(walker.GetPath()).GetCodeText(), 932) };
        const auto save_path{ std::string{ msCodeTextDir }.append(walker.GetName()).append(".txt") };
        ZxFile{ save_path, ZxFile::OpenMod::WriteForce }.Write(std::span{ code_text });
    }
}

[[maybe_unused]] static auto ExportJsonText(const std::string_view msCodeTextDir, const std::string_view msJsonDir) -> void
{
    ZxFS::DirMake(msJsonDir, false);

    RxEAGLS::CodeText::TextEditor text_editor;

    text_editor.SetNameMap(":NameSuffix", "和也");

    for (ZxFS::Walker walker{ msCodeTextDir }; walker.NextFile(); text_editor.Clear())
    {
        text_editor.ScanViaPath(walker.GetPath());
        text_editor.ExportViaJson(std::string{ msJsonDir }.append(walker.GetName()).append(".json"));
    }
}


auto main() -> int
{
    try
    {
        ::ExportCodeText("script/", "code_text/");
        ::FormatCodeText("code_text/", "formated/");
        ::ExportJsonText("formated/", "json/");
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
