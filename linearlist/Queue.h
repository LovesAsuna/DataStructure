#ifndef CPLUSPLUS_QUEUE_H
#define CPLUSPLUS_QUEUE_H

#include "List.h"

template<typename T>
class Queue : public List<T> {
public:
    void enqueue(T const &e) {
        this->insertAsLast(e);
    }

    T dequeue() {
        return this->remove(this->first());
    }

    T &front() {
        return this->first()->data;
    }

    friend ostream &operator<<(ostream &o, Queue<T> &q) {
        ListNodePosi(T)first = q.first();
        while (first != q.last()) {
            o << first->data << " ";
            first = first->succ;
        }
        o << first->data << endl;
        return o;
    }

};

#endif