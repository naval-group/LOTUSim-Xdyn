#ifndef DEMOCLASS_HPP_
#define DEMOCLASS_HPP_

#include <string>

class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }
};

std::string call_go(Animal *animal);
std::string call_go(Animal *animal) {
    return animal->go(3);
}

#endif