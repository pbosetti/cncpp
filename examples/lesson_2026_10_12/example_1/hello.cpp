#include <iostream>

// To compile this program:
// cd ~/Develop/cncpp-main26-27/examples/lesson_2026_10_12
// c++ example_1/hello.cpp -o hello

int main() {
    // std output (terminal) << message << end line;
    std::cout << "Hello everyone" << std::endl;
    std::cout << "Hello " << "everyone" << std::endl; // same as above

    int unused;
    float unused_float;
    double unused_double;

    // These lines here do the same thing: increment i by 1.
    // i ++;
    // i += 1;
    // i = i + 1;
    for (int i = 0; i <= 5; i += 1) {
        std::cout << "Value of i: " << i << std::endl;
    }

    return 0;
}