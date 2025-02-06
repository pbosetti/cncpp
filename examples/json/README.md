
# `json` library

JSON (JavaScript Object Notation) is a lightweight data-interchange format. It is easy for humans to read and write. There are many libraries available to handle JSON in `C++`, with the most popular one being [`nlohmann/json`](https://github.com/nlohmann/json) (also known as JSON for Modern `C++`). This library is a single header file that provides a simple way to work with JSON data.

## Read JSON from a file

The `json` class provides an API for manipulating a JSON value. To create a `json` object by reading a JSON file:

```cpp
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json; // a C++ alias for nlohmann::json

// ...

std::ifstream f("example.json");
json data = json::parse(f);
```

## Creating `json` objects from JSON literals

Assume you want to create hard-code this literal JSON value in a file, as a `json` object:

```json
{
  "pi": 3.141,
  "happy": true
}
```

There are various options:

```cpp
// using (raw) string literals and json::parse
json ex1 = json::parse(R"(
  {
  "pi": 3.141,
  "happy": true
  }
)");

// using user-defined (raw) string literals
using namespace nlohmann::literals;
json ex2 = R"(
  {
  "pi": 3.141,
  "happy": true
  }
)"_json;

// using initializer lists
json ex3 = {
  {"happy", true},
  {"pi", 3.141},
};
```

## JSON as first-class data type

Here are some examples to give you an idea how to use the class.

Assume you want to create the JSON object

```json
{
  "pi": 3.141,
  "happy": true,
  "name": "Niels",
  "nothing": null,
  "answer": {
  "everything": 42
  },
  "list": [1, 0, 2],
  "object": {
  "currency": "usd",
  "value": 42.99
  }
}
```

With this library, you could write:

```cpp
// create an empty structure (null)
json j;

// add a number that is stored as double (note the implicit conversion of j to an object)
j["pi"] = 3.141;

// add a Boolean that is stored as bool
j["happy"] = true;

// add a string that is stored as std::string
j["name"] = "Niels";

// add another null object by passing nullptr
j["nothing"] = nullptr;

// add an object inside the object
j["answer"]["everything"] = 42;

// add an array that is stored as std::vector (using an initializer list)
j["list"] = { 1, 0, 2 };

// add another object (using an initializer list of pairs)
j["object"] = { {"currency", "usd"}, {"value", 42.99} };

// instead, you could also write (which looks very similar to the JSON above)
json j2 = {
  {"pi", 3.141},
  {"happy", true},
  {"name", "Niels"},
  {"nothing", nullptr},
  {"answer", {
  {"everything", 42}
  }},
  {"list", {1, 0, 2}},
  {"object", {
  {"currency", "usd"},
  {"value", 42.99}
  }}
};
```

Note that in all these cases, you *never* need to "tell" the compiler which JSON value type you want to use. If you want to be explicit or express some edge cases, the functions [`json::array()`](https://json.nlohmann.me/api/basic_json/array/) and [`json::object()`](https://json.nlohmann.me/api/basic_json/object/) will help:

```cpp
// a way to express the empty array []
json empty_array_explicit = json::array();

// ways to express the empty object {}
json empty_object_implicit = json({});
json empty_object_explicit = json::object();

// a way to express an _array_ of key/value pairs [["currency", "usd"], ["value", 42.99]]
json array_not_object = json::array({ {"currency", "usd"}, {"value", 42.99} });
```

## Serialization/deserialization

### To/from strings

You can create a JSON value (deserialization) by appending `_json` to a string literal:

```cpp
// create object from string literal
json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;

// or even nicer with a raw string literal
auto j2 = R"(
  {
  "happy": true,
  "pi": 3.141
  }
)"_json;
```

Note that without appending the `_json` suffix, the passed string literal is not parsed, but just used as JSON string value. That is, `json j = "{ \"happy\": true, \"pi\": 3.141 }"` would just store the string `"{ "happy": true, "pi": 3.141 }"` rather than parsing the actual object.

The string literal should be brought into scope with `using namespace nlohmann::literals;` (see [`json::parse()`](https://json.nlohmann.me/api/operator_literal_json/)).

The above example can also be expressed explicitly using [`json::parse()`](https://json.nlohmann.me/api/basic_json/parse/):

```cpp
// parse explicitly
auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");
```

You can also get a string representation of a JSON value (serialize):

```cpp
// explicit conversion to string
std::string s = j.dump(); // {"happy": true,"pi": 3.141}

// serialization with pretty printing
// pass in the amount of spaces to indent
std::cout << j.dump(4) << std::endl;
// {
//    "happy": true,
//    "pi": 3.141
// }
```

Note the difference between serialization and assignment:

```cpp
// store a string in a JSON value
json j_string = "this is a string";

// retrieve the string value
auto cpp_string = j_string.template get<std::string>();
// retrieve the string value (alternative when a variable already exists)
std::string cpp_string2;
j_string.get_to(cpp_string2);

// retrieve the serialized value (explicit JSON serialization)
std::string serialized_string = j_string.dump();

// output of original string
std::cout << cpp_string << " == " << cpp_string2 << " == " << j_string.template get<std::string>() << '\n';
// output of serialized value
std::cout << j_string << " == " << serialized_string << std::endl;
```

[`.dump()`](https://json.nlohmann.me/api/basic_json/dump/) returns the originally stored string value.

Note the library only supports UTF-8. When you store strings with different encodings in the library, calling [`dump()`](https://json.nlohmann.me/api/basic_json/dump/) may throw an exception unless `json::error_handler_t::replace` or `json::error_handler_t::ignore` are used as error handlers.

### To/from streams (*e.g.*, files, string streams)

You can also use streams to serialize and deserialize:

```cpp
// deserialize from standard input
json j;
std::cin >> j;

// serialize to standard output
std::cout << j;

// the setw manipulator was overloaded to set the indentation for pretty printing
std::cout << std::setw(4) << j << std::endl;
```

These operators work for any subclasses of `std::istream` or `std::ostream`. Here is the same example with files:

```cpp
// read a JSON file
std::ifstream i("file.json");
json j;
i >> j;

// write prettified JSON to another file
std::ofstream o("pretty.json");
o << std::setw(4) << j << std::endl;
```

Please note that setting the exception bit for `failbit` is inappropriate for this use case. It will result in program termination due to the `noexcept` specifier in use.

### Read from iterator range

You can also parse JSON from an iterator range; that is, from any container accessible by iterators whose `value_type` is an integral type of 1, 2 or 4 bytes, which will be interpreted as UTF-8, UTF-16 and UTF-32 respectively. For instance, a `std::vector<std::uint8_t>`, or a `std::list<std::uint16_t>`:

```cpp
std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
json j = json::parse(v.begin(), v.end());
```

You may leave the iterators for the range [begin, end):

```cpp
std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
json j = json::parse(v);
```

### Custom data source

Since the parse function accepts arbitrary iterator ranges, you can provide your own data sources by implementing the `LegacyInputIterator` concept.

```cpp
struct MyContainer {
  void advance();
  const char & get_current();
};

struct MyIterator {
  using difference_type = std::ptrdiff_t;
  using value_type = char;
  using pointer = const char *;
  using reference = const char &;
  using iterator_category = std::input_iterator_tag;

  MyIterator & operator++() {
    target->advance();
    return *this;
  }

  bool operator!=(const MyIterator & rhs) const {
    return rhs.target != target;
  }

  reference operator*() const {
    return target->get_current();
  }

  MyContainer* target = nullptr;
};

MyIterator begin(MyContainer & tgt) {
  return MyIterator{& tgt};
}

MyIterator end(const MyContainer &) {
  return {};
}

void foo() {
  MyContainer c;
  json j = json::parse(c);
}
```

### SAX interface

The library uses a SAX-like interface with the following functions:

```cpp
// called when null is parsed
bool null();

// called when a boolean is parsed; value is passed
bool boolean(bool val);

// called when a signed or unsigned integer number is parsed; value is passed
bool number_integer(number_integer_t val);
bool number_unsigned(number_unsigned_t val);

// called when a floating-point number is parsed; value and original string is passed
bool number_float(number_float_t val, const string_t & s);

// called when a string is parsed; value is passed and can be safely moved away
bool string(string_t & val);
// called when a binary value is parsed; value is passed and can be safely moved away
bool binary(binary_t & val);

// called when an object or array begins or ends, resp. The number of elements is passed (or -1 if not known)
bool start_object(std::size_t elements);
bool end_object();
bool start_array(std::size_t elements);
bool end_array();
// called when an object key is parsed; value is passed and can be safely moved away
bool key(string_t & val);

// called when a parse error occurs; byte position, the last token, and an exception is passed
bool parse_error(std::size_t position, const std::string & last_token, const detail::exception & ex);
```

The return value of each function determines whether parsing should proceed.

To implement your own SAX handler, proceed as follows:

1. Implement the SAX interface in a class. You can use class `nlohmann::json_sax<json>` as base class, but you can also use any class where the functions described above are implemented and public.
2. Create an object of your SAX interface class, *e.g.*, `my_sax`.
3. Call `bool json::sax_parse(input, & my_sax)`; where the first parameter can be any input like a string or an input stream and the second parameter is a pointer to your SAX interface.

Note the `sax_parse` function only returns a `bool` indicating the result of the last executed SAX event. It does not return a  `json` value - it is up to you to decide what to do with the SAX events. Furthermore, no exceptions are thrown in case of a parse error - it is up to you what to do with the exception object passed to your `parse_error` implementation. Internally, the SAX interface is used for the DOM parser (class `json_sax_dom_parser`) as well as the acceptor (`json_sax_acceptor`), see file [`json_sax.hpp`](https://github.com/nlohmann/json/blob/develop/include/nlohmann/detail/input/json_sax.hpp).

## STL-like access

We designed the JSON class to behave just like an STL container. In fact, it satisfies the [**ReversibleContainer**](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer) requirement.

```cpp
// create an array using push_back
json j;
j.push_back("foo");
j.push_back(1);
j.push_back(true);

// also use emplace_back
j.emplace_back(1.78);

// iterate the array
for (json::iterator it = j.begin(); it != j.end(); ++it) {
  std::cout << *it << '\n';
}

// range-based for
for (auto & element : j) {
  std::cout << element << '\n';
}

// getter/setter
const auto tmp = j[0].template get<std::string>();
j[1] = 42;
bool foo = j.at(2);

// comparison
j == R"(["foo", 1, true, 1.78])"_json; // true

// other stuff
j.size(); // 4 entries
j.empty(); // false
j.type(); // json::value_t::array
j.clear(); // the array is empty again

// convenience type checkers
j.is_null();
j.is_boolean();
j.is_number();
j.is_object();
j.is_array();
j.is_string();

// create an object
json o;
o["foo"] = 23;
o["bar"] = false;
o["baz"] = 3.141;

// also use emplace
o.emplace("weather", "sunny");

// special iterator member functions for objects
for (json::iterator it = o.begin(); it != o.end(); ++it) {
  std::cout << it.key() << " : " << it.value() << "\n";
}

// the same code as range for
for (auto & el : o.items()) {
  std::cout << el.key() << " : " << el.value() << "\n";
}

// even easier with structured bindings (C++17)
for (auto & [key, value] : o.items()) {
  std::cout << key << " : " << value << "\n";
}

// find an entry
if (o.contains("foo")) {
  // there is an entry with key "foo"
}

// or via find and an iterator
if (o.find("foo") != o.end()) {
  // there is an entry with key "foo"
}

// or simpler using count()
int foo_present = o.count("foo"); // 1
int fob_present = o.count("fob"); // 0

// delete an entry
o.erase("foo");
```

## Conversion from STL containers

Any sequence container (`std::array`, `std::vector`, `std::deque`, `std::forward_list`, `std::list`) whose values can be used to construct JSON values (*e.g.*, integers, floating point numbers, Booleans, string types, or again STL containers described in this section) can be used to create a JSON array. The same holds for similar associative containers (`std::set`, `std::multiset`, `std::unordered_set`, `std::unordered_multiset`), but in these cases the order of the elements of the array depends on how the elements are ordered in the respective STL container.

```cpp
std::vector<int> c_vector {1, 2, 3, 4};
json j_vec(c_vector);
// [1, 2, 3, 4]

std::deque<double> c_deque {1.2, 2.3, 3.4, 5.6};
json j_deque(c_deque);
// [1.2, 2.3, 3.4, 5.6]

std::list<bool> c_list {true, true, false, true};
json j_list(c_list);
// [true, true, false, true]

std::forward_list<int64_t> c_flist {12345678909876, 23456789098765, 34567890987654, 45678909876543};
json j_flist(c_flist);
// [12345678909876, 23456789098765, 34567890987654, 45678909876543]

std::array<unsigned long, 4> c_array {{1, 2, 3, 4}};
json j_array(c_array);
// [1, 2, 3, 4]

std::set<std::string> c_set {"one", "two", "three", "four", "one"};
json j_set(c_set); // only one entry for "one" is used
// ["four", "one", "three", "two"]

std::unordered_set<std::string> c_uset {"one", "two", "three", "four", "one"};
json j_uset(c_uset); // only one entry for "one" is used
// maybe ["two", "three", "four", "one"]

std::multiset<std::string> c_mset {"one", "two", "one", "four"};
json j_mset(c_mset); // both entries for "one" are used
// maybe ["one", "two", "one", "four"]

std::unordered_multiset<std::string> c_umset {"one", "two", "one", "four"};
json j_umset(c_umset); // both entries for "one" are used
// maybe ["one", "two", "one", "four"]
```

Likewise, any associative key-value containers (`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`) whose keys can construct an `std::string` and whose values can be used to construct JSON values (see examples above) can be used to create a JSON object. Note that in case of multimaps only one key is used in the JSON object and the value depends on the internal order of the STL container.

```cpp
std::map<std::string, int> c_map { {"one", 1}, {"two", 2}, {"three", 3} };
json j_map(c_map);
// {"one": 1, "three": 3, "two": 2 }

std::unordered_map<const char*, double> c_umap { {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
json j_umap(c_umap);
// {"one": 1.2, "two": 2.3, "three": 3.4}

std::multimap<std::string, bool> c_mmap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
json j_mmap(c_mmap); // only one entry for key "three" is used
// maybe {"one": true, "two": true, "three": true}

std::unordered_multimap<std::string, bool> c_ummap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
json j_ummap(c_ummap); // only one entry for key "three" is used
// maybe {"one": true, "two": true, "three": true}
```

## JSON Pointer and JSON Patch

The library supports **JSON Pointer** ([RFC 6901](https://tools.ietf.org/html/rfc6901)) as alternative means to address structured values. On top of this, **JSON Patch** ([RFC 6902](https://tools.ietf.org/html/rfc6902)) allows describing differences between two JSON values - effectively allowing patch and diff operations known from Unix.

```cpp
// a JSON value
json j_original = R"({
  "baz": ["one", "two", "three"],
  "foo": "bar"
})"_json;

// access members with a JSON pointer (RFC 6901)
j_original["/baz/1"_json_pointer];
// "two"

// a JSON patch (RFC 6902)
json j_patch = R"([
  { "op": "replace", "path": "/baz", "value": "boo" },
  { "op": "add", "path": "/hello", "value": ["world"] },
  { "op": "remove", "path": "/foo"}
])"_json;

// apply the patch
json j_result = j_original.patch(j_patch);
// {
//   "baz": "boo",
//   "hello": ["world"]
// }

// calculate a JSON patch from two JSON values
json::diff(j_result, j_original);
// [
//   { "op": " replace", "path": "/baz", "value": ["one", "two", "three"] },
//   { "op": "remove", "path": "/hello" },
//   { "op": "add", "path": "/foo", "value": "bar" }
// ]
```

## JSON Merge Patch

The library supports **JSON Merge Patch** ([RFC 7386](https://tools.ietf.org/html/rfc7386)) as a patch format. Instead of using JSON Pointer (see above) to specify values to be manipulated, it describes the changes using a syntax that closely mimics the document being modified.

```cpp
// a JSON value
json j_document = R"({
  "a": "b",
  "c": {
  "d": "e",
  "f": "g"
  }
})"_json;

// a patch
json j_patch = R"({
  "a": "z",
  "c": {
  "f": null
  }
})"_json;

// apply the patch
j_document.merge_patch(j_patch);
// {
//  "a": "z",
//  "c": {
//   "d": "e"
//  }
// }
```

## Implicit conversions

Supported types can be implicitly converted to JSON values.

It is recommended to **NOT USE** implicit conversions **FROM** a JSON value. You can find more details about this recommendation [here](https://www.github.com/nlohmann/json/issues/958). You can switch off implicit conversions by defining `JSON_USE_IMPLICIT_CONVERSIONS` to `0` before including the `json.hpp` header. When using CMake, you can also achieve this by setting the option `JSON_ImplicitConversions` to `OFF`.

```cpp
// strings
std::string s1 = "Hello, world!";
json js = s1;
auto s2 = js.template get<std::string>();
// NOT RECOMMENDED
std::string s3 = js;
std::string s4;
s4 = js;

// Booleans
bool b1 = true;
json jb = b1;
auto b2 = jb.template get<bool>();
// NOT RECOMMENDED
bool b3 = jb;
bool b4;
b4 = jb;

// numbers
int i = 42;
json jn = i;
auto f = jn.template get<double>();
// NOT RECOMMENDED
double f2 = jb;
double f3;
f3 = jb;

// etc.
```

Note that `char` types are not automatically converted to JSON strings, but to integer numbers. A conversion to a string must be specified explicitly:

```cpp
char ch = 'A'; // ASCII value 65
json j_default = ch; // stores integer number 65
json j_string = std::string(1, ch); // stores string "A"
```

## Arbitrary types conversions

Every type can be serialized in JSON, not just STL containers and scalar types. Usually, you would do something along those lines:

```cpp
namespace ns {
  // a simple struct to model a person
  struct person {
    std::string name;
    std::string address;
    int age;
  };
}

ns::person p = {"ned flanders", "via sommarive 9", 60};

// convert to JSON: copy each value into the JSON object
json j;
j["name"] = p.name;
j["address"] = p.address;
j["age"] = p.age;

// ...

// convert from JSON: copy each value from the JSON object
ns::person p {
  j["name"].template get<std::string>(),
  j["address"].template get<std::string>(),
  j["age"].template get<int>()
};
```

It works, but that's quite a lot of boilerplate... Fortunately, there's a better way:

```cpp
// create a person
ns::person p {"ned flanders", "via sommarive 9", 60};

// conversion: person -> json
json j = p;

std::cout << j << std::endl;
// {"address": "via sommarive 9", "age": 60, "name": "ned flanders"}

// conversion: json -> person
auto p2 = j.template get<ns::person>();

// that's it
assert(p == p2);
```

### Basic usage

To make this work with one of your types, you only need to provide two functions:

```cpp
using json = nlohmann::json;

namespace ns {
  void to_json(json & j, const person & p) {
    j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
  }

  void from_json(const json & j, person & p) {
    j.at("name").get_to(p.name);
    j.at("address").get_to(p.address);
    j.at("age").get_to(p.age);
  }
} // namespace ns
```

That's all! When calling the `json` constructor with your type, your custom `to_json` method will be automatically called. Likewise, when calling `template get<your_type>()` or `get_to(your_type&)`, the `from_json` method will be called.

Some important things:

- Those methods **must** be in your type's namespace (which can be the global namespace), or the library will not be able to locate them (in this example, they are in namespace `ns`, where `person` is defined).
- Those methods **must** be available (*e.g.*, proper headers must be included) everywhere you use these conversions. Look at [issue 1108](https://github.com/nlohmann/json/issues/1108) for errors that may occur otherwise.
- When using `template get<your_type>()`, `your_type` **must** be [DefaultConstructible](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible). (There is a way to bypass this requirement described later.)
- In function `from_json`, use function [`at()`](https://json.nlohmann.me/api/basic_json/at/) to access the object values rather than `operator[]`. In case a key does not exist, `at` throws an exception that you can handle, whereas `operator[]` exhibits undefined behavior.
- You do not need to add serializers or deserializers for STL types like `std::vector`: the library already implements these.
