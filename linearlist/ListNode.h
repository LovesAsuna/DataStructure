#ifndef CPLUSPLUS_LISTNODE_H
#define CPLUSPLUS_LISTNODE_H

#define ListNodePosi(T) ListNode<T> *
typedef int Rank;

template<typename T>
struct ListNode {
    T data;
    // 前驱
    ListNodePosi(T)pred;
    // 后继
    ListNodePosi(T)succ;

    ListNode() = default;

    explicit ListNode(T e, ListNodePosi(T)p = nullptr, ListNodePosi(T)s = nullptr) : data(e), pred(p), succ(s) {}

    // 前插入
    ListNodePosi(T)insertAsPred(T const &e) {
        ListNodePosi(T)x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
    }

    // 后插入
    ListNodePosi(T)insertAsSucc(T const &e) {
        ListNodePosi(T)x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};

#endif
