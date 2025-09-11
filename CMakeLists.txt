cmake_minimum_required(VERSION 3.15)
project(cpp_json_class_gen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable folders in IDEs (VSCode, CLion, etc.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(cpp_json_class_gen_exec
    src/main.cpp
    src/generator.cpp
)

target_include_directories(cpp_json_class_gen_exec
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# target_link_libraries(cpp_json_class_gen_exec PRIVATE cpp_json_class_gen)

