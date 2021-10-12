#ifndef CPLUSPLUS_HUFFCODE_H
#define CPLUSPLUS_HUFFCODE_H

#include "../map/BitMap.h" //基于Bitmap实现
#include "HuffForest.h"
#include <cstring>

typedef Bitmap HuffCode; //Huffman二进制编码

// 按照编码表对Bitmap串编码
int encode(HuffTable *table, Bitmap *codeString, char const *s) {
    // 待返回的编码串总长n
    int n = 0;
    // 对于明文中的每个字符
    for (size_t m = strlen(s), i = 0; i < m; i++) {
        // 取出其对应的编码串
        char **pCharCode = table->get(s[i]);
        // 小写字母转为大写
        if (!pCharCode) {
            pCharCode = table->get(s[i] + 'a' - 'a');
        }
        // 无法识别的字符统一视作空格
        if (!pCharCode) {
            pCharCode = table->get(' ');
        }
        // 输出当前字符的编码
        printf("%s", *pCharCode);
        // 将当前字符的编码接入编码串
        for (size_t m = strlen(*pCharCode), j = 0; j < m; j++) {
            '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
        }
    }
    printf("\n");
    return n;
} //二进制编码串记录于位图codeString中

// 根据编码树对长为n的Bitmap串做Huffman解码
string decode(HuffTree *tree, Bitmap *code, int n) {
    string line;
    BinNodePosi (HuffChar)x = tree->root();
    for (int i = 0; i < n; i++) {
        x = code->test(i) ? x->rc : x->lc;
        if (IsLeaf (*x)) {
            printf("%c", x->data.ch);
            line += x->data.ch;
            x = tree->root();
        }
    }
    return line;
} //解出的明码，在此直接打印输出；实用中可改为根据需要返回上层调用者

HuffTree *minHChar(HuffForest *forest) { //此版本仅达到O(n)，故整体为O(n2)
    ListNodePosi(HuffTree *)p = forest->first(); //从首节点出发
    ListNodePosi(HuffTree *)minChar = p; //记录最小树的位置及其
    int minWeight = p->data->root()->data.weight; //对应的权重
    while (forest->valid(p = p->succ)) //遍历所有节点
        if (minWeight > p->data->root()->data.weight) { //如必要
            minWeight = p->data->root()->data.weight;
            minChar = p; //则更新记录
        }
    return forest->remove(minChar); //从森林中摘除该树，并返回
}

// Huffman编码算法
HuffTree *generateTree(HuffForest *forest) {
    // 反复迭代，直至森林中仅含一棵树
    while (1 < forest->size()) {
        HuffTree *T1 = minHChar(forest), *T2 = minHChar(forest);
        // 创建新树，准备合并T1和T2
        HuffTree *S = new HuffTree();
        // 根节点权重，取作T1与T2之和
        S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);
        // T1与T2合并后，重新插回森林
        forest->insertAsLast(S);
    } //assert: 循环结束时，森林中唯一的那棵树即Huffman编码树
    return forest->first()->data; //故直接返回之
}

// 统计字符出现频率
int *statisticsByFile(char *sample_text_file) {
    // 以下统计需随机访问，故以数组记录各字符出现次数
    int *freq = new int[N_CHAR];
    // 清零
    memset(freq, 0, sizeof(int) * N_CHAR);
    // assert: 文件存在且可正确打开
    FILE *fp = fopen(sample_text_file, "r");
    // 逐个扫描样本文件中的每个字符
    for (char ch; 0 < fscanf(fp, "%c", &ch);) {
        // 累计对应的出现次数
        if (ch >= 0x20) {
            freq[ch - 0x20]++;
        }
    }
    fclose(fp);
    return freq;
}

// 统计字符出现频率
int *statisticsBySequence(const char *sequence) {
    // 以下统计需随机访问，故以数组记录各字符出现次数
    int *freq = new int[N_CHAR];
    // 清零
    memset(freq, 0, sizeof(int) * N_CHAR);
    // assert: 文件存在且可正确打开
    // 逐个扫描样本文件中的每个字符
    char ch;
    for (int i = 0; i < strlen(sequence); i++) {
        // 累计对应的出现次数
        if ((ch = sequence[i]) >= 0x20) {
            freq[ch - 0x20]++;
        }
    }
    return freq;
}

// 根据频率统计表，为每个字符创建一棵树
HuffForest *initForest(int *freq) {
    // 以List实现的Huffman森林
    auto *forest = new HuffForest;
    // 为每个字符
    for (int i = 0; i < N_CHAR; i++) {
        // 生成一棵树，并将字符及其频率
        forest->insertAsLast(new HuffTree);
        // 存入其中
        forest->last()->data->insertAsRoot(HuffChar(0x20 + i, freq[i]));
    }
    return forest;
}

//通过遍历获取各字符的编码
static void generateCT(Bitmap *code, int length, HuffTable *table, BinNodePosi (HuffChar)v) {
    // 若是叶节点（还有多种方法可以判断）
    if (IsLeaf (*v)) {
        table->put(v->data.ch, code->bits2string(length));
        return;
    }
    // Left = 0
    if (HasLChild (*v)) {
        code->clear(length);
        generateCT(code, length + 1, table, v->lc);
    }
    // Right = 1
    if (HasRChild (*v)) {
        code->set(length);
        generateCT(code, length + 1, table, v->rc);
    }
}

// 将各字符编码统一存入以散列表实现的编码表中
HuffTable *generateTable(HuffTree *tree) {
    auto *table = new HuffTable;
    auto *code = new Bitmap;
    generateCT(code, 0, table, tree->root());
    release(code);
    return table;
} //release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包

#endif
