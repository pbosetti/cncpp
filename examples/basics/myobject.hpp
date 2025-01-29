#ifndef MYOBJECT_HPP
#define MYOBJECT_HPP
#include <iostream>

class MyObject {
public:
    MyObject(std::string name) : _name(name){
        std::cerr << "MyObject initialized" << std::endl;
    }
    virtual ~MyObject() {
        std::cout << "MyObject destroyed" << std::endl;
    }

    void say_hello();

    // accessors:
    std::string name() const { return _name; }

private:
    std::string _name;
};

#endif // MYOBJECT_HPP