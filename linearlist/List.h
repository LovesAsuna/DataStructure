#ifndef CPLUSPLUS_LIST_H
#define CPLUSPLUS_LIST_H

#include "ListNode.h"
#include <iostream>

using namespace std;

template<typename T>
class List {
private:
    //规模
    int _size{};
    ListNodePosi(T)header;
    ListNodePosi(T)trailer;
protected:
    // 从起始于位置p的n个元素中选出最大者
    virtual ListNodePosi(T)selectMax(ListNodePosi(T)p, int n) {
        // 最大者暂定为p
        ListNodePosi(T)max = p;
        // 从首节点p出发，将后续节点逐一与max比较
        for (ListNodePosi(T)cur = p; 1 < n; n--) {
            //若当前元素不小于max，则
            if ((cur = cur->succ)->data >= max->data) {
                max = cur; //更新最大元素位置记录
            }
        }
        return max;
    }

public:
    List() {
        init();
    }

    //整体复制列表L
    List(List<T> const &L) {
        copyNodes(L.first(), L._size);
    }

    // 复制列表L中自第r项起的n项
    List(List<T> const &L, Rank r, int n) {
        ListNodePosi(T)p = L.first();
        while (0 < r--) {
            p = p->succ;
        }
        copyNodes(p, n);
    }

    // 复制列表中自位置p起的n项（assert: p为合法位置，且至少有n-1个后继节点）
    List(ListNodePosi(T)p, int n) {
        copyNodes(p, n);
    }

    ListNodePosi(T)first() const {
        return header->succ;
    }

    ListNodePosi(T)last() const {
        return trailer->pred;
    }

    // 判断位置p是否对外合法
    bool valid(ListNodePosi(T)p) {
        return p && (trailer != p) && (header != p);
    } //将头、尾节点等同于NULL

    ListNodePosi(T)insertBefore(ListNodePosi(T)p, T const &e) {
        _size++;
        return p->insertAsPred(e);
    }

    ListNodePosi(T)insertAfter(ListNodePosi(T)p, T const &e) {
        _size++;
        return p->insertAsSucc(e);
    }

    ListNodePosi(T)insertAsLast(T const &e) {
        _size++;
        return trailer->insertAsPred(e);
    }

    ListNodePosi(T)insertAsFirst(T const &e) {
        _size++;
        return header->insertAsSucc(e);
    }

    T remove(ListNodePosi(T)p) {
        // 备份待删除节点数值(设类型T可直接赋值)
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;
    }

    void copyNodes(ListNodePosi(T)p, int n) {
        // 创建头、尾哨兵节点并做初始化
        init();
        while (n--) {
            insertAsLast(p->data);
            p = p->succ;
        }
    }

    void init() {
        header = new ListNode<T>;
        trailer = new ListNode<T>;
        header->succ = trailer;
        header->pred = nullptr;
        trailer->pred = header;
        trailer->succ = nullptr;
        _size = 0;
    }

    ~List() {
        clear();
        delete header;
        delete trailer;
    }

    int clear() {
        int oldSize = _size;
        while (0 < _size) {
            remove(header->succ);
        }
        return oldSize;
    }

    Rank size() const {
        return _size;
    }

    bool isEmpty() {
        return _size <= 0;
    }

    // 删除无序列表中的重复元素
    int deduplicate() {
        // 平凡列表自然无重复
        if (_size < 2) {
            return 0;
        }
        int oldSize = _size;
        // p从首节点起
        ListNodePosi(T)p = first();
        int r = 1;
        // 依次直到末节点
        while (trailer != (p = p->succ)) {
            // 在p的r个(真)前驱中，查找与之雷同者
            ListNodePosi(T)q = find(p->data, r, p);
            //若存在删除之，否则秩增加
            q ? remove(q) : r++;
        }
        // 链表规模变化量，即被删除元素总数
        return oldSize - _size;
    }

    // 成批剔除重复元素
    int uniquify() {
        // 平凡列表自然无重复
        if (_size < 2) {
            return 0;
        }
        int oldSize = _size;
        // p从首节点起
        ListNodePosi(T)p = first();
        // q为p的后继
        ListNodePosi(T)q;
        while (trailer != (q = p->succ)) {
            if (p->data != q->data) {
                p = q;
            } else {
                remove(q);
            }
            return oldSize - _size;
        }
    }

    // 在节点p(可能是trailer)的n个(真)前驱中，找到等于e的最后者
    // 从外部调用时,0<=n<=Rank(p)<=_size
    ListNodePosi(T)find(T const &e, int n, ListNodePosi(T)p) const {
        // 从右向左，逐个将p的前驱与e比对
        while (0 < n--) {
            // 直至命中或范围越界
            if (e == (p = p->pred)->data) {
                return p;
            }
        }
        // 若越出左边界，意外着查找失败
        return nullptr;
    }

    // 在有序链表内节点p的n个(真)前驱中，找到不大于e的最后者
    ListNodePosi(T)search(T const &e, int n, ListNodePosi(T)p) const {
        // 对于p的最近的n个前驱，从右向左
        while (0 < n--) {
            if (((p = p->pred)->data) <= e) {
                break;
            }
        }
        // 直至命中、数值越界或范围越界，返回查找终止的位置
        return p;
    }

    // 对列表中起始于位置p的连续n个元素做选择排序
    void selectSort(ListNodePosi(T)p, int n) {
        //待排序区间(head, tail)
        ListNodePosi(T)head = p->pred;
        ListNodePosi(T)tail = p;
        for (int i = 0; i < n; i++) {
            tail = tail->succ;
        }
        while (1 < n) {
            insertBefore(tail, remove(selectMax(head->succ, n)));
            tail = tail->pred;
            n--;
        }
    }

    void insertionSort(ListNodePosi(T)p, int n) {
        // 逐一引入各节点.由S(r)得到S(r+1)
        for (int r = 0; r < n; r++) {
            // 查找+插入
            insertAfter(search(p->data, r, p), p->data);
            // 转向下一节点
            p = p->succ;
            remove(p->pred);
        }
    }

    T &operator[](Rank r) const { //assert: 0 <= r < size
        //从首节点出发
        ListNodePosi(T)p = first();
        //顺数第r个节点即是
        while (0 < r--) p = p->succ;
        //目标节点，迒回其中所存元素
        return p->data;
    }

    friend ostream &operator<<(ostream &out, List<T> &L) {
        ListNodePosi(T)p = L.first();
        out << "[";
        while (p != L.last()) {
            out << p->data << ", ";
            p = p->succ;
        }
        out << p->data << "]" << endl;
        return out;
    }

    int operator<=(List<T> &v) {
        int index = 0;
        ListNode<T> *p = first();
        ListNode<T> *q = v.first();
        while (p->data == q->data) {
            p = p->succ;
            q = q->succ;
        }
        if (p->data > q->data) {
            return 1;
        } else if (p->data < q->data) {
            return -1;
        } else {
            return 0;
        }
    }
};

#endif
