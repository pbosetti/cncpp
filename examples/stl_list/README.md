# STL `list`

In C++, `std::list` is a **doubly linked list** that provides dynamic memory allocation and efficient insertion/deletion at both ends and anywhere in the list. Unlike `std::vector`, which uses **contiguous memory**, a `list` consists of **nodes** where each node stores an element and pointers to its **previous** and **next** elements.

## Declaring a `list`

A `list` can be declared in multiple ways:

```cpp
#include <list>

std::list<int> lst; // empty list
std::list<int> lst1 = {1, 2, 3}; // list with elements
std::list<int> lst2{4, 5, 6}; // brace initialization
std::list<int> lst3(5, 10); // list with 5 elements, all set to 10
```

### Declaring a list of objects
A `list` can store **user-defined objects**:

```cpp
struct Person {
  std::string name;
  int age;
};

std::list<Person> people = {{"Alice", 25}, {"Bob", 30}};
```

## Accessing elements

Unlike `vector`, `std::list` **does not support random access** using `[]` or `at()`. Instead, elements are accessed using **iterators**.

### Using iterators

```cpp
std::list<int> lst = {10, 20, 30, 40};

// Forward iteration
for (auto it = lst.begin(); it != lst.end(); ++it) {
  std::cout << *it << " ";
}
```

### Using `front()` and `back()`
- **front()** returns the first element.
- **back()** returns the last element.

```cpp
std::list<int> lst = {10, 20, 30};
std::cout << lst.front(); // Output: 10
std::cout << lst.back();  // Output: 30
```

## Iterating through a `list`

### Using range-based for loop

```cpp
std::list<int> lst = {1, 2, 3, 4, 5};
for (auto num : lst) {
  std::cout << num << " ";
}
```

### Using reverse iterators
To traverse in reverse order:

```cpp
for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
  std::cout << *it << " ";
}
```

## Adding elements

- **push_back(value)** → Adds an element to the end.
- **push_front(value)** → Adds an element to the beginning.
- **emplace_back(value)** → Constructs an element at the end.
- **emplace_front(value)** → Constructs an element at the beginning.

```cpp
std::list<int> lst;
lst.push_back(10);  // {10}
lst.push_front(20); // {20, 10}
lst.emplace_back(30);  // {20, 10, 30}
lst.emplace_front(40); // {40, 20, 10, 30}
```

### Using `insert()`
To insert elements at a **specific position**, use an iterator:

```cpp
auto it = lst.begin();
std::advance(it, 2); // Move iterator to 3rd element
lst.insert(it, 15);  // Insert 15 at the 3rd position
```

## Removing elements

- **pop_back()** → Removes the last element.
- **pop_front()** → Removes the first element.
- **erase(iterator)** → Removes the element at the given iterator.
- **erase(start, end)** → Removes a range of elements.
- **remove(value)** → Removes all occurrences of a value.

```cpp
std::list<int> lst = {1, 2, 3, 4, 5};
lst.pop_back(); // {1, 2, 3, 4}
lst.pop_front(); // {2, 3, 4}

auto it = lst.begin();
std::advance(it, 1); // Move to 2nd element
lst.erase(it); // Remove 2nd element

lst.remove(4); // Remove all occurrences of 4
```

## List operations

### **Sorting a list**
Unlike `vector`, `std::list` provides a built-in `sort()` function.

```cpp
std::list<int> lst = {5, 1, 4, 2, 3};
lst.sort(); // lst = {1, 2, 3, 4, 5}
```

### **Reversing a list**
```cpp
lst.reverse(); // lst = {5, 4, 3, 2, 1}
```

### **Merging two sorted lists**
```cpp
std::list<int> lst1 = {1, 3, 5};
std::list<int> lst2 = {2, 4, 6};

lst1.merge(lst2); // lst1 = {1, 2, 3, 4, 5, 6}
```

### **Removing duplicates**
```cpp
lst.unique();
```
> The list must be **sorted** before using `unique()`.

## Splicing (Moving elements between lists)

- **splice(position, source_list)** → Moves the entire `source_list` into another list at the specified `position`.

```cpp
std::list<int> lst1 = {1, 2, 3};
std::list<int> lst2 = {4, 5, 6};

auto it = lst1.begin();
std::advance(it, 2); // Move to the 3rd element
lst1.splice(it, lst2); // Moves all elements from lst2 into lst1
```

## Capacity functions

| Function  | Description |
|-----------|-------------|
| `size()`  | Returns the number of elements |
| `empty()` | Returns `true` if the list is empty |
| `clear()` | Removes all elements |

```cpp
std::list<int> lst = {1, 2, 3};
std::cout << lst.size(); // Output: 3

lst.clear();
std::cout << lst.empty(); // Output: true
```

## Advantages of `std::list`
- **Fast insertion and deletion** anywhere in the list (O(1)).
- **No memory reallocation** (unlike `vector`).
- **Efficient for frequent insertions/deletions** in the middle.

## Disadvantages of `std::list`
- **No random access** (O(n) to access an element).
- **Higher memory overhead** due to extra pointers.
- **Slower traversal** compared to `vector`.

## When to Use `std::list`

| Use Case | Choose |
|----------|--------|
| Fast insertion/removal | `std::list` |
| Frequent middle insertions/deletions | `std::list` |
| Random access needed | `std::vector` |
| Fast traversal required | `std::vector` |
