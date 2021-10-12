#ifndef CPLUSPLUS_BINNODE_H
#define CPLUSPLUS_BINNODE_H

#include "Macro/Basic_Macro.h"
#include "../linearlist/Queue.h"
#include "../linearlist/Stack.h"

// 节点位置
#define BinNodePosi(T) BinNode<T>*
// 节点高度（与“空树高度为-1”的约定相统一）
#define stature(p) ((p) ? (p)->height : -1)
// 节点颜色
typedef enum {
    RB_RED, RB_BLACK
} RBColor;

template<typename T>
struct BinNode {
    // 父节点及左、右孩子
    BinNodePosi(T)parent;
    BinNodePosi(T)lc;
    BinNodePosi(T)rc;

    // 数值
    T data;

    BinNode() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) {}

    explicit BinNode(T e, BinNodePosi(T)p = nullptr, BinNodePosi(T)lc = nullptr, BinNodePosi(T)rc = nullptr, int h = 0,
                     int l = 1, RBColor c = RB_RED) : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}

    // 高度（通用）
    int height;

    // Null Path Length（左式堆，也可直接用height代替）
    int npl;

    // 颜色（红黑树）
    RBColor color;

    // 统计当前节点后代总数，即以其为根的子树规模
    int size() {
        // 计入本身
        int s = 1;
        // 递归计入左子树规模
        if (lc) {
            s += lc->size();
        }
        // 递归计入右子树规模
        if (rc) {
            s += rc->size();
        }
        return s;
    }
    // 作为左孩子插入新节点
    BinNodePosi(T)insertAsLC(T const &e) {
        return lc = new BinNode(e, this);
    }

    // 作为右孩子插入新节点
    BinNodePosi(T)insertAsRC(T const &e) {
        return rc = new BinNode(e, this);
    }

    // 取当前节点的直接后继，在中序遍历中不小于当前节点的最小节点
    BinNodePosi(T)succ() {
        // 记录后继的临时变量
        BinNodePosi(T)s = this;
        // 若有右孩子，则直接后继必在右子树中，具体地就是
        if (rc) {
            // 右子树中
            s = rc;
            // 最靠左（最小）的节点
            while (HasLChild(*s)) {
                s = s->lc;
            }
        } else {
            // 否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
            // 逆向地沿右向分支，不断朝左上方移动
            while (IsRChild(*s)) {
                s = s->parent;
            }
            // 最后再朝右上方移动一步，即抵达直接后继（如果存在）
            s = s->parent;
        }
        return s;
    }

    // 子树层次遍历
    template<typename VST>
    void travLevel(VST &visit) {
        // 辅助队列
        Queue<BinNodePosi(T)> Q;
        // 根节点入队
        Q.enqueue(this);
        // 在队列再次变空之前，反复迭代
        while (!Q.isEmpty()) {
            BinNodePosi(T)x = Q.dequeue();
            // 取出队首节点并访问之
            visit(x->data);
            // 左孩子入队
            if (HasLChild (*x)) {
                Q.enqueue(x->lc);
            }
            // 右孩子入队
            if (HasRChild (*x)) {
                Q.enqueue(x->rc);
            }
        }
    }

    // 子树先序遍历
    template<typename VST>
    void travPre(VST &visit) {
        // 此处暂随机选择以做测试，共三种选择
        switch (rand() % 3) {
            case 1:
                travPre_I1(this, visit);
                break; //迭代版#1
            case 2:
                travPre_I2(this, visit);
                break; //迭代版#2
            default:
                travPre_R(this, visit);
                break; //递归版
        }
    }

    // 子树中序遍历
    template<typename VST>
    void travIn(VST &visit) {
        switch (rand() % 5) { //此处暂随机选择以做测试，共五种选择
            case 1:
                travIn_I1(this, visit);
                break; //迭代版#1
            case 2:
                travIn_I2(this, visit);
                break; //迭代版#2
            case 3:
                travIn_I3(this, visit);
                break; //迭代版#3
            case 4:
                travIn_I4(this, visit);
                break; //迭代版#4
            default:
                travIn_R(this, visit);
                break; //递归版
        }
    }

    // 子树后序遍历
    template<typename VST>
    void travPost(VST &visit) {
        // 此处暂随机选择以做测试，共两种选择
        switch (rand() % 2) {
            case 1:
                travPost_I(this, visit);
                break; //迭代版
            default:
                travPost_R(this, visit);
                break; //递归版
        }
    }

    // 比较器、判等器（各列其一，其余自行补充）
    bool operator<(BinNode const &bn) {
        return data < bn.data;
    }

    bool operator==(BinNode const &bn) {
        return data == bn.data;
    }
};

// 二叉树先序遍历算法（迭代版#1）
template<typename T, typename VST>
void travPre_I1(BinNodePosi(T)x, VST &visit) {
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x) S.push(x); //根节点入栈
    while (!S.isEmpty()) { //在栈变空之前反复循环
        x = S.pop();
        visit(x->data); //弹出并访问当前节点，其非空孩子的入栈次序为先右后左
        if (HasRChild(*x)) S.push(x->rc);
        if (HasLChild(*x)) S.push(x->lc);
    }
}

// 从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template<typename T, typename VST>
static void visitAlongVine(BinNodePosi(T)x, VST &visit, Stack<BinNodePosi(T)> &S) {
    while (x) {
        // 访问当前节点
        visit(x->data);
        // 右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
        S.push(x->rc);
        // 沿左分支深入一层
        x = x->lc;
    }
}

// 二叉树先序遍历算法（迭代版#2）
template<typename T, typename VST>
void travPre_I2(BinNodePosi(T)x, VST &visit) {
    // 辅助栈
    Stack<BinNodePosi(T)> S;
    while (true) {
        // 从当前节点出发，逐批访问
        visitAlongVine(x, visit, S);
        // 直到栈空
        if (S.isEmpty()) {
            break;
        }
        // 弹出下一批的起点
        x = S.pop();
    }
}

// 二叉树先序遍历算法（递归版）
template<typename T, typename VST>
void travPre_R(BinNodePosi(T)x, VST &visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点
template<typename T>
static void goAlongVine(BinNodePosi(T)x, Stack<BinNodePosi(T)> &S) {
    // 当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
    while (x) {
        S.push(x);
        x = x->lc;
    }
}

// 二叉树中序遍历算法（迭代版#1）
template<typename T, typename VST>
void travIn_I1(BinNodePosi(T)x, VST &visit) {
    // 辅助栈
    Stack<BinNodePosi(T)> S;
    while (true) {
        // 从当前节点出发，逐批入栈
        goAlongVine(x, S);
        // 直至所有节点处理完毕
        if (S.isEmpty()) break;
        x = S.pop();
        // 弹出栈顶节点并访问之
        visit(x->data);
        // 转向右子树
        x = x->rc;
    }
}

// 二叉树中序遍历算法（迭代版#2）
template<typename T, typename VST>
void travIn_I2(BinNodePosi(T)x, VST &visit) {
    // 辅助栈
    Stack<BinNodePosi(T)> S;
    while (true)
        if (x) {
            // 根节点进栈
            S.push(x);
            // 深入遍历左子树
            x = x->lc;
        } else if (!S.isEmpty()) {
            // 尚未访问的最低祖先节点退栈
            x = S.pop();
            // 访问该祖先节点
            visit(x->data);
            // 遍历祖先的右子树
            x = x->rc;
        } else {
            break; //遍历完成
        }
}

// 二叉树中序遍历算法（迭代版#3，无需辅助栈）
template<typename T, typename VST>
void travIn_I3(BinNodePosi(T)x, VST &visit) {
    // 前一步是否刚从左子树回溯——省去栈，仅O(1)辅助空间
    bool backtrack = false;
    while (true) {
        // 若有左子树且不是刚刚回溯，则
        if (!backtrack && HasLChild (*x)) {
            // 深入遍历左子树
            x = x->lc;
        } else {
            // 否则——无左子树或刚刚回溯（相当于无左子树）
            // 访问该节点
            visit(x->data);
            // 若其右子树非空，则
            if (HasRChild (*x)) {
                // 深入右子树继续遍历
                x = x->rc;
                // 并关闭回溯标志
                backtrack = false;
            } else { //若右子树空，则
                // 回溯（含抵达末节点时的退出返回）
                if (!(x = x->succ())) {
                    break;
                }
                // 并设置回溯标志
                backtrack = true;
            }
        }
    }
}

// 二叉树中序遍历（迭代版#4，无需栈或标志位）
template<typename T, typename VST>
void travIn_I4(BinNodePosi(T)x, VST &visit) {
    while (true) {
        // 若有左子树，则
        if (HasLChild (*x)) {
            // 深入遍历左子树
            x = x->lc;
        } else {
            //否则访问当前节点，并
            visit(x->data);
            // 不断地在无右分支处
            while (!HasRChild (*x)) {
                // 回溯至直接后继（在没有后继的末节点处，直接退出）
                if (!(x = x->succ())) {
                    return;
                } else visit(x->data); //访问新的当前节点
            }
            //（直至有右分支处）转向非空的右子树
            x = x->rc;
        }
    }
}

// 二叉树中序遍历算法（递归版）
template<typename T, typename VST>
void travIn_R(BinNodePosi(T)x, VST &visit) {
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template<typename T>
//在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoLeftmostLeaf(Stack<BinNodePosi(T)> &S) {
    // 沿途所遇节点依次入栈
    // 自顶而下，反复检查当前节点（即栈顶）
    while (BinNodePosi(T)x = S.top())
        // 尽可能向左
        if (HasLChild (*x)) {
            // 若有右孩子，优先入栈
            if (HasRChild (*x)) {
                S.push(x->rc);
            }
            // 然后才转至左孩子
            S.push(x->lc);
        } else {// 实不得已
            // 才向右
            S.push(x->rc);
        }
    // 返回之前，弹出栈顶的空节点
    S.pop();
}

// 二叉树的后序遍历（迭代版）
template<typename T, typename VST>
void travPost_I(BinNodePosi(T)x, VST &visit) {
    // 辅助栈
    Stack<BinNodePosi(T)> S;
    if (x) {
        // 根节点入栈
        S.push(x);
    }
    // x始终为当前节点
    while (!S.isEmpty()) {
        // 若栈顶非x之父（而为右兄）
        if (S.top() != x->parent) {
            // 则在其右兄子树中找到HLVFL（相当于递归深入）
            gotoLeftmostLeaf(S);
        }
        // 弹出栈顶（即前一节点之后继），并访问之
        x = S.pop();
        visit(x->data);
    }
}

// 二叉树后序遍历算法（递归版）
template<typename T, typename VST>
void travPost_R(BinNodePosi(T)x, VST &visit) {
    if (!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

#endif
