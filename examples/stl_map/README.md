# STL map

In `C++`, a **map** is an associative container that stores elements in **key-value pairs**. It provides **fast lookups, insertions, and deletions** while keeping the keys in a **sorted order**. Internally, `std::map` is implemented as a **self-balancing binary search tree (usually a Red-Black Tree)**, ensuring O(log n) complexity for operations like insertion, deletion, and lookup.

## Declaring a map

A map is declared using the `std::map` template:

```cpp
std::map<key_type, value_type> map_name;
```

### Examples

```cpp
std::map<int, std::string> student;

// initializing a map using initializer list
std::map<int, std::string> students = {
    {101, "Alice"},
    {102, "Bob"},
    {103, "Charlie"}
};

// initializing an empty map
std::map<std::string, int> wordCount;
```

## Accessing elements

Elements in a `std::map` can be accessed using the key.

### Using `[]` operator

```cpp
std::map<int, std::string> students;
students[101] = "Alice";
students[102] = "Bob";
std::cout << students[101]; // outputs "Alice"
```

- If the key **does not exist**, the `[]` operator **creates a new entry** with a default value.
- This is useful for inserting elements but **can lead to unintended insertions**.

### Using `at()` method (safer option)

```cpp
std::cout << students.at(101); // outputs "Alice"
```

- Unlike `[]`, `at()` **throws an exception** if the key does not exist.

### Checking if a key exists

```cpp
if (students.find(105) != students.end()) {
    std::cout << "Key exists!";
} else {
    std::cout << "Key does not exist!";
}
```

- `find()` returns an iterator to the key if found; otherwise, it returns `map.end()`.

## Iterating through a map

### Using a `for` loop with iterators

```cpp
for (auto it = students.begin(); it != students.end(); ++it) {
    std::cout << "ID: " << it->first << ", Name: " << it->second << std::endl;
}
```

### Using a range-based for loop

```cpp
for (const auto& [id, name] : students) {
    std::cout << "ID: " << id << ", Name: " << name << std::endl;
}
```

- Since `C++`17, **structured bindings** allow destructuring key-value pairs in a map.

### Lower and upper bounds

```cpp
auto it = students.lower_bound(102); // returns iterator to key 102
auto it = students.upper_bound(102); // returns iterator to key 103
```

- `lower_bound()` returns an iterator to the **first element** with a key **not less than** the given key.
- `upper_bound()` returns an iterator to the **first element** with a key **greater than** the given key.
If the key does not exist, both functions return an iterator to the next element.

## Adding and modifying elements

### Using `[]` operator (creates or modifies an entry)

```cpp
students[104] = "David"; // inserts new pair {104, "David"}
students[102] = "Bob Jr."; // modifies value for key 102
```

### Using `insert()` method

```cpp
students.insert({105, "Emma"});
```

- `insert()` **does not overwrite existing values**.
- It returns a `std::pair<iterator, bool>`, where `bool` indicates success or failure.

```cpp
auto result = students.insert({102, "Eve"});
if (!result.second) {
    std::cout << "Insertion failed: Key already exists";
}
```

## Removing elements

### Using `erase()`

```cpp
students.erase(101); // removes key 101 if it exists
```

- If the key **does not exist**, `erase()` does nothing.
- You can also erase using an iterator:

```cpp
students.erase(students.begin()); // removes the first element
```

### Using `clear()`

```cpp
students.clear(); // removes all elements
```

## Other useful operations

### Checking size

```cpp
std::cout << students.size(); // returns the number of elements in the map
```

### Checking if map is empty

```cpp
if (students.empty()) {
    std::cout << "Map is empty!";
}
```

### Swapping two maps

```cpp
std::map<int, std::string> map1 = {{1, "A"}, {2, "B"}};
std::map<int, std::string> map2 = {{3, "X"}, {4, "Y"}};

map1.swap(map2); // map1 now contains {3, "X"}, {4, "Y"}
```

## Map properties

### Keys are unique

- If you try to insert a duplicate key, the existing value **will not** be replaced (when using `insert()`).
- Use `operator[]` or `at()` to update values.

### Keys are sorted

- Elements in a `std::map` are **always sorted** by key.
- You can use **custom comparator functions** to change sorting order.

### Uses balanced BST (O(log n) operations)

- Insertion, deletion, and lookup take **O(log n)** time.

## Custom comparator for sorting

By default, `std::map` sorts elements in **ascending order**. You can define a custom comparator:

```cpp
struct Descending {
    bool operator()(int a, int b) const {
        return a > b; // sort in descending order
    }
};

std::map<int, std::string, Descending> rev_map;
```

## Unordered map (faster alternative)

If **ordering of keys is not required**, `std::unordered_map` is a better choice. It uses a **hash table**, providing O(1) average time complexity for insertions, deletions, and lookups.

```cpp
std::unordered_map<int, std::string> u_map;
u_map[1] = "Alice";
```

## Summary

| Feature | `std::map` (Ordered) | `std::unordered_map` (Unordered) |
|---------|----------------------|----------------------------------|
| Data structure | Balanced BST (RB-Tree) | Hash Table |
| Key ordering | Sorted (ascending by default) | No ordering |
| Lookup time | O(log n) | O(1) average, O(n) worst case |
| Insertion time | O(log n) | O(1) average, O(n) worst case |
| Best for | Sorted key-value pairs | Fast lookups when ordering is unnecessary |

`std::map` is **great when you need ordered keys and log-time complexity**, whereas `std::unordered_map` is **faster for general-purpose key-value storage** where ordering does not matter.

By understanding `std::map`, you can efficiently store and manipulate key-value data structures in `C++`!

