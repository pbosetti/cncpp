# Lambda Functions in C++

Lambda functions in C++ provide a convenient way to define anonymous functions (functions without a name) inline. They are particularly useful for callbacks, event handling, and functional programming techniques.

## Syntax

A lambda function has the following general syntax:
```cpp
[capture](parameters) -> return_type { function_body };
```
- **Capture list (`[]`)**: defines how external variables are accessed.
- **Parameter list (`()`)**: specifies function arguments (like normal functions).
- **Return type (`-> return_type`)** *(optional)*: specifies the return type.
- **Function body (`{}`)**: contains the function logic.

### Capture list

The capture list allows passing external variables to the lambda:
- `[&]`: captures all variables by reference.
- `[=]`: captures all variables by value.
- `[var1, var2]`: captures `var1` and `var2` by value.
- `[&var1, &var2]`: captures `var1` and `var2` by reference.

## Code breakdown

The provided C++ program demonstrates the use of lambda functions in an object destructor for custom callbacks.

### `CustomDeath` Class

This class represents an object that executes a callback function upon destruction.
- Uses `std::function<void()>` to store a lambda function as a callback.
- Defines a **destructor** (`~CustomDeath()`) that calls the stored lambda function when the object goes out of scope.

#### Key Features:
- The **constructor** initializes the object with a name and a callback function.
- The **destructor** executes the callback function, if defined, before the object is destroyed.

### `main()` function

1. Creates an instance `silent` without a callback.
2. Creates an instance `chatty` with a lambda function as its destruction callback:
   - The lambda captures `chatty` by reference (`[&]`).
   - When `chatty` goes out of scope, the destructor prints a message using the lambda.

```cpp
CustomDeath chatty = CustomDeath("chatty", [&]() {
    cout << chatty.name << ": Oh, no! I'm being released!" << endl;
});
```
