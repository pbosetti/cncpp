#include <iostream>
#include <string>
#include <cmath>

// The "unsing namespace" allows to by-default
// use the functions in the standard library (
// which are the ones that start with std::)
// without specifying std::
// std::cout becomes cout
using namespace std;

// This is called function overloading
// it allows to write the same function with
// different arguments types / number
void describe(int value);
void describe(double value);

int main() {
    // Use std::cout for all the common messages
    // Use std::cerr for warnings/errors.
    cout << "Msg to standard output" << endl;
    cerr << "Msg to standard error" << endl;
    // We tested the two lines above by doing:
    // ./build/ex3 > log.txt
    // This wrote a file log.txt with the things
    // that were written to standard output (std::cout)
    // while the std::cerr was written to the terminal.

    // Most common types
    int age = 20;
    double height = 1.82;
    bool is_sudent = false;
    char initial = 'M'; // Pay attention to the single quote, 'M' -> char, "M" -> string
    string name = "Mario"; // std::string name = "Mario";

    // Common functions from cmath;
    pow(2, 10.0); // std::pow(2, 10.0);
    sqrt(10.0); // std::sqrt(10.0);
    round(8.99); // std::round(8.99); // becomes 9.0
    ceil(5.01); // std::ceil(5.01); // becomes 6.0
    floor(5.9); // std::floor(5.9); // becomes 5.0

    while (age > 0) {
        age --;
        // age -= 1;
        // age = age - 1;

        // this is local only, cannot be accessed from
        // outside the while loop
        int new_age = age;
    }
    // cout << new_age << endl; // Not visible in this scope

    cout << age << endl; // should be 0
    {
        int age = 1;
        int new_age = 30;

        // This will be 1
        cout << "Inside scope: " << age << endl;
    }
    cout << age << endl;
    // cout << new_age << endl; // again new_age is not visible here

    // C++ will call the function that fits better our arguments.
    // given our oveloading it is either an integer or a double argument
    describe(1); // Int version
    describe(2 / 3); // Int version!!!! This is a division between two integers and is equal to 0
    describe(2.0 / 3.0); // Doube division!!! this is equal to 0.66..
    describe(2.0); // Double version

    // std::string, std::to_string()
    // This is concatenation of multiple strings to create a line.
    string line = "Hello " + name + ", age: " + to_string(age);
    cout << line << " " << line.size() << endl;
    // Clearing all the content of the string, so its size will be 0
    line.clear();
    cout << line << " " << line.size() << endl;

    return 0;
}


// Function definition.
// Here based on the type we implement differently
void describe(int value) {
    cout << "Int version of describe: " << value << endl;
}
void describe(double value) {
    cout << "Double version of describe: " << value << endl;
}