#include<iostream> // to print messages on the terminal
#include<fstream> // to write and read files

// Include YAML-CPP library
#include<yaml-cpp/yaml.h>

// Include JSON library
#include<nlohmann/json.hpp>

using nlohmann::json;

// Main function
int main(int argc, char * argv[]) // to use variable inputs
{
  // Read the YAML file (bad way)
  // YAML::Node config = YAML::LoadFile("./../examples/lesson6/config.yaml");

  // Right way:
  // Execute the lesson6 executable as (from the build folder):
  // ./examples/lesson6 ./../examples/lesson6/config.yaml
  // argv[0] = "./examples/lesson6"
  // argv[1] = "./../examples/lesson6/config.yaml"
  if (argc != 3) {
    std::cerr << "Please provide a yaml and a json file!\n";
    return -1; // Return a value != 0 if there is an error
  }
  //YAML::Node config = YAML::LoadFile(argv[1]);

  // Try-catch
  YAML::Node config;
  try {
    // Here, we try lo load the file
    config = YAML::LoadFile(argv[1]);
  } catch (const YAML::Exception & e) {
    // If the commend in the try scope fail
    std::cerr << "Error parsing YAML: " << e.what() << std::endl;
  }

  // Access the values inside the YAML object
  { // Curly brakets indicate a 'scope' and variable exists only within the scoper they are declared
    std::string name = config["name"].as<std::string>(); // Retrive the value of the 'name' field and cast it to an std::string
    int age = config["age"].as<int>(); // Retrive the value of the 'age' field and cast it to an integer
    std::cout
      << "Name: " << name << std::endl
      << "Age:  " << age << std::endl;
  }

  // Nested fields
  {
    std::string name = config["person"]["name"].as<std::string>();
    int age = config["person"]["age"].as<int>();
    std::cout
      << "Name: " << name << std::endl
      << "Age:  " << age << std::endl;
  }

  // Write a YAML file
  YAML::Node new_node;
  new_node["name"]       = "Alice";
  new_node["age"]        = 40;
  new_node["is_student"] = false;

  // Nested fields
  new_node["nested_field"]["name"]       = "Bob";
  new_node["nested_field"]["age"]        = 20;
  new_node["nested_field"]["is_student"] = true;

  // ofstream : output file stream
  {std::ofstream fout("output.yaml");
  fout << new_node;
  fout.close();}

  // Iterate through the fields of a YAML file
  for (const auto & item : config["items"]) {
    std::cout << "Item: " << item.as<std::string>() << std::endl;
  }

  // Modify a YAML
  YAML::Node mod_config = YAML::LoadFile(argv[1]);
  mod_config["person"]["name"] = "Bla";
  {std::ofstream fout("config_mod.yaml");
  fout << mod_config;
  fout.close();}

  // JSON library
  std::ifstream json_fstr(argv[2]);
  json data;
  try {
    // Here, we try lo load the file
    data = json::parse(json_fstr);
  } catch (const std::exception& e) {
    // If the commend in the try scope fail
    std::cerr << "Error parsing JSON!" << std::endl;
  }

  // Access the values of the json
  std::cout << "data[""name""] = " << data["name"] << std::endl;
  std::cout << "data[""age""] = " << data["age"] << std::endl;

  // Iterate through the JSON objects
  for (auto const & [key, value] : data.items()) {
    std::cout << key << " : " << value << std::endl;
  }

  // Creating JSON objects
  // Using literals (std::string)
  using namespace nlohmann::literals;
  json ex1 = R"({
    "number" : 2,
    "happy" : true
  })"_json;
  std::cout << "Using literals\n" << ex1 << std::endl;

  json ex2 = json::parse(R"({
    "number" : 2,
    "happy" : true
  })");
  std::cout << "Using literals (parse)\n" << ex2 << std::endl;

  json ex3 = {
    {"number", 2},
    {"happy", true}
  };
  std::cout << "Using literals (initialization list)\n" << ex3 << std::endl;

  // Using the json class
  json ex4;
  ex4["number"] = 2;
  ex4["happy"] = true;
  std::cout << "Using json class\n" << ex4 << std::endl;

  // Serialization of JSON string (writing a file)
  std::ofstream json_out("output.json");
  json_out << ex4.dump();
  json_out.close(); // close the stream (file)

  // Vector of integers
  std::vector<int> v = {1, 2, 3, 4};
  json json_v(v);
  std::cout << "Json_v = \n" << json_v << std::endl; 

  return 0;
}
