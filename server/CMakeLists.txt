cmake_minimum_required(VERSION 3.10.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_executable(server mian.cpp)

