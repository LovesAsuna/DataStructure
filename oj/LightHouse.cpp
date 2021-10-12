#include <iostream>
#include <cstdlib>

#define LL long long

using namespace std;
const int max_size = 1 * 1e6;

struct Posi {
    LL x;
    LL y;
} p[max_size];

int LightHouseCmp(const void *a, const void *b) {
    Posi *c = (Posi *) a;
    Posi *d = (Posi *) b;
    if (c->x != d->x) {
        return c->x - d->x;
    } else {
        return d->y - c->y;
    }
}

LL getCount(LL lo, LL hi) {
    if (hi <= lo) {
        return 0;
    }
    LL mi = (lo + hi) / 2;
    LL lCount = getCount(lo, mi);
    LL rCount = getCount(mi + 1, hi);
    LL count = 0;

    LL i = lo, j = mi + 1, k = 0;
    auto *temp = new LL[hi - lo + 1];
    while (i <= mi && j <= hi) {
        if (p[i].y <= p[j].y) {
            count += (hi - j + 1);
            temp[k++] = p[i++].y;
        } else {
            temp[k++] = p[j++].y;
        }
    }
    while (i <= mi) {
        temp[k++] = p[i++].y;
    }
    while (j <= hi) {
        temp[k++] = p[j++].y;
    }
    for (i = lo, k = 0; i <= hi; i++, k++) {
        p[i].y = temp[k];
    }
    delete[]temp;

    return lCount + count + rCount;
}

int lighthouse() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> p[i].x >> p[i].y;
    }
    qsort(p, n, sizeof(p[n]), LightHouseCmp);

    cout << getCount(0, n - 1) << endl;

    return 0;
}