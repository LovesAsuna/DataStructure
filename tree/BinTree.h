#ifndef CPLUSPLUS_BINTREE_H
#define CPLUSPLUS_BINTREE_H

#include "BinNode.h"
#include "../system/Release.h"
#include "../linearlist/Stack.h"

template<typename T>
class BinTree {
private:
    int max(int first, int second) {
        return first > second ? first : second;
    }

protected:
    int _size;
    BinNodePosi(T)_root;

    int updateHeight(BinNodePosi(T)x) {
        return x->height = 1 + max(stature(x->lc), stature(x->rc));
    }

    void updateHeightAbove(BinNodePosi(T)x) {
        while (x) {
            updateHeight(x);
            x = x->parent;
        }
    }

public:
    // 构造函数
    BinTree() : _size(0), _root(nullptr) {}

    // 析构函数
    ~BinTree() {
        if (0 < _size) {
            remove(_root);
        }
    }

    int size() const {
        return _size;
    }

    bool isEmpty() const {
        return !_root;
    }

    BinNodePosi(T)root() const {
        return _root;
    }

    // 将e当作根节点插入空的二叉树
    BinNodePosi(T)insertAsRoot(T const &e) {
        _size = 1;
        return _root = new BinNode<T>(e);
    }

    BinNodePosi(T)insertAsRC(BinNodePosi(T)x, T const &e) {
        _size++;
        x->insertAsRC(e);
        updateHeightAbove(x);
        return x->rc;
    }

    BinNodePosi(T)insertAsLC(BinNodePosi(T)x, T const &e) {
        _size++;
        x->insertAsLC(e);
        updateHeightAbove(x);
        return x->lc;
    }

    BinNodePosi(T)attachAsRC(BinNodePosi(T)x, BinTree<T> *&S) { //x->rc == NULL
        x->rc = S->_root;
        if (x->rc) {
            x->rc->parent = x; //接入
        }
        _size += S->_size;
        updateHeightAbove(x); //更新全树规模与x所有祖先的高度
        S->_root = NULL;
        S->_size = 0;
        release(S);
        S = NULL;
        return x; //释放原树，返回接入位置
    }

    BinNodePosi(T)attachAsLC(BinNodePosi(T)x, BinTree<T> *&S) { //x->lc == NULL
        x->lc = S->_root;
        if (x->lc) {
            x->lc->parent = x; //接入
        }
        _size += S->_size;
        updateHeightAbove(x); //更新全树规模与x所有祖先的高度
        S->_root = NULL;
        S->_size = 0;
        release(S);
        S = NULL;
        return x; //释放原树，返回接入位置
    }

    // 删除以位置x处节点为根的子树，返回该子树原先的规模
    int remove(BinNodePosi(T)x) {//assert: x为二叉树中的合法位置
        // 切断来自父节点的指针
        FromParentTo (*x) = nullptr;
        // 更新祖先高度
        updateHeightAbove(x->parent);
        int n = removeAt(x);
        _size -= n;
        // 删除子树x，更新规模，返回删除节点总数
        return n;
    }

    // 删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
    static int removeAt(BinNodePosi(T)x) { //assert: x为二叉树中的合法位置
        if (!x) return 0; //递归基：空树
        int n = 1 + removeAt(x->lc) + removeAt(x->rc); //递归释放左、右子树
        release(x->data);
        release(x);
        // 释放被摘除节点，并返回删除节点总数
        return n;
    } // release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包

    // 将子树x从当前树中摘除，并将其转换为一棵独立子树
    BinTree<T> *secede(BinNodePosi(T)x) {
        // 切断来自父节点的指针
        FromParentTo (*x) = nullptr;
        // 更新原树中所有祖先的高度
        updateHeightAbove(x->parent);
        auto *S = new BinTree<T>;
        S->_root = x;
        // 新树以x为根
        x->parent = nullptr;
        S->_size = x->size();
        _size -= S->_size;
        // 更新规模，返回分离出来的子树
        return S;
    }

    template<typename VST>
    // 操作器
    // 层次遍历
    void travLevel(VST &visit) {
        if (_root) {
            _root->travLevel(visit);
        }
    }

    template<typename VST>
    // 操作器
    // 先序遍历
    void travPre(VST &visit) {
        if (_root) {
            _root->travPre(visit);
        }
    }

    template<typename VST>
    // 操作器
    // 中序遍历
    void travIn(VST &visit) {
        if (_root) {
            _root->travIn(visit);
        }
    }

    template<typename VST>
    // 操作器
    // 后序遍历
    void travPost(VST &visit) {
        if (_root) {
            _root->travPost(visit);
        }
    }

    // 比较器（其余自行补充）
    bool operator<(BinTree<T> const &t) {
        return _root && t._root && (_root < t._root);
    }

    // 判等器
    bool operator==(BinTree<T> const &t) {
        return _root && t._root && (_root == t._root);
    }
};

#endif
