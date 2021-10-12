#ifndef CPLUSPLUS_LEFTHEAP_H
#define CPLUSPLUS_LEFTHEAP_H

#include "ComplHeap.h"
#include "../Tree/BinTree.h"

// 基于二叉树，以左式堆形式实现的PQ
template<typename T>
class LeftHeap : public PQ<T>, public BinTree<T> {
public:
    // 默认构造
    LeftHeap() = default;

    // 批量构造：可改进为Floyd建堆算法
    LeftHeap(T *E, int n) {
        for (int i = 0; i < n; i++) {
            insert(E[i]);
        }
    }

    // 按照比较器确定的优先级次序插入元素
    void insert(T e) {
        // 将e封装为左式堆，与当前左式堆合并
        this->_root = merge(this->_root, new BinNode<T>(e, NULL));
        // 更新规模
        this->_size++;
    }

    // 取出优先级最高的元素
    T getMax() {
        // 按照此处约定，堆顶即优先级最高的词条
        return this->_root->data;
    }

    // 删除优先级最高的元素
    T delMax() {
        BinNodePosi(T)lHeap = this->_root->lc;
        // 左子堆
        if (lHeap) {
            lHeap->parent = NULL;
        }
        BinNodePosi(T)rHeap = this->_root->rc;
        // 右子堆
        if (rHeap) {
            rHeap->parent = NULL;
        }
        T e = this->_root->data;
        delete this->_root;
        // 删除根节点
        this->_size--;
        // 合并原左、右子堆
        this->_root = merge(lHeap, rHeap);
        // 返回原根节点的数据项
        return e;
    }

    // 根据相对优先级确定适宜的方式，合并以a和b为根节点的两个左式堆
    static BinNodePosi(T)merge(BinNodePosi(T)a, BinNodePosi(T)b) {
        // 退化情况
        if (!a) {
            return b;
        }
        // 退化情况
        if (!b) {
            return a;
        }
        // 一般情况：首先确保b不大
        if (a->data < b->data) {
            swap(a, b);
        }
        // 将a的右子堆，与b合并
        (a->rc = merge(a->rc, b))->parent = a;
        // 若有必要
        if (!a->lc || a->lc->npl < a->rc->npl) {
            // 交换a的左、右子堆，以确保右子堆的npl不大
            swap(a->lc, a->rc);
        }
        // 更新a的npl
        a->npl = a->rc ? a->rc->npl + 1 : 1;
        return a; // 返回合并后的堆顶
    } //本算法只实现结构上的合并，堆的规模须由上层调用者负责更新
};

#endif
