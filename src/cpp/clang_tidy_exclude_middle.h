#ifndef CLANG_TIDY_EXCLUDE_MIDDLE_H
#define CLANG_TIDY_EXCLUDE_MIDDLE_H

template<class T>
class B : public A {
public:
    B() {
        std::cout << "B()\n";
        this->method(); 
    }
    virtual void method() { std::cout << "B::method" << std::endl; }

    virtual ~B() { std::cout << "B::destructor" << std::endl; }
};

#endif