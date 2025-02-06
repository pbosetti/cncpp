/*
     _ ____   ___  _   _
    | / ___| / _ \| \ | |
 _  | \___ \| | | |  \| |
| |_| |___) | |_| | |\  |
 \___/|____/ \___/|_| \_|

JSON library
Author: Davide Stocco, 2025
*/

// Run this program with the following command from the root directory:
// ./build/ex_yaml examples/json/example.json

// STL library headers
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>

// JSON library header
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {

  // Check for command-line arguments
  if (argc < 2) {
    std::cerr << "Please provide a YAML files paths as an argument." << std::endl;
    return -1;
  }

  // Reading JSON from a file
  std::ifstream file("example.json");
  json data = json::parse(file);
  std::cout << "Reading from JSON file: " << std::endl;
  std::cout << data << std::endl;

  // Creating JSON objects from JSON literals
  // Using raw string literals and json::parse
  json ex1 = json::parse(R"({
    "pi": 3.141,
    "happy": true
  })");

  // Using user-defined raw string literals
  using namespace nlohmann::literals;
  json ex2 = R"({
    "pi": 3.141,
    "happy": true
  })"_json;

  // Using initializer lists
  json ex3 = {
    {"happy", true},
    {"pi", 3.141},
  };

  std::cout << "Created JSON objects from literals:" << std::endl;
  std::cout << ex1 << std::endl;
  std::cout << ex2 << std::endl;
  std::cout << ex3 << std::endl;

  // JSON as first-class data type
  json j;
  j["pi"] = 3.141;
  j["happy"] = true;
  j["name"] = "Niels";
  j["nothing"] = nullptr;
  j["answer"]["everything"] = 42;
  j["list"] = { 1, 0, 2 };
  j["object"] = { {"currency", "usd"}, {"value", 42.99} };

  std::cout << "JSON with different data types:" << std::endl;
  std::cout << j << std::endl;

  // Serialization/Deserialization
  // Convert JSON object to string
  std::string serialized_string = j.dump();
  std::cout << "Serialized JSON string: " << serialized_string << std::endl;

  // Convert back from string
  json deserialized_json = json::parse(serialized_string);
  std::cout << "Deserialized JSON from string: " << deserialized_json << std::endl;

  // Pretty print JSON
  std::cout << "Pretty printed JSON: " << std::endl;
  std::cout << std::setw(4) << deserialized_json << std::endl;

  // Working with JSON containers (e.g., vector)
  std::vector<int> v = {1, 2, 3, 4};
  json j_vec(v); // Convert vector to JSON
  std::cout << "Converted vector to JSON: " << j_vec << std::endl;

  // Working with JSON Objects and Iteration
  json o;
  o["foo"] = 23;
  o["bar"] = false;
  o["baz"] = 3.141;

  // Iterating through JSON object
  std::cout << "Iterating through JSON object:" << std::endl;
  for (auto& [key, value] : o.items()) {
    std::cout << key << " : " << value << "\n";
  }

  // JSON Pointer
  json j_original = R"({
    "baz": ["one", "two", "three"],
    "foo": "bar"
  })"_json;
  std::cout << "Original JSON:" << std::endl;
  std::cout << j_original << std::endl;
  std::cout << "Accessed via JSON Pointer (/baz/1): " << j_original["/baz/1"_json_pointer] << std::endl;

  // JSON Patch
  json j_patch = R"([
    { "op": "replace", "path": "/baz", "value": "boo" },
    { "op": "add", "path": "/hello", "value": ["world"] },
    { "op": "remove", "path": "/foo"}
  ])"_json;

  json j_result = j_original.patch(j_patch);
  std::cout << "After applying patch:" << std::endl;
  std::cout << j_result << std::endl;

  // JSON Merge Patch
  json j_document = R"({
    "a": "b",
    "c": {
      "d": "e",
      "f": "g"
    }
  })"_json;

  json j_merge_patch = R"({
    "a": "z",
    "c": {
      "f": null
    }
  })"_json;

  j_document.merge_patch(j_merge_patch);
  std::cout << "After merge patch:" << std::endl;
  std::cout << j_document << std::endl;

  return 0;
}
