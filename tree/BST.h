#ifndef CPLUSPLUS_BST_H
#define CPLUSPLUS_BST_H

#include "BinTree.h"

template<typename T>
class BST : public BinTree<T> {
protected:
    // “命中”节点的父亲
    BinNodePosi(T)_hot;
    // 按照“3 + 4”结构，联接3个节点及四棵子树
    BinNodePosi(T)connect34(
            BinNodePosi(T)a, BinNodePosi(T)b, BinNodePosi(T)c,
            BinNodePosi(T)T0, BinNodePosi(T)T1, BinNodePosi(T)T2, BinNodePosi(T)T3) {
        a->lc = T0;
        if (T0) T0->parent = a;
        a->rc = T1;
        if (T1) T1->parent = a;
        updateHeight(a);
        c->lc = T2;
        if (T2) T2->parent = c;
        c->rc = T3;
        if (T3) T3->parent = c;
        updateHeight(c);
        b->lc = a;
        a->parent = b;
        b->rc = c;
        c->parent = b;
        updateHeight(b);
        // 该子树新的根节点
        return b;
    }

    // 对v及其父亲、祖父做统一旋转调整，v为非空孙辈节点
    BinNodePosi(T)rotateAt(BinNodePosi(T)v) {
        // 父亲
        BinNodePosi(T)p = v->parent;
        // 祖父
        BinNodePosi(T)g = p->parent;
        // 视v、p和g相对位置分四种情况
        /* zig */
        if (IsLChild (*p)) {
            /* zig-zig */
            if (IsLChild (*v)) {
                // 向上联接
                p->parent = g->parent;
                return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
            } else {
                /* zig-zag */
                v->parent = g->parent;
                // 向上联接
                return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
            }
        } else { /* zag */
            /* zag-zag */
            if (IsRChild (*v)) {
                p->parent = g->parent;
                // 向上联接
                return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
            } else { /* zag-zig */
                v->parent = g->parent;
                // 向上联接
                return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
            }
        }

    }

public:
    // 基本接口：以virtual修饰，强制要求所有派生类（BST变种）根据各自的规则对其重写
    // 在BST中查找关键码e
    virtual BinNodePosi(T)&search(const T &e) {
        if (!this->_root || e == this->_root->data) {
            _hot = NULL;
            return this->_root;
        } // 在树根v处命中
        for (_hot = this->_root;;) { // 自顶而下
            BinNodePosi(T)&c = (e < _hot->data) ? _hot->lc : _hot->rc; // 确定方向
            if (!c || e == c->data) return c;
            _hot = c; // 命中返回，或者深入一层
        } // 无论命中或失败，hot均指向v之父亲（或为NULL）
    }

    // 将关键码e插入BST树中
    virtual BinNodePosi(T)insert(const T &e) {
        BinNodePosi(T)&x = search(e);
        // 确认目标不存在（留意对_hot的设置）
        if (x) return x;
        // 创建新节点x：以e为关键码，以_hot为父
        x = new BinNode<T>(e, _hot);
        this->_size++;
        // 更新全树规模
        // 更新x及其历代祖先的高度
        BinTree<T>::updateHeightAbove(x);
        // 新插入的节点，必为叶子
        return x;
    }// 无论e是否存在于原树中，返回时总有x->weight == e

    // 删除
    virtual bool remove(const T &e) {
        // 定位目标节点
        BinNodePosi(T)&x = search(e);
        // 确认目标节点存在(此时_hot为x的父亲)
        if (!x) {
            return false;
        }
        // 分两大类情况实施删除，更新全树规模
        removeAt(x, _hot);
        // 更新全树规模
        this->_size--;
        // 更新_hot及其历代祖先的高度
        BinTree<T>::updateHeightAbove(_hot);
        // 删除成功与否，由返回值指示
        return true;
    }

    static BinNodePosi(T)removeAt(BinNodePosi(T)&x, BinNodePosi(T)&hot) {
        // 实际被摘除的节点，初值同x
        BinNodePosi(T)w = x;
        // 实际被删除节点的接替者
        BinNodePosi(T)succ;
        // 若*x的左子树为空，则可
        if (!HasLChild (*x)) {
            // 直接将*x替换为其右子树
            succ = x = x->rc;
        } else if (!HasRChild (*x)) {// 若右子树为空，则可
            // 对称地处理——注意：此时succ != NULL
            succ = x = x->lc;
        } else { // 若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
            //（在右子树中）找到*x的直接后继*w
            w = w->succ();
            // 交换*x和*w的数据元素
            swap(x->data, w->data);
            // 实际被删除节点的父亲节点
            BinNodePosi(T)u = w->parent;
            // 隔离节点*w
            ((u == x) ? u->rc : u->lc) = succ = w->rc;
        }
        // 记录实际被删除节点的父亲
        hot = w->parent;
        // 并将被删除节点的接替者与hot相联
        if (succ) succ->parent = hot;
        release(w->data);
        release(w);
        return succ; // 释放被摘除节点，返回接替者
    } // release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包
};

#endif
