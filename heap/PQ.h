#ifndef CPLUSPLUS_PQ_H
#define CPLUSPLUS_PQ_H

template<typename T>
struct PQ {
    // 按照比较器确定的优先级次序插入词条
    virtual void insert(T const &) = 0;

    // 取出优先级最高的词条
    virtual T getMax() = 0;

    // 删除优先级最高的词条
    virtual T delMax() = 0;
};

#endif
