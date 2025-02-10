# STL `list`

In `C++`, `std::list` is a **doubly linked list** that provides dynamic memory allocation and efficient insertion/deletion at both ends and anywhere in the list. Unlike `std::vector`, which uses **contiguous memory**, a `list` consists of **nodes** where each node stores an element and pointers to its **previous** and **next** elements.

## Declaring a `list`

A `list` can be declared in multiple ways:

```cpp
#include <list>

std::list<int> lst; // empty list
std::list<int> lst1 = {1, 2, 3}; // list with elements
std::list<int> lst2{4, 5, 6}; // brace initialization
std::list<int> lst3(5, 10); // list with 5 elements, all set to 10
```

A `list` can store **user-defined objects** as follows:

```cpp
struct Person {
  std::string name;
  int age;
};

std::list<Person> people = {{"Alice", 25}, {"Bob", 30}};
```

## Accessing elements

Unlike `vector`, `std::list` **does not support random access** using `[]` or `at()`. Instead, elements are accessed using **iterators**.

```cpp
std::list<int> lst = {10, 20, 30, 40};

// Forward iteration
for (auto it = lst.begin(); it != lst.end(); ++it) {
  std::cout << *it << " ";
}
```

The functions `front()` and `back()` can be used to access the **first** and **last** elements, respectively.

```cpp
std::list<int> lst = {10, 20, 30};
std::cout << lst.front(); // Output: 10
std::cout << lst.back();  // Output: 30
```

## Iterating through a `list`

- **Using range-based for loop**

```cpp
std::list<int> lst = {1, 2, 3, 4, 5};
for (auto num : lst) {
  std::cout << num << " ";
}
```

- **Using reverse iterators**

```cpp
for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
  std::cout << *it << " ";
}
```

## Adding elements

Elements can be added at the **beginning** or **end** of the list using the following functions:

- **`push_back(value)`** adds an element to the end.
- **`push_front(value)`** adds an element to the beginning.
- **`emplace_back(value)`** constructs an element at the end.
- **`emplace_front(value)`** constructs an element at the beginning.

```cpp
std::list<int> lst;
lst.push_back(10);  // {10}
lst.push_front(20); // {20, 10}
lst.emplace_back(30);  // {20, 10, 30}
lst.emplace_front(40); // {40, 20, 10, 30}
```

To insert elements at a **specific position**, use an iterator together with the `insert()` function.

```cpp
auto it = lst.begin();
std::advance(it, 2); // Move iterator to 3rd element
lst.insert(it, 15);  // Insert 15 at the 3rd position
```

## Removing elements

Elements can be removed using the following functions:

- **`pop_back()`** removes the last element.
- **`pop_front()`** removes the first element.
- **`erase(iterator)`** removes the element at the given iterator.
- **`erase(start, end)`** removes a range of elements.
- **`remove(value)`** removes all occurrences of a value.

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

### Sorting
Unlike `vector`, `std::list` provides a built-in `sort()` function.

```cpp
std::list<int> lst = {5, 1, 4, 2, 3};
lst.sort(); // lst = {1, 2, 3, 4, 5}
```

### Reversing
```cpp
lst.reverse(); // lst = {5, 4, 3, 2, 1}
```

### Merging two lists
```cpp
std::list<int> lst1 = {1, 3, 5};
std::list<int> lst2 = {2, 4, 6};

lst1.merge(lst2); // lst1 = {1, 2, 3, 4, 5, 6}
```

### Removing duplicates
```cpp
lst.unique();
```
> The list must be **sorted** before using `unique()`.

### Splicing (moving elements between lists)

- **`splice(position, source_list)`** moves the entire `source_list` into another list at the specified `position`.

```cpp
std::list<int> lst1 = {1, 2, 3};
std::list<int> lst2 = {4, 5, 6};

auto it = lst1.begin();
std::advance(it, 2); // move to the 3rd element
lst1.splice(it, lst2); // moves all elements from lst2 into lst1
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
- Fast insertion and deletion anywhere in the list $\mathcal{O}(1)$.
- No memory reallocation (unlike `vector`).
- Efficient for frequent insertions/deletions in the middle.

## Disadvantages of `std::list`
- No random access ($\mathcal{O}(n)$ to access an element).
- Higher memory overhead due to extra pointers.
- Slower traversal compared to `vector`.

## `std::vector` vs `std::list`

| Use Case | Choose |
|----------|--------|
| Fast insertion/removal | `std::list` |
| Frequent middle insertions/deletions | `std::list` |
| Random access needed | `std::vector` |
| Fast traversal required | `std::vector` |
