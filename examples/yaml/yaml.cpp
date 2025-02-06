
/*
__   __ _    __  __ _
\ \ / // \  |  \/  | |
 \ V // _ \ | |\/| | |
  | |/ ___ \| |  | | |___
  |_/_/   \_\_|  |_|_____|

YAML library
Author: Davide Stocco, 2025
*/

// Run this program with the following command from the root directory:
// ./build/ex_yaml examples/yaml/config.yaml examples/yaml/nested_config.yaml examples/yaml/list_config.yaml

// STL library headers
#include <iostream>
#include <fstream>

// YAML library header
#include <yaml-cpp/yaml.h>

// JSON library header
#include <nlohmann/json.hpp>

// Convert YAML to JSON
#include "yaml2json.hpp"

int main(int argc, char* argv[]) {

  // Check for command-line arguments
  if (argc < 2) {
    std::cerr << "Please provide a YAML files paths as an argument." << std::endl;
    return -1;
  }

  // Reading YAML from a File
  std::cout << "Reading YAML from a file..." << std::endl;

  // Load the YAML file
  YAML::Node config = YAML::LoadFile(argv[1]); // config.yaml

  // Access values from the YAML file
  std::string name = config["name"].as<std::string>();
  int age = config["age"].as<int>();

  // Display values from the YAML file
  std::cout << "Name: " << name << std::endl;
  std::cout << "Age: " << age << std::endl << std::endl;

  // Writing YAML to a File
  std::cout << "Writing YAML to a file..." << std::endl;

  // Create a new YAML Node
  YAML::Node new_node;
  new_node["name"] = "Alice";
  new_node["age"] = 25;
  new_node["is_student"] = true;

  // Write the YAML data to a file
  std::ofstream fout("examples/yaml/output.yaml");
  fout << new_node;

  std::cout << "YAML data written to 'output.yaml'" << std::endl << std::endl;

  // Working with Nested Structures
  std::cout << "Working with nested structures..." << std::endl;

  // Load a nested YAML file
  YAML::Node nested_config = YAML::LoadFile(argv[2]); // nested_config.yaml

  // Access nested structures
  std::string nested_name = nested_config["person"]["name"].as<std::string>();
  int nested_age = nested_config["person"]["age"].as<int>();

  // Display nested values
  std::cout << "Person Name: " << nested_name << std::endl;
  std::cout << "Person Age: " << nested_age << std::endl << std::endl;

  // Modifying Existing YAML
  std::cout << "Modifying existing YAML..." << std::endl;

  // Load an existing YAML file
  YAML::Node modify_config = YAML::LoadFile(argv[1]); // config.yaml

  // Modify values
  modify_config["age"] = 35; // change age to 35
  modify_config["name"] = "robert"; // change name to robert

  // Save the modified YAML back to file
  std::ofstream fout_modified("examples/yaml/modified_config.yaml");
  fout_modified << modify_config;

  std::cout << "YAML data modified and written to 'modified_config.yaml'" << std::endl << std::endl;

  // Accessing Lists and Arrays
  std::cout << "Reading a list from YAML..." << std::endl;

  // Load a YAML file containing a list
  YAML::Node list_config = YAML::LoadFile(argv[3]); // list_config.yaml

  // Iterate through the list and display each item
  for (const auto& item : list_config["items"]) {
    std::cout << "Item: " << item.as<std::string>() << std::endl;
  }
  std::cout << std::endl;

  // Exception Handling
  std::cout << "Handling exceptions..." << std::endl;

  try {
    // Attempt to load a non-existent file
    YAML::Node invalid_config = YAML::LoadFile("non_existent.yaml");
  } catch (const YAML::Exception& e) {
    std::cerr << "Error parsing YAML: " << e.what() << std::endl;
  }

  std::cout << std::endl;

  // Converting YAML to JSON
  std::cout << "Converting YAML to JSON..." << std::endl;

  // Load a YAML file
  YAML::Node yaml_data = YAML::LoadFile(argv[1]); // config.yaml

  // Convert YAML to JSON
  nlohmann::json json_data = yaml2json(yaml_data);

  // Display the JSON representation
  std::cout << "Converted JSON:" << std::endl << json_data.dump(4) << std::endl;

  return 0;
}
