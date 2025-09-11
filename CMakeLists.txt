cmake_minimum_required(VERSION 3.15)
project(cpp_json_class_gen LANGUAGES CXX)

find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable folders in IDEs (VSCode, CLion, etc.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(cpp_json_class_gen_exec
    src/generator.cpp
)

target_include_directories(cpp_json_class_gen_exec
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cpp_json_class_gen_exec PRIVATE nlohmann_json::nlohmann_json)

function(generate_class_from_json TARGET_NAME)
  cmake_parse_arguments(ARG "" "" "CONFIGS" ${ARGN})

  set(GENERATED_HEADERS "")
  foreach(JSON_FILE ${ARG_CONFIGS})
    get_filename_component(BASENAME ${JSON_FILE} NAME_WE)
    set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.hpp)
    list(APPEND GENERATED_HEADERS ${GENERATED_HEADER})

    add_custom_command(
      OUTPUT ${GENERATED_HEADER}
      COMMAND cpp_json_class_gen_exec ${CMAKE_CURRENT_SOURCE_DIR}/${JSON_FILE} ${GENERATED_HEADER}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${JSON_FILE} cpp_json_class_gen_exec
    )
  endforeach()

  # Create a single custom target that depends on ALL generated headers for this group
  set(CODEGEN_TARGET "generate_${TARGET_NAME}")
  add_custom_target(${CODEGEN_TARGET} DEPENDS ${GENERATED_HEADERS})

  # Make the main target depend on the code generation being complete
  add_dependencies(${TARGET_NAME} ${CODEGEN_TARGET})

  # Automatically add the binary directory to the target's include path
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

add_subdirectory(examples)

