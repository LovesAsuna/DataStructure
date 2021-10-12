#ifndef CPLUSPLUS_GRAPH_H
#define CPLUSPLUS_GRAPH_H

#include "../linearlist/Stack.h"
#include "../linearlist/Queue.h"
#include <climits>

typedef enum {
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus;

typedef enum {
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} EType;

template<typename Tv, typename Te>
class Graph {
protected:
    // 所有顶点、边的辅助信息复位
    void reset() {
        for (int i = 0; i < n; i++) { //所有顶点的
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1; //状态，时间标签
            parent(i) = -1;
            priority(i) = INT_MAX; //（在遍历树中的）父节点，优先级数
            for (int j = 0; j < n; j++) { //所有边的
                if (exists(i, j)) {
                    type(i, j) = UNDETERMINED; //类型
                }
            }

        }
    }

    //（连通域）广度优先搜索算法
    virtual void BFS(int v, int &clock) {
        Queue<int> Q;
        status(v) = DISCOVERED;
        // 初始化
        Q.enqueue(v);
        // 反复地
        while (!Q.isEmpty()) {
            int v = Q.dequeue();
            // 取出队列首顶点
            dTime(v) = ++clock;
            // 并考察v的每一邻居u
            for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
                // 若u尚未被发现，则
                if (UNDISCOVERED == status(u)) {
                    // 发现该顶点
                    status(u) = DISCOVERED;
                    Q.enqueue(u);
                    // 引入数边
                    type(v, u) = TREE;
                    parent(u) = v;
                } else {
                    // 若已被发现(正在队列中)，或者甚至已访问完毕(已出队列)，则
                    // 将(v,u)归类于跨边
                    type(v, u) = CROSS;
                }
            }
            status(v) = VISITED;
        }
    }

    //（连通域）深度优先搜索算法
    virtual void DFS(int v, int &clock) {
        // 发现当前顶点
        dTime(v) = ++clock;
        status(v) = DISCOVERED;
        // 枚举v的每一邻居u
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
            // 并试其状态分别处理
            switch (status(u)) {
                // u尚未被发现，意味着支撑树可在此拓展
                case UNDISCOVERED:
                    type(v, u) = TREE;
                    parent(u) = v;
                    // 递归
                    DFS(u, clock);
                    break;
                    // u已被发现，但尚未访问完毕，应属被后代指向的祖先
                case DISCOVERED:
                    type(v, u) = BACKWARD;
                    break;
                    // u已访问完毕(VISITED, 有向图)，则视承袭关系分为前向边或跨边
                default:
                    type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
                    break;
            }
        }
        status(v) = VISITED;
        fTime(v) = ++clock;
    }

// 利用此处闲置的fTime[]充当hca[]
#define hca(x) (fTime(x))

    //（连通域）基于DFS的双连通分量分解算法
    virtual void BCC(int v, int &clock, Stack<int> &S) {
        hca(v) = dTime(v) = ++clock;
        status(v) = DISCOVERED;
        // v被发现并入栈
        S.push(v);
        // 枚举v的所有邻居u
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
            // 并视u的状态分别处理
            switch (status(u)) {
                case UNDISCOVERED:
                    parent(u) = v;
                    type(v, u) = TREE;
                    // 从顶点u处深入
                    BCC(u, clock, S);
                    // 遍历返回后，若发现u（通过后向边）可指向v的真祖先
                    if (hca(u) < dTime(v)) {
                        // 则v亦必如此
                        hca(v) = hca(v) < hca(u) ? hca(v) : hca(u);
                    } else {
                        // 否则，以v为关节点（u以下即是一个BCC，且其中顶点此时正集中于栈S的顶部）
                        while (u != S.pop()); //弹出当前BCC中（除v外）的所有节点，可视需要做进一步处理
                    }
                    break;
                case DISCOVERED:
                    // 标记(v, u)，并按照“越小越高”的准则
                    type(v, u) = BACKWARD;
                    // 更新hca[v]
                    if (u != parent(v)) {
                        hca(v) = hca(v) < dTime(u) ? hca(v) : dTime(u);
                    }
                    break;
                default: //VISITED (digraphs only)
                    type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                    break;
            }
        }
        status(v) = VISITED; //对v的访问结束
    }

    //（连通域）基于DFS的拓扑排序算法
    virtual bool TSort(int v, int &clock, Stack<Tv> *S) {
        dTime(v) = ++clock;
        // 发现顶点v
        status(v) = DISCOVERED;
        // 枚举v的所有邻居u
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
            // 并视u的状态分别处理
            switch (status(u)) {
                case UNDISCOVERED:
                    parent(u) = v;
                    type(v, u) = TREE;
                    // 从顶点u处出发深入搜索
                    if (!TSort(u, clock, S)) {
                        // 若u及其后代不能拓扑排序（则全图亦必如此），故返回并报告
                        return false;
                    }
                    break;
                case DISCOVERED:
                    // 一旦发现后向边（非DAG），则
                    type(v, u) = BACKWARD;
                    // 不必深入，故返回并报告
                    return false;
                default:
                    //VISITED (digraphs only)
                    type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                    break;
            }
        }
        status(v) = VISITED;
        // 顶点被标记为VISITED时，随即入栈
        S->push(vertex(v));
        return true; //v及其后代可以拓扑排序
    }

    //（连通域）优先级搜索框架
    template<typename PU>
    void PFS(int s, PU prioUpdater) {
        priority(s) = 0;
        status(s) = VISITED;
        // 初始化，起点s加至PFS树中
        parent(s) = -1;
        while (true) {
            // 将下一顶点和边加至PFS树中枚举s的所有邻居w
            for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) {
                // 更新顶点w的优先级及其父顶点
                prioUpdater(this, s, w);
            }
            for (int shortest = INT_MAX, w = 0; w < n; w++) {
                // 从尚未加入遍历树的顶点中
                if (UNDISCOVERED == status(w))
                    // 选出下一个
                    if (shortest > priority(w)) {
                        shortest = priority(w);
                        s = w;
                    } //优先级最高的顶点s
            }
            // 直至所有顶点均已加入
            if (VISITED == status(s)) {
                break;
            }
            status(s) = VISITED;
            // 将s及与其父的联边加入遍历树
            type(parent(s), s) = TREE;
        }
    }

public:
    // 顶点
    //顶点总数
    int n;

    //插入顶点，返回编号
    virtual int insert(Tv const &) = 0;

    // 删除顶点及其关联边，返回该顶点信息
    virtual Tv remove(int) = 0;

    // 顶点v的数据（该顶点的确存在）
    virtual Tv &vertex(int) = 0;

    // 顶点v的入度（该顶点的确存在）
    virtual int inDegree(int) = 0;

    // 顶点v的出度（该顶点的确存在）
    virtual int outDegree(int) = 0;

    // 顶点v的首个邻接顶点
    virtual int firstNbr(int) = 0;

    // 顶点v的（相对于顶点j的）下一邻接顶点
    virtual int nextNbr(int, int) = 0;

    // 顶点v的状态
    virtual VStatus &status(int) = 0;

    // 顶点v的时间标签dTime
    virtual int &dTime(int) = 0;

    // 顶点v的时间标签fTime
    virtual int &fTime(int) = 0;

    // 顶点v在遍历树中的父亲
    virtual int &parent(int) = 0;

    // 顶点v在遍历树中的优先级数
    virtual int &priority(int) = 0;

    // 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
    // 边总数
    int e;

    // 边(v, u)是否存在
    virtual bool exists(int, int) = 0;

    // 在顶点v和u之间插入权重为w的边e
    virtual void insert(Te const &, int, int, int) = 0;

    // 删除顶点v和u之间的边e，返回该边信息
    virtual Te remove(int, int) = 0;

    // 边(v, u)的类型
    virtual EType &type(int, int) = 0;

    // 边(v, u)的数据（该边的确存在）
    virtual Te &edge(int, int) = 0;

    // 边(v, u)的权重
    virtual int &weight(int, int) = 0;

    // 算法
    // 广度优先搜索算法
    void bfs(int s) {
        // 初始化
        reset();
        int clock = 0;
        int v = s;
        // 逐一检查所有顶点
        do {
            // 一旦遇到尚未发现的顶点
            if (UNDISCOVERED == status(v)) {
                // 即从该顶点触发启动一次BFS
                BFS(v, clock);
            }
        } while (s != (v = (++v % n))); // 按序号检查，故不漏不重
    }

    // 深度优先搜索算法
    void dfs(int s) {
        // 初始化
        reset();
        int clock = 0;
        int v = s;
        // 逐一检查所有顶点
        do {
            // 一旦遇到尚未发现的顶点
            if (UNDISCOVERED == status(v)) {
                // 即从该顶点出发启动一次DFS
                DFS(v, clock);
            }
        } while (s != (v = (++v % n))); // 按序号检查，故不漏不重
    }

    // 基于DFS的双连通分量分解算法
    void bcc(int s) {
        reset();
        int clock = 0;
        int v = s;
        // 栈S用以记录已访问的顶点
        Stack<int> S;
        do {
            // 一旦发现未发现的顶点（新连通分量）
            if (UNDISCOVERED == status(v)) {
                // 即从该顶点出发启动一次BCC
                BCC(v, clock, S);
                // 遍历返回后，弹出栈中最后一个顶点——当前连通域的起点
                S.pop();
            }
        } while (s != (v = (++v % n)));
    }

    // 基于DFS的拓扑排序算法
    Stack<Tv> *tSort(int s) {
        reset();
        int clock = 0;
        int v = s;
        // 用栈记录排序顶点
        Stack<Tv> *S = new Stack<Tv>;
        do {
            if (UNDISCOVERED == status(v)) {
                // clock并非必需
                if (!TSort(v, clock, S)) {
                    // 任一连通域（亦即整图）非DAG
                    while (!S->empty())
                        S->pop();
                    break; //则不必继续计算，故直接返回
                }
            }
        } while (s != (v = (++v % n)));
        return S; // 若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
    }

    // 最小支撑树Prim算法
    void prim(int s) {
        reset();
        priority(s) = 0;
        // 共需引入n个顶点和n-1条边
        for (int i = 0; i < n; i++) {
            status(s) = VISITED;
            // 引入当前的s
            if (-1 != parent(s)) {
                type(parent(s), s) = TREE;
            }
            // 枚举s的所有邻居j
            for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) {
                // 对邻接顶点j做松弛
                if ((status(j) == UNDISCOVERED) && (priority(j) > weight(s, j))) {
                    priority(j) = weight(s, j);
                    parent(j) = s;
                } //与Dijkstra算法唯一的不同之处
            }
            // 选出下一极短跨边
            for (int shortest = INT_MAX, j = 0; j < n; j++) {
                if ((status(j) == UNDISCOVERED) && (shortest > priority(j))) {
                    shortest = priority(j);
                    s = j;
                }
            }
        }
    }

    // 最短路径Dijkstra算法
    void dijkstra(int s) {
        reset();
        priority(s) = 0;
        // 共需引入n个顶点和n-1条边
        for (int i = 0; i < n; i++) {
            status(s) = VISITED;
            // 引入当前的s
            if (-1 != parent(s)) {
                type(parent(s), s) = TREE;
            }
            // 枚举s的所有邻居j
            for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) {
                // 对邻接顶点j做松弛
                if ((status(j) == UNDISCOVERED) && (priority(j) > priority(s) + weight(s, j))) {
                    priority(j) = priority(s) + weight(s, j);
                    parent(j) = s;
                } //与Prim算法唯一的不同之处
            }
            // 选出下一最近顶点
            for (int shortest = INT_MAX, j = 0; j < n; j++) {
                if ((status(j) == UNDISCOVERED) && (shortest > priority(j))) {
                    shortest = priority(j);
                    s = j;
                }
            }
        }
    }

    // 优先级搜索框架
    template<typename PU>
    void pfs(int s, PU prioUpdater) {
        reset();
        // 初始化
        int v = s;
        // 逐一检查所有顶点
        do {
            // 一旦遇到尚未发现的顶点
            if (UNDISCOVERED == status(v)) {
                // 即从该顶点出収启劢一次PFS
                PFS(v, prioUpdater);
            }
        } while (s != (v = (++v % n))); // 按序号检查，故丌漏丌重
    }
};

// 针对Prim算法的顶点优先级更新器
template<typename Tv, typename Te>
struct PrimPU {
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v) {
        // 对于uk每一尚未被发现的邻接顶点v
        if (UNDISCOVERED == g->status(v))
            // 按Prim策略做松弛
            if (g->priority(v) > g->weight(uk, v)) {
                // 更新优先级（数）
                g->priority(v) = g->weight(uk, v);
                // 更新父节点
                g->parent(v) = uk;
            }
    }
};

// 针对Dijkstra算法的顶点优先级更新器
template<typename Tv, typename Te>
struct DijkPU {
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v) {
        // 对于uk每一尚未被发现的邻接顶点v，按Dijkstra策略
        if (UNDISCOVERED == g->status(v))
            // 做松弛
            if (g->priority(v) > g->priority(uk) + g->weight(uk, v)) {
                // 更新优先级（数）
                g->priority(v) = g->priority(uk) + g->weight(uk, v);
                // 并同时更新父节点
                g->parent(v) = uk;
            }
    }
};


#endif
