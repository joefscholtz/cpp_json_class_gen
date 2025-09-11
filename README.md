# cpp_json_class_gen

A powerful, compile-time code generation tool written in pure C++ that creates C++ classes and structs from simple JSON template files. This project provides a robust, workflow for build-time code generation, seamlessly integrated with CMake.

## Features

- Compile-Time Generation: Generates C++ header files (.hpp) during the build process. Your final application has no dependency on the generator or the JSON templates.
- Rich Type Definition: Define more than just member variables. The JSON schema supports:
  - class or struct generation.
  - Inheritance from multiple base classes.
  - Custom method declarations.
  - Inclusion of required headers.
- Safe Deserialization: Generated from_json functions use safe access methods (.value()) to prevent crashes when optional keys are missing from the input JSON.
- Automatic Serialization: Automatically generates to_json and from_json functions for full compatibility with the nlohmann::json library.
- Clean CMake Integration: Provides a simple CMake function (generate_class_from_json) that makes it trivial to add code generation to any target.

## Getting Started

### Prerequisites

- A C++20 compatible compiler (GCC, Clang, MSVC).
- CMake (version 3.15 or higher).

### Build Instructions

To build the generator and run the included examples, clone the repository and use the following commands:
Bash

```bash
# Clone the repository

git clone https://github.com/your-username/cpp_json_class_gen.git
cd cpp_json_class_gen

# Configure the project using a CMake preset

cmake --preset default

# Build the generator and all examples

cmake --build --preset default --parallel

# Run the example

./build/examples/GoogleApiEvent/GoogleApiEventExample

```

### Usage

Integrating the code generator into your project involves two simple steps:

#### 1. Create Your JSON Schemas

For each C++ class you want to generate, create a .json file that defines its structure. For example, to create an ApiEventsList class that contains a vector of ApiEvent objects, you would create two schemas.

config/ApiEvent.json:

```json
{
  "kind": "class",
  "struct_name": "ApiEvent",
  "cpp_namespace": "gcal",
  "is_final": true,
  "base_classes": ["events::BaseEvent"],
  "include_headers": ["string", "vector", "BaseEvent.hpp"],
  "methods": ["bool isAllDayEvent() const"],
  "schema": {
    "iCalUID": "std::string",
    "summary": "std::string",
    "start_time": "std::string",
    "end_time": "std::string"
  }
}
```

config/ApiEventsList.json:

```bash
{
"kind": "struct",
"struct_name": "ApiEventsList",
"cpp_namespace": "gcal",
"include_headers": ["string", "vector", "ApiEvent.hpp"],
"schema": {
"kind": "std::string",
"etag": "std::string",
"summary": "std::string",
"items": "std::vector<gcal::ApiEvent>"
}
}
```

#### 2. Integrate with CMake

In your project's CMakeLists.txt, simply call the generate_class_from_json function provided by the tool's top-level CMakeLists.txt.

examples/GoogleApiEvent/CMakeLists.txt

```CMake
# Define your application's executable

add_executable(GoogleApiEventExample src/main.cpp)

# Add your project's own include directories

target_include_directories(GoogleApiEventExample PRIVATE include)

# Call the generator function for your target, listing all required schemas

generate_class_from_json(GoogleApiEventExample
CONFIGS
config/ApiEvent.json
config/ApiEventsList.json
)

# Link to any necessary libraries

target_link_libraries(GoogleApiEventExample PRIVATE nlohmann_json::nlohmann_json)
```

#### 3. Use the Generated Code

You can now include the generated headers in your C++ code and use the types as if they were hand-written.

src/main.cpp:

```C++
#include <iostream>

// Headers are generated into the build directory
#include "ApiEvent.hpp"
#include "ApiEventsList.hpp"
#include "nlohmann/json.hpp"

int main() {
std::string json_data = R"({ ... })";

    try {
        nlohmann::json j = nlohmann::json::parse(json_data);

        // Use the generated type directly!
        gcal::ApiEventsList event_list = j.get<gcal::ApiEventsList>();

        std::cout << "Summary: " << event_list.summary << std::endl;
        std::cout << "Items: " << event_list.items.size() << std::endl;

    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON Error: " << e.what() << std::endl;
    }
    return 0;

}
```

## JSON Schema Reference

The following keys are supported in the JSON template files:
|Key |Type |Required? |Description |
|----|-----|----------|------------|
|kind |string |Optional |Specifies class or struct. Defaults to struct.|
|struct_name string Required The name of the C++ class/struct to be generated.|
|cpp_namespace string Required The C++ namespace to enclose the generated type in.|
|is_final boolean Optional If true, adds the final specifier to the class.|
|base_classes array of strings Optional A list of classes to inherit from (e.g., "MyBaseClass").|
|include_headers array of strings Required A list of headers to #include in the generated file.|
|methods array of strings Optional A list of method declarations to add to the class.|
|schema object Required A key-value map where the key is the member variable name and the value is its C++ type as a |string.|
