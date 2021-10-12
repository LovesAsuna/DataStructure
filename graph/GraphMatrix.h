#ifndef CPLUSPLUS_GRAPHMATRIX_H
#define CPLUSPLUS_GRAPHMATRIX_H

#include "Graph.h"
#include "../linearlist/Vector.h"

template<typename Te>
struct Edge {
    Te data;
    // 权重
    int weight;
    EType type;

    Edge(Te const &d, int w) : data(d), weight(w), type(UNDETERMINED) {}

    bool operator<=(Edge<Te> &edge) {
        return weight <= edge.weight;
    }
};

template<typename Tv>
struct Vertex {
    Tv data;
    int inDegree;
    int outDegree;
    VStatus status;
    // 时间标签
    int dTime, fTime;
    // 遍历树中的父节点
    int parent;
    // 在遍历树中的优先级(最短通路、极端跨边)
    int priority;

    explicit Vertex(Tv const &d) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1),
                                   fTime(-1),
                                   parent(-1), priority(INT_MAX) {};

    explicit Vertex() : inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1),
                        fTime(-1),
                        parent(-1), priority(INT_MAX) {}

    bool operator<=(Vertex<Tv> &vertex) {
        return priority <= vertex.priority;
    }
};

template<typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te> {
private:
    // 顶点集(向量)
    Vector<Vertex<Tv>> V;
    // 边集(邻接矩阵)
    Vector<Vector<Edge<Te> *>> E;
public:
    GraphMatrix() {
        this->n = this->e = 0;
    }

    Tv &vertex(int i) {
        return V[i].weight;
    }

    int inDegree(int i) {
        return V[i].inDegree;
    }

    int outDegree(int i) {
        return V[i].outDegree;
    }

    VStatus &status(int i) {
        return V[i].status;
    }

    EType &type(int i, int j) {
        return E[i][j]->type;
    }

    bool exists(int i, int j) override {
        return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != nullptr;
    }

    int &dTime(int i) {
        return V[i].dTime;
    }

    Te &edge(int i, int j) {
        return E[i][j]->data;
    }

    int &weight(int i, int j) {
        return E[i][j]->weight;
    }

    void insert(Te const &edge, int w, int i, int j) {
        // 忽略已有的边
        if (exists(i, j)) {
            return;
        }
        // 创建新边
        E[i][j] = new Edge<Te>(edge, w);
        this->e++;
        V[i].outDegree++;
        V[j].inDegree++;
    }

    // 插入顶点，返回编号
    int insert(Tv const &vertex) {
        for (int j = 0; j < this->n; j++) {
            E[j].insert(nullptr);
        }
        this->n++;
        E.insert(Vector<Edge<Te> *>(this->n));
        for (int i = 0; i < this->n; i++) {
            E[this->n - 1].insert(nullptr);
        }
        return V.insert(Vertex<Tv>(vertex));
    }

    // 删除第i个顶点及其关联边（0 <= i < n）
    Tv remove(int i) {
        // 所有出边
        for (int j = 0; j < this->n; j++) {
            if (exists(i, j)) {
                delete E[i][j];
                V[j].inDegree--;
                this->e--;
            } // 逐条删除
        }
        // 删除第i行
        E.remove(i);
        this->n--;
        Tv vBak = vertex(i);
        // 删除顶点i
        V.remove(i);
        // 所有入边
        for (int j = 0; j < this->n; j++) {
            if (Edge<Te> *x = E[j].remove(i)) {
                delete x;
                V[j].outDegree--;
                this->e--;
            } //逐条删除
        }
        // 返回被删除顶点的信息
        return vBak;
    }

    Te remove(int i, int j) {
        // 备份边(i, j)的信息
        Te eBak = edge(i, j);
        // 删除边(i, j)
        delete E[i][j];
        E[i][j] = nullptr;
        // 更新边计数
        this->e--;
        V[i].outDegree--;
        V[j].inDegree--;
        return eBak;
    }

    int &fTime(int i) {
        return V[i].fTime;
    }

    int &parent(int i) {
        return V[i].parent;
    }

    int &priority(int i) {
        return V[i].priority;
    }

    int nextNbr(int i, int j) {
        while ((-1 < j) && !exists(i, --j));
        return j;
    }

    int firstNbr(int i) {
        return nextNbr(i, this->n);
    }

    ~GraphMatrix() {
        for (int j = 0; j < this->n; j++) {
            for (int k = 0; k < this->n; k++) {-
                delete E[j][k];
            }
        }
    }
};

#endif
