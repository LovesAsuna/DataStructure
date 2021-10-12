#include <iostream>
#include <string>
#include "huffman/HuffCode.h"
#include <cstdio>

using namespace std;

// 哈夫曼树文件
FILE *treeFile = nullptr;
// 哈夫曼树
HuffTree *tree = nullptr;
// 哈夫曼编码长度
int length = 0;

HuffTree *init() {
    int n;
    cout << "请输入字符集大小n: " << endl;
    cin >> n;
    cin.get();
    char ch;
    int weight;
    // 以下统计需随机访问，故以数组记录各字符出现次数
    int *freq = new int[N_CHAR];
    // 清零
    memset(freq, 0, sizeof(int) * N_CHAR);
    while (n--) {
        cout << "请输入字符: " << endl;
        cin.get(ch);
        cin.get();
        cout << "请输入权值(频率): " << endl;
        cin >> weight;
        cin.get();
        freq[ch - 0x20] = weight;
    }

    // 创建Huffman森林
    HuffForest *forest = initForest(freq);
    // 生成Huffman编码树
    // 哈夫曼树
    tree = generateTree(forest);
    release(forest);
    treeFile = fopen("../hfmTree.txt", "w");
    fwrite(freq, sizeof(int), N_CHAR, treeFile);
    delete[]freq;
    fclose(treeFile);
    return tree;
}

int encoding() {
    treeFile = fopen("../hfmTree.txt", "r");
    // 文件存在且哈夫曼编码树不存在
    if (treeFile && !tree) {
        cout << "哈夫曼编码树已存在，将自动读入！" << endl;
        int *freq = new int[N_CHAR];
        treeFile = fopen("../hfmTree.txt", "r");
        fread(freq, sizeof(int), N_CHAR, treeFile);
        fclose(treeFile);
        // 创建Huffman森林
        HuffForest *forest = initForest(freq);
        release(freq);
        // 生成Huffman编码树
        // 哈夫曼树
        tree = generateTree(forest);
    }
    // 将Huffman编码树转换为编码表
    HuffTable *table = generateTable(tree);

    // 正文文件
    FILE *fp = fopen("../ToBeTran.txt", "r");

    // 二进制编码串
    auto codeString = new Bitmap;

    // 逐个扫描样本文件中的每个字符
    string line;
    for (char c = 0; 0 < fscanf(fp, "%c", &c);) {
        // 累计对应的出现次数
        if (c >= 0x20) {
            line += c;
        }
    }

    // 哈夫曼编码长度
    length = encode(table, codeString, line.data());
    cout << "哈夫曼编码长度为: " << length << endl;
    // 将编码结果写出到文件
    codeString->dump("../CodeFile.txt");
    return length;
}

void decoding() {
    treeFile = fopen("../hfmTree.txt", "r");
    // 文件存在且哈夫曼编码树不存在
    if (treeFile && !tree) {
        cout << "哈夫曼编码树已存在，将自动读入！" << endl;
        int *freq = new int[N_CHAR];
        treeFile = fopen("../hfmTree.txt", "r");
        fread(freq, sizeof(int), N_CHAR, treeFile);
        fclose(treeFile);
        // 创建Huffman森林
        HuffForest *forest = initForest(freq);
        release(freq);
        // 生成Huffman编码树
        // 哈夫曼树
        tree = generateTree(forest);
    }
    cout << "请输入哈夫曼编码长度: " << endl;
    cin >> length;
    auto code = new Bitmap("../CodeFile.txt", length);
    string result = decode(tree, code, length);
    auto resultFile = fopen("../TextFile.txt", "w");
    for (char i : result) {
        fputc(i, resultFile);
    }
    fclose(resultFile);
}

int homeowrk() {
    char function;
    bool quit = false;
    while (!quit) {
        cout << "================================" << endl;
        cout << "I：初始化" << endl;
        cout << "E：编码" << endl;
        cout << "D：译码" << endl;
        cout << "Q：退出" << endl;
        cout << "================================" << endl;
        cout << "请输入对应的命令" << endl;
        cin >> function;
        switch (function) {
            case 'I':
                init();
                break;
            case 'E':
                encoding();
                break;
            case 'D':
                decoding();
                cout << endl;
                break;
            case 'Q':
                quit = true;
                break;
            default:
                break;
        }
    }
    return 0;
}

