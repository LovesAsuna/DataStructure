#ifndef CPLUSPLUS_AVL_MACRO_H
#define CPLUSPLUS_AVL_H
#define tallerChild(x) ( \
    stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*左高*/ \
    stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*右高*/ \
    IsLChild( * (x) ) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
    ) \
    ) \
 )

#include "Macro/Basic_Macro.h"
#include "Macro/AVL_Macro.h"
#include "BST.h" //基于BST实现AVL树

template<typename T>
class AVL : public BST<T> { //由BST派生AVL树模板类
public:
    // 将关键码e插入AVL树中
    BinNodePosi(T)insert(const T &e) {
        BinNodePosi(T)&x = search(e);
        //确 认目标节点不存在
        if (x) return x;
        // 创建新节点x
        BinNodePosi(T)xx = x = new BinNode<T>(e, this->_hot);
        this->_size++;
        // 此时，x的父亲_hot若增高，则其祖父有可能失衡
        // 从x之父出发向上，逐层检查各代祖先g
        for (BinNodePosi(T)g = this->_hot; g; g = g->parent) {
            // 一旦发现g失衡，则（采用“3 + 4”算法）使之复衡，并将子树
            if (!AvlBalanced(*g)) {
                FromParentTo (*g) = rotateAt(tallerChild(tallerChild(g))); //重新接入原树
                break; // g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
            } else {// 否则（g依然平衡），只需简单地
                updateHeight(g); // 更新其高度（注意：即便g未失衡，高度亦可能增加）
            }
        } // 至多只需一次调整；若果真做过调整，则全树高度必然复原
        return xx; //返回新节点位置
    }// 无论e是否存在于原树中，总有AVL::insert(e)->weight == e

    // 从AVL树中删除关键码e
    bool remove(const T &e) {
        BinNodePosi(T)&x = search(e);
        // 确认目标存在（留意_hot的设置）
        if (!x) {
            return false;
        }
        removeAt(x, this->_hot);
        // 先按BST规则删除之（此后，原节点之父_hot及其祖先均可能失衡）
        this->_size--;
        // 从_hot出发向上，逐层检查各代祖先g
        for (BinNodePosi(T)g = this->_hot; g; g = g->parent) {
            // 一旦发现g失衡，则（采用“3 + 4”算法）使之复衡，并将该子树联至
            if (!AvlBalanced (*g)) {
                g = FromParentTo (*g) = rotateAt(tallerChild (tallerChild(g))); //原父亲
            }
            // 并更新其高度（注意：即便g未失衡，高度亦可能降低）
            updateHeight(g);
        } // 可能需做Omega(logn)次调整——无论是否做过调整，全树高度均可能降低
        return true; // 删除成功
    }// 若目标节点存在且被删除，返回true；否则返回false
    // BST::search()等其余接口可直接沿用
};

#endif
