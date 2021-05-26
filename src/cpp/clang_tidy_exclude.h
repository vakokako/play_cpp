#ifndef CLANG_TIDY_EXCLUDE_H
#define CLANG_TIDY_EXCLUDE_H

#include <iostream>

class A {
public:
    A() {
        std::cout << "A()\n";
        method();
    }
    virtual void method() { std::cout << "A::method" << std::endl; }

    void otherMethod() {
        std::cout << "A::othermethod()\n";
        method();
    }

    virtual ~A() {
        std::cout << "A::destructor" << std::endl;
        // method();
    }
};

int dummy() {
    return 6;
}

#endif