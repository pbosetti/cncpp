# `yaml-cpp` library

The `yaml-cpp` library is a popular `C++` library for parsing and emitting YAML (YAML Ain't Markup Language) documents. YAML is a human-readable data serialization format often used for configuration files, data exchange, and logging.

## Basic YAML Operations

### Reading YAML from a File

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  // load the YAML file
  YAML::Node config = YAML::LoadFile("config.yaml");

  // access the values from the YAML file
  std::string name = config["name"].as<std::string>();
  int age = config["age"].as<int>();

  std::cout << "Name: " << name << std::endl;
  std::cout << "Age: " << age << std::endl;

  return 0;
}
```

**Explanation**:

- The YAML file `config.yaml` is loaded into a `YAML::Node` object using `YAML::LoadFile()`.
- We access values by key using the square bracket operator (`config["key"]`) and convert them into appropriate types using `.as<type>()`.

**`config.yaml`**

```yaml
name: John Doe
age: 30
```

### Writing YAML to a File

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  // create a YAML Node
  YAML::Node node;
  node["name"] = "Alice";
  node["age"] = 25;
  node["is_student"] = true;

  // write the YAML data to a file
  std::ofstream fout("output.yaml");
  fout << node;

  std::cout << "YAML data written to output.yaml" << std::endl;

  return 0;
}
```

**Explanation**:

- A new `YAML::Node` object is created, and values are assigned to it using key-value pairs.
- The `YAML::Node` is written to a file by streaming it to an `std::ofstream`.

**`output.yaml`**

```yaml
name: Alice
age: 25
is_student: true
```

### Working with Nested Structures

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  YAML::Node config = YAML::LoadFile("nested_config.yaml");

  // accessing nested structures
  std::string name = config["person"]["name"].as<std::string>();
  int age = config["person"]["age"].as<int>();

  std::cout << "Name: " << name << std::endl;
  std::cout << "Age: " << age << std::endl;

  return 0;
}
```

**`nested_config.yaml`**

```yaml
person:
  name: Bob
  age: 40
```

## Modifying Existing YAML

You can modify the YAML structure directly by accessing nodes and setting new values.

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  // load existing YAML file
  YAML::Node config = YAML::LoadFile("config.yaml");

  // modify values
  config["age"] = 35; // change age to 35
  config["name"] = "Robert"; // change name to Robert

  // save modified YAML back to file
  std::ofstream fout("modified_config.yaml");
  fout << config;

  std::cout << "YAML data modified and written to modified_config.yaml" << std::endl;

  return 0;
}
```

**`modified_config.yaml`**

```yaml
name: Robert
age: 35
```

## Accessing Lists and Arrays

You can also handle lists or arrays in YAML.

###  **Reading a List**

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  YAML::Node config = YAML::LoadFile("list_example.yaml");

  // accessing a list (array)
  for (const auto& item : config["items"]) {
    std::cout << "Item: " << item.as<std::string>() << std::endl;
  }

  return 0;
}
```

**`list_example.yaml`**

```yaml
items:
  - apple
  - banana
  - cherry
```

### Output:

```
Item: apple
Item: banana
Item: cherry
```

## Exception Handling

`yaml-cpp` can throw exceptions if there are issues parsing the YAML file. It's a good practice to wrap the code in try-catch blocks.

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  try {
    YAML::Node config = YAML::LoadFile("non_existent.yaml");
  } catch (const YAML::Exception& e) {
    std::cerr << "Error parsing YAML: " << e.what() << std::endl;
  }

  return 0;
}
```

## Converting YAML to JSON

Sometimes, you may need to convert YAML to JSON for use in other applications. The `yaml-cpp` library doesn't directly support JSON conversion, but you can use `nlohmann/json` to convert the YAML data into JSON.

```cpp
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

int main() {
  YAML::Node config = YAML::LoadFile("config.yaml");

  // convert YAML to JSON
  nlohmann::json j = config.as<nlohmann::json>();

  // print the JSON object
  std::cout << j.dump(4) << std::endl;

  return 0;
}
```