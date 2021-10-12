#include <iostream>
#include "linearlist/Queue.h"

//请独立完成
using namespace std;

typedef enum {
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus;

struct vertxNode {
    int id = -1;
    vertxNode *neighbor = nullptr;
};


struct vertxList {
    int id = -1;
    vertxNode *neighbor = nullptr;
    vertxNode *last = nullptr;
    VStatus status = UNDISCOVERED;
    int parent = -1;
};


void addNeighbor(vertxList &list, int newNeighbor) {
    if (!list.neighbor) {
        list.neighbor = new vertxNode();
        list.neighbor->id = newNeighbor;
        list.last = list.neighbor;
    } else {
        list.last->neighbor = new vertxNode();
        list.last = list.last->neighbor;
        list.last->id = newNeighbor;
    }
}

vertxNode *firstNeighbor(int v, vertxList *list) {
    return list[v].neighbor;
}

vertxNode *nextNeighbor(vertxNode *neighbor) {
    return neighbor->neighbor;
}

bool bfs(int v, vertxList *list) {
    Queue<int> Q;
    list[v].status = DISCOVERED;
    Q.enqueue(v);
    while (!Q.isEmpty()) {
        int v = Q.dequeue();
        // cout << v << " ";
        for (vertxNode *u = firstNeighbor(v, list); u; u = nextNeighbor(u)) {
            if (UNDISCOVERED == list[u->id].status) {
                list[u->id].status = DISCOVERED;
                Q.enqueue(u->id);
                list[u->id].parent = v;
            } else {
                if (list[v].parent != u->id) {
                    return false;
                }
            }
        }
        list[v].status = VISITED;
    }
    return true;
}

int main() {
    int n, m;
    cin >> n >> m;
    auto v = new vertxList[n];
    for (int i = 0; i < n; i++) {
        v[i].id = i;
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        addNeighbor(v[a], b);
        addNeighbor(v[b], a);
    }
    cout << (bfs(0, v) ? 1 : -1);
    return 0;
}