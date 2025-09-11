cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_executable(GoogleApiEventExample src/main.cpp)
target_include_directories(GoogleApiEventExample PRIVATE include)

# Call the function from the top-level CMakeLists.txt to generate our headers.
# Pass the target name and a list of all the JSON configs it needs.
generate_class_from_json(GoogleApiEventExample
  CONFIGS
    meta_schemas/ApiEvent.json
    meta_schemas/ApiEventsList.json
)

# Link to any libraries needed at runtime (like nlohmann for parsing)
target_link_libraries(GoogleApiEventExample PRIVATE nlohmann_json::nlohmann_json)
