#ifndef CPLUSPLUS_HASHTABLE_H
#define CPLUSPLUS_HASHTABLE_H

#include "Dictionary.h" //引入词典ADT
#include "../map/BitMap.h" //引入位图
#include "../system/Release.h"
#include <cstring>

template<typename K, typename V> //key、value
class Hashtable : public Dictionary<K, V> { //符合Dictionary接口的Hashtable模板类
private:
    Entry<K, V> **ht; //桶数组，存放词条指针
    int M; //桶数组容量
    int N; //词条数量
    Bitmap *lazyRemoval; //懒惰删除标记
    // 根据file文件中的记录，在[c, n)内取最小的素数
    int primeNLT(int c, int n, const char *file) {
        // file已经按位图格式，记录了n以内的所有素数，因此只要
        Bitmap B(file, n);
        // 从c开始，逐位地
        while (c < n) {
            // 测试，即可
            if (B.test(c)) {
                c++;
            } else {// 返回首个发现的素数
                return c;
            }
        }

        return c; //若没有这样的素数，返回n（实用中不能如此简化处理）
    }

#define lazilyRemoved(x)  (lazyRemoval->test(x))
#define markAsRemoved(x)  (lazyRemoval->set(x))
protected:
    // 沿关键码k对应的查找链，找到词条匹配的桶
    int probe4Hit(const K &k) {
        // 从起始桶（按除余法确定）出发
        int r = hashCode(k) % M;
        // 沿查找链线性试探：跳过所有冲突的桶，以及带懒惰删除标记的桶
        while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved (r))) {
            r = (r + 1) % M;
        }
        // 调用者根据ht[r]是否为空，即可判断查找是否成功
        return r;
    }

    // 沿关键码k对应的查找链，找到首个可用空桶
    int probe4Free(const K &k) {
        // 从起始桶（按除余法确定）出发
        int r = hashCode(k) % M;
        // 沿查找链逐桶试探，直到首个空桶（无论是否带有懒惰删除标记）
        while (ht[r]) r = (r + 1) % M;
        // 为保证空桶总能找到，装填因子及散列表长需要合理设置
        return r;
    }

    // 重散列算法：扩充桶数组，保证装填因子在警戒线以下
    void rehash() {
        int old_capacity = M;
        Entry<K, V> **old_ht = ht;
        M = primeNLT(2 * M, 1048576, "../_input/prime-1048576-bitmap.txt"); //容量至少加倍
        N = 0;
        ht = new Entry<K, V> *[M];
        // 新桶数组
        memset(ht, 0, sizeof(Entry<K, V> *) * M);
        release(lazyRemoval);
        // 新开懒惰删除标记比特图
        lazyRemoval = new Bitmap(M);
        // 扫描原桶数组
        for (int i = 0; i < old_capacity; i++) {
            // 将非空桶中的词条逐一
            if (old_ht[i]) {
                put(old_ht[i]->key, old_ht[i]->value); //插入至新的桶数组
            }
        }
        release(old_ht); //释放原桶数组——由于其中原先存放的词条均已转移，故只需释放桶数组本身
    }

public:
    // 创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
    explicit Hashtable(int c = 5) {
        M = primeNLT(c, 1048576, "../_input/prime-1048576-bitmap.txt"); //不小于c的素数M
        N = 0;
        ht = new Entry<K, V> *[M]; //开辟桶数组（还需核对申请成功），初始装填因子为N/M = 0%
        memset(ht, 0, sizeof(Entry<K, V> *) * M); //初始化各桶
        lazyRemoval = new Bitmap(M); //懒惰删除标记比特图
    }

    // 释放桶数组及其中各（非空）元素所指向的词条
    ~Hashtable() {
        // 逐一检查各桶
        for (int i = 0; i < M; i++) {
            // 释放非空的桶
            if (ht[i]) {
                release(ht[i]);
            }
        }
        release(ht); //释放桶数组
        release(lazyRemoval); //释放懒惰删除标记
    }

    // 当前的词条数目
    int size() const {
        return N;
    }

    // 插入（禁止雷同词条，故可能失败）
    bool put(K k, V v) {
        // 雷同元素不必重复插入
        if (ht[probe4Hit(k)]) {
            return false;
        }
        // 为新词条找个空桶（只要装填因子控制得当，必然成功）
        int r = probe4Free(k);
        ht[r] = new Entry<K, V>(k, v);
        // 插入（注意：懒惰删除标记无需复位）
        ++N;
        // 装填因子高于50%后重散列
        if (N * 2 > M) {
            rehash();
        }
        return true;
    }

    // 散列表词条查找算法
    V *get(K k) {
        int r = probe4Hit(k);
        return ht[r] ? &(ht[r]->value) : nullptr;
    }

    // 散列表词条删除算法
    bool remove(K k) {
        int r = probe4Hit(k);
        //对应词条不存在时，无法删除
        if (!ht[r]) {
            return false;
        }
        release(ht[r]);
        ht[r] = nullptr;
        markAsRemoved(r);
        N--;
        return true;
        //否则释放桶中词条，设置懒惰删除标记，并更新词条总数
    }

    // 字符
    static size_t hashCode(char c) {
        return (size_t) c;
    }

    // 整数以及长长整数
    static size_t hashCode(int k) {
        return (size_t) k;
    }

    static size_t hashCode(long long i) {
        return (size_t) ((i >> 32) + (int) i);
    }

    // 生成字符串的循环移位散列码（cyclic shift h code）
    static size_t hashCode(char s[]) {
        // 散列码
        unsigned int h = 0;
        // 自左向右，逐个处理每一字符
        for (size_t n = strlen(s), i = 0; i < n; i++) {
            h = (h << 5) | (h >> 27);
            h += (int) s[i];
        } //散列码循环左移5位，再累加当前字符
        return (size_t) h; //如此所得的散列码，实际上可理解为近似的“多项式散列码”
    } //对于英语单词，"循环左移5位"是实验统计得出的最佳值
};

#endif
