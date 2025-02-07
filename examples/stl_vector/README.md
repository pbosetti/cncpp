# STL `vector`

In C++, vectors are **dynamically-sized arrays**. They offer a convenient and efficient way to store collections of elements of the same type. Imagine a flexible container that automatically adjusts its size as you add or remove elements, that's essentially what a vector is.

## Declaring a vector

There are several ways to declare a vector. Here are some examples:

```cpp
vector<int> v = {1, 2, 3}; // initializer list
vector<int> vect1{10, 20, 30}; // brace initialization
vector<int> v; // empty vector
vector<int> vect(n, 10); // size n, all values 10
vector<int> vect(10); // size 10
```

You can also create a vector of vectors, which is essentially a 2D dynamic array (or matrix):

```cpp
// create a vector containing a single vector with the elements 1, 2, and 3.
vector<vector<data_type>> vec;
v2 = {1, 2, 3};
vec.push_back(v2);

// create a 5x5 matrix with all elements set to 0, you can use
vector<vector<int>> v(5, vector<int>(5, 0));
```

## Accessing elements

1. Using the `at()` method

    ```cpp
    vector<int> num {1, 2, 3, 4, 5};
    cout << "Element at Index 0: " << num.at(0) << endl;
    ```

2. Using the `[]` operator

    ```cpp
    cout << num[1];
    ```

The `at()` method is preferred over the `[]` operator because it throws an exception if you try to access an element that is out of bounds. The `[]` operator, on the other hand, will simply give you a *garbage* value.

```cpp
vector<int> num {1, 2, 3};

// gives garbage value
cout << num[4];

// throws an exception
cout << num.at(4);
```

- **`front()`** returns the first element of the vector.

```cpp
vector<int> v = {1, 2, 3};
int first_element = v.front(); // first_element = 1
```

- **`back()`** returns the last element of the vector.

```cpp
vector<string> v = {"apple", "banana"};
string last_element = v.back(); // last_element = "banana"
```

> **Note**: `front()` and `back()` throw an exception if the vector is empty.

## Auto keyword

> The `auto` keyword in `C++` automatically detects and assigns a data type to the variable with which it is used. This can be helpful for making your code more concise and readable. Notably, `auto` is a **type inference** keyword, not a data type.

Here are some examples of how to use the `auto` keyword:

```cpp
auto integer_var{25}; // creates integer type variable
auto float_var{26.77}; // creates float type variable
auto string_var{"Hello"}; // creates string type variable
auto char_var{"C"}; // creates char type variable
```

> Since C++11, initialization can be done using curly braces `{}` or the assignment operator `=`.

You can also use `auto` with references:

```cpp
auto number1 = 10;

auto & number2 = number1; // number2 is a reference to number1
number2 = 20; // modifies the original value of number1
```

## Foreach loop

The `for` loop is a powerful tool in `C++` for iterating over a sequence of elements. However, it can be a bit verbose...

```cpp
for (int i = 0; i < numbers.size(); i++) {
  cout << numbers[i] << " ";
}
```

The `foreach` loop is a more concise way to iterate over a vector:

```cpp
for (data_type variable_name : container_name) {
  // operations using variable_name
}
```

Here's an example of how to use the `foreach` loop to iterate over a vector of numbers:

```cpp
vector<int> numbers = {1, 2, 3, 4, 5};

// modifies original vector
for (auto & num : numbers) {
  num *= 2; // do something with num
}

// does NOT modify original vector
for (auto num : numbers) {
  num *= 2; // do something with num
}
```

The `foreach` loop is a great way to make your code more concise and readable. It's especially useful for working with vectors.

## Iterators

> `C++` Vector Iterators are powerful tools for accessing and manipulating elements within a vector. They act like pointers, but with additional functionalities specific to vector operations.

## Declaring iterators

The syntax for declaring a vector iterator is:

```cpp
vector<T>::iterator iterator_name;
```

Here, `T` represents the type of elements stored in the vector.

**Example**

```cpp
// iterator for an int vector
vector<int>::iterator iter1;

// iterator for a double vector
vector<double>::iterator iter2;
```

## Initializing iterators

There are two main ways to initialize vector iterators:

1. **Using `begin()` and `end()`**

- `begin()` returns an iterator pointing to the first element.
- `end()` refers to the theoretical element after the last element.

**Example**

```cpp
vector<int> num = {1, 2, 3};
vector<int>::iterator iter;

// iter points to num[0]
iter = num.begin();
cout << "num[0] = " << *iter << endl;

// iter points to num[2]
iter = num.end() - 1;
```

2. **Using reverse iterators**

- `rbegin()` returns a reverse iterator pointing to the last element.
- `rend()` refers to the theoretical element before the first element.

**Example**

```cpp
vector<int> num = {1, 2, 3};
auto iter = num.rbegin(); // iter points to num[2]

iter++; // iter points to num[1]
iter++; // iter points to num[0]

// loop through the vector in reverse order
for (auto i = num.rbegin(); i != num.rend(); ++i) {
  cout << *i << " "; // prints 3 2 1
}
```

## Looping through vectors with iterators

Iterators are commonly used in `for` loops to iterate through each element in a vector.

**Example**

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// forward loop
for (auto i = v.begin(); i != v.end(); ++i) {
  cout << *i << " "; // prints 1 2 3 4 5
}

// reverse loop
for (auto ir = v.rbegin(); ir != v.rend(); ++ir) {
  cout << *ir << " "; // prints 5 4 3 2 1
}
```

Using iterators provides more control and flexibility compared to traditional indexing. They offer functionalities like element insertion, deletion, and distance calculation, making them essential tools for working with vectors in C++.

# Vector functionalities

This section explores some essential functionalities used to manipulate elements within a vector

## Adding elements

- **`push_back()`** adds new elements to the **back** of the vector. This is the most common way to add elements as it maintains the original order and is efficient for large datasets.

```cpp
vector<int> v = {1, 2, 3};
v.push_back(4);
v.push_back(5);
// v now contains: 1, 2, 3, 4, 5
```

- **`emplace_back()`** *constructs* and *adds* a new element to the back of the vector. It avoids unnecessary copies compared to `push_back()` and can be more efficient for large objects.

```cpp
vector<string> v = {"apple", "banana"};
v.emplace_back("orange");
// v now contains: "apple", "banana", "orange"
```

Let's break down the differences between `insert`, `push_back`, and `emplace_back` functions in a user-friendly way.

## `push_back`

- Usage:

    ```cpp
    vector<int> numbers = {1, 2, 3};
    numbers.push_back(4);
    ```

- **Explanation**: You use `push_back` when you want to add an element to the back (end) of the vector. It's like adding a new item to the end of a line.

## `emplace_back`

- Usage:

    ```cpp
    vector<string> names = {"Alice", "Bob"};
    names.emplace_back("Charlie");
    ```

- **Explanation**: `emplace_back` is similar to `push_back`, but it directly constructs the element in place. It's like welcoming a new person into a group by constructing them right there instead of preparing them separately and then adding.

## `insert`

- **Purpose**: Inserting elements at a specified position in the vector.

- Usage:

    ```cpp
    vector<int> numbers = {1, 2, 3};
    auto it = numbers.begin() + 1;
    numbers.insert(it, 4);
    ```

- **Explanation**: `insert` is like adding elements at a specific point in the vector. It's like squeezing a new person into a group but not necessarily at the end – you decide where they fit.

**Inserting books (Example)**

- **Push**: This is like adding a book to the **end** of a specific section. It's quick and easy, like adding a new book to the "Science" section at the end of the other science books.
- **Emplace**: This is like building a book directly on the shelf. It's more efficient for large or complex books, like building a custom scrapbook directly on the shelf instead of bringing it from another location.

In summary, `push_back` is for adding to the end, `emplace_back` is for constructing and adding to the end.

- **insert(position, value)** inserts a new element at a specified **position** within the vector. The existing elements at and after the position are shifted to make room for the new element.

```cpp
vector<int> v = {1, 2, 3};
v.insert(v.begin() + 1, 10); // Insert 10 at index 1
// v now contains: 1, 10, 2, 3
```

- **insert(position, n, value)** inserts n *copies* of a specific value at a specified position.

```cpp
vector<char> v = {'a', 'b', 'c'};
v.insert(v.begin() + 1, 2, 'x'); // Insert 2 'x' at index 1
// v now contains: 'a', 'x', 'x', 'b', 'c'
```

- **insert(position, begin, end)** inserts a *range of elements* from another container (begin and end iterators) at a specified position.

```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2 = {4, 5, 6};
v1.insert(v1.begin() + 2, v2.begin(), v2.end());
// v1 now contains: 1, 2, 4, 5, 6, 3
```

## Removing elements

- **`pop_back()`** removes the element from the *back* of the vector. This is efficient for large datasets as it does not need to shift elements.

```cpp
vector<string> v = {"apple", "banana", "orange"};
v.pop_back();
// v now contains: "apple", "banana"
```

- **erase(position)** removes the element at a specified **position** from the vector. The remaining elements are shifted down to fill the gap.

```cpp
vector<int> v = {1, 2, 3, 4};
v.erase(v.begin() + 2); // erase element at index 2
// v now contains 1, 2, and 4

vector<string> fruits = {"apple", "banana", "orange"};
fruits.erase(fruits.end() - 1); // Remove the last element ("orange")

// fruits now contains {"apple", "banana"}
```

- **erase(begin, end)** removes a **range of elements** from a specified position (begin and end iterators) within the vector.

```cpp
vector<char> v = {'a', 'b', 'c', 'd', 'e'};
v.erase(v.begin() + 1, v.begin() + 3); // erases elements from index 1 to 2 (exclusive)
// v now contains 'a', 'd', and 'e'
```

## Complexity of `erase`

The time complexity of `erase` depends on the operation:

- **Single element**: O(n), where n is the number of elements after the erased element. This is because remaining elements need to be shifted to fill the gap.
- **Range of elements**: O(n + m), where n is the number of elements after the erased range and m is the number of erased elements.
- **Erasing the last element**: O(1), as no shifting is necessary.

## Other operations

- **`swap()`** exchanges the contents of two vectors of the same type. This is useful when swapping large datasets as it avoids copying elements.

```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2 = {4, 5, 6};
v1.swap(v2);
```

# Vector operations

This section explores additional functions for managing vector size and accessing elements.

## Size and capacity

- **`size()`** returns the number of elements currently present in the vector.

```cpp
vector<int> v = {1, 2, 3};
int size = v.size(); // size = 3
```

- **`capacity()`** returns the size of the memory currently allocated to the vector. This may be larger than the actual number of elements.

```cpp
vector<int> v;
v.push_back(1); // capacity might be increased
size_t capacity = v.capacity(); // capacity might be > 1
```

## Resizing and shrinking

- **resize(n)** changes the size of the vector to n. If n is smaller than the current size, elements are removed. If it's larger, new elements are added with their default values.

```cpp
vector<int> v = {1, 2, 3};
v.resize(5); // v becomes {1, 2, 3, 0, 0}
```

- **resize(n, value)** is similar to `resize(n)`, but assigns the specified value to all new elements added.

```cpp
vector<string> v = {"apple", "banana"};
v.resize(5, "orange"); // v becomes {"apple", "banana", "orange", "orange", "orange"}
```

- **`shrink_to_fit()`** reduces the capacity of the vector to match its size.

```cpp
vector<int> v(100); // allocated space for 100 elements
v.push_back(1); // only 1 element added
v.shrink_to_fit(); // capacity reduced to 1
```

- **reserve(n)** requests that the vector capacity be at least enough to hold n elements. This avoids reallocation when adding elements.

```cpp
vector<char> v;
v.reserve(100); // ensures space for at least 100 elements
```

- **`empty()`** checks if the vector contains any elements.

```cpp
vector<int> v;
bool is_empty = v.empty(); // is_empty is true
```

## Modifying elements

- **assign(n, value)** replaces all elements in the vector with the specified value.

```cpp
vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

v.assign(10, 0);
deb(v); // v = {0,0,0,0,0,0,0,0,0,0}
deb(v.capacity());

v.assign(20, 0); // creates a vector of size 20 and assigns 0 to all elements
deb(v); // v = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
deb(v.capacity());

v.assign(5, 10); // creates a vector of size 5 and assigns 10 to all elements
deb(v); // v = {10,10,10,10,10}
deb(v.capacity()); // capacity remains same (not lowered)
```

## Additional functionalities

- `clear()` removes all elements from the vector, but **does not change its capacity**.
- `front()` and `back()` are not iterators and **should not be used for loop iterations**.

By understanding these functions, you can effectively manage the size and content of your vectors, leading to efficient and optimized code.

## Advantages of vectors

- Vectors have contiguous memory allocation, which means that elements are stored in contiguous memory locations. This makes accessing elements faster compared to other containers like lists and deques. Note that accessing elements in a vector using the `[]` operator is faster than using the `at()` function, as the `at()` function performs a range check.
- Vectors have a dynamic size, which means that they can be resized during runtime.
- Vectors support random access, which means that elements can be accessed in constant time using an index.

## Disadvantages of vectors

- Inserting or deleting elements in the middle of a vector can be expensive, as all the elements after the insertion or deletion point have to be shifted.
- Vectors have a fixed capacity, which means that if the number of elements exceeds the capacity, the vector has to be reallocated and all the elements have to be copied to the new memory location. This can be expensive in terms of time and memory. For example, if the capacity of a vector is 10, and we try to insert the 11th element, the vector will be reallocated to a new memory location with a capacity of 20 and all the 10 elements will be copied to the new memory location.