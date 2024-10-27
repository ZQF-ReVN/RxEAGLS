# RxEAGLS
cmake_minimum_required(VERSION 3.25)

# Project Name
project(ReVN_RxEAGLS)

# Export Symbols
if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Set Source File Path
list(APPEND ReVN_RxEAGLS_Core_Src_Files
    "src/ReVN/RxEAGLS/Core/Script_Binary.cpp"
    "src/ReVN/RxEAGLS/Core/CodeText_Lexical.cpp"
    "src/ReVN/RxEAGLS/Core/CodeText_Formater.cpp"
    "src/ReVN/RxEAGLS/Core/CodeText_TextEditor.cpp")

# Set Library
add_library(ReVN_RxEAGLS_Core ${ReVN_RxEAGLS_Core_Src_Files})
add_library(ReVN::RxEAGLS::Core ALIAS ReVN_RxEAGLS_Core)
target_include_directories(ReVN_RxEAGLS_Core PUBLIC "src")
target_compile_features(ReVN_RxEAGLS_Core PRIVATE cxx_std_23)
target_compile_options(ReVN_RxEAGLS_Core PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_link_libraries(ReVN_RxEAGLS_Core PUBLIC Zut::ZxMem)
target_link_libraries(ReVN_RxEAGLS_Core PRIVATE Zut::ZxFile Zut::ZxJson)

# Warning
if(MSVC)
    target_compile_options(ReVN_RxEAGLS_Core PRIVATE /W4)
else()
    target_compile_options(ReVN_RxEAGLS_Core PRIVATE -Wall -Wextra)
endif()

