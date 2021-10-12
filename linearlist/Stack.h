#ifndef CPLUSPLUS_STACK_H
#define CPLUSPLUS_STACK_H

#include "Vector.h"

template<typename T>
class Stack : public Vector<T> {
public:
    Stack() = default;

    explicit Stack(int size) : Vector<T>(size) {}

    void push(T const &e) {
        this->insert(this->size(), e);
    }

    T pop() {
        return this->remove(this->size() - 1);
    }

    T &top() {
        return (*this)[this->size() - 1];
    }

    friend ostream &operator<<(ostream &o, Stack<T> &s) {
        o << "栈顶->栈底: [";
        int size = s._size;
        while (size--) {
            o << s._elem[size] << " ";
        }
        if (!s.isEmpty()) {
            o << "\b";
        }
        o << "]";
        return o;
    }
};

#endif
