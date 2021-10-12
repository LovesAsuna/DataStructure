#ifndef CPLUSPLUS_COMPLHEAP_H
#define CPLUSPLUS_COMPLHEAP_H

#include "Vector.h"
#include "PQ.h"
#include "Macro/ComplHeap_Macro.h"

// 完全二叉堆
template<typename T>
class ComplHeap : public PQ<T>, public Vector<T> {
private:
    // Floyd建堆算法
    void heapify(T *A, Rank n) {
        // 自底而上，依次
        for (int i = n / 2 - 1; 0 <= i; i--) {
            // 下滤各内部节点
            percolateDown(A, n, i);
        }
    }

    // 对向量前n个词条中的第i个实施下滤，i < n
    Rank percolateDown(T *A, Rank n, Rank i) {
        // i及其(之多两个)孩子中，堪为父者
        Rank j;
        // 只要i非j，则
        while (i != (j = ProperParent(A, n, i))) {
            // 两者换位，并继续考查下降后的i
            swap(A[i], A[j]);
            i = j;
        }
        // 返回下滤抵达的位置(亦i亦j)
        return i;
    }

    // 对向量中的第i个词条实施上滤操作，i < _size
    Rank percolateUp(T *A, Rank i) {
        // 在抵达堆顶之前，反复地
        while (0 < i) {
            // 考察[i]的父亲[j]
            Rank j = Parent(i);
            // 一旦父子顺序， 上滤完成; 否则
            if (A[i] < A[j]) {
                break;
            }
            // 父子换位，并继续考查上一层
            swap(A[i], A[j]);
            i = j;
        }
        return i;
    }

public:
    // 默认构造
    ComplHeap() = default;

    // 批量构造
    ComplHeap(T *A, Rank n) {
        this->copyFrom(A, 0, n);
        heapify(this->_elem, n);
    }

    // 按照比较器确定的优先级次序，插入词条
    void insert(T const &e) {
        Vector<T>::insert(e);
        percolateUp(this->_elem, this->_size - 1);
    }

    // 读取优先级最高的词条
    T getMax() {
        return this->_elem[0];
    }

    // 删除优先级最高的词条
    T delMax() {
        T maxElem = this->_elem[0];
        // 摘除堆顶(首词条)，代之以末词条
        this->_elem[0] = this->_elem[--this->_size];
        // 对新堆顶实施下滤
        percolateDown(this->_elem, this->_size, 0);
        return maxElem;
    }

};

#endif
