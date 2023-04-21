//AVLTree.h -AVL树ADT原型
//树中不允许重复数据
//左子树的所有项都在根节点的前面,右子树的所有项都在根节点的后面
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "Format.h"

#ifndef MAXITEM
#define MAXITEM (50) //树中最多元素个数
#endif

#ifndef DATABASE
#define DATABASE "data.txt" //树中最多元素个数
#endif

/*定义结构*/

//二叉树节点结构体
typedef struct trnode
{
    Item item;
    int height;
    struct trnode * left;
    struct trnode * right;
} Trnode;

//二叉树访问结构体
typedef struct tree
{
    Trnode * root;
    int size;
} Tree;

//二叉查找结构体
typedef struct pair
{
    Trnode * parent;
    Trnode * child;
} Pair;

/*函数原型*/

//O:将树初始化为空
//P:NULL
//E:树被初始化为空,并返回一个指向Tree的指针
Tree * InitializeTree(void);

//O:确定树是否为空
//P:ptree指向一个Tree类型
//E:若为空,返回true,否则返回false
bool IsTreeEmpty(const Tree * ptree);

//O:确定树是否已满
//P:ptree指向一个Tree类型
//E:若为空,返回true,否则返回false
bool IsTreeFull(const Tree * ptree);

//O:确定树中的项数
//P:ptree指向一个Tree类型
//E:返回一个树的项数
int TreeItemCount(const Tree * ptree);

//O:在树中添加一个项
//P:pi指向待添加项的地址,ptree指向要添加到的树
//E:如果可以添加,则返回该节点及其父级
bool AddItem(const Item * pi, Tree * ptree);

//O:在树中删除一个项
//P:pi指向待删除项的地址,ptree指向要操作的树
//E:如果可以删除,则返回true,同时将该项从树中删除,否则返回false
bool DelItem(const Item * pi, Tree * ptree);

//O:在树中查找一个项
//P:pi指向待查找项的地址,ptree指向要操作的树
//E:如果查找到,则返回true,否则返回false
bool InTree(const Item * pi, const Tree * ptree);

//O:将函数应用于树中的每一项
//P:ptree指向一个Tree类型,pfun指向一个函数(该函数接受一个Item类型的变量)
//E:将pfun指向的函数应用于ptree指向的树中的所有值
void Traverse(const Tree * ptree, void (*pfun)(Item item));

//O:清空树
//P:ptree指向一个Tree类型
//E:ptree指向的Tree被清空
void DelAll(Tree * ptree);

//O:将通讯录写入指定文件流
//P:ptree指向一个Tree类型,File指向一个文件
//E:ptree指向的Tree中的每一个Item以指定顺序写入文件流
bool TreeWritef(const Tree * ptree, FILE * fp, const char * order);

//O:从指定文件流中读取一个树
//P:ptree指向一个Tree类型,File指向一个文件
//E:ptree指向的Tree中的每一个Item以指定顺序写入文件流并自平衡
bool TreeReadf(Tree * ptree, FILE * fp);

//O:在交互页面(标准输入流中)读取树的一个元素
//P:ptree指向一个Tree类型
//E:ptree指向的树中的Item写入文件流并自平衡
bool TreeReadOne(Tree * ptree);

//O:在输入流中显示二叉树结构
//P:ptree指向一个Tree类型,fp是一个文件指针
//E:ptree指向的树以图表方式显示在fp指向的文件流中,以name为标签
bool ShowTree(Tree *ptree, FILE * fp);

//O:在指定文件流中输出一个Item结构
//P:fp是一个文件指针
//E:在fp指向的文件中格式化打印了一个item中的数据
bool PrintItem(const Item item, FILE * fp);

//O:在树中搜索节点
//P:pi指向一个Item结构,ptree指向一个Tree结构
//O:值为pi指向的值的节点,返回以这个节点为子节点的Pair结构
Pair SeekItem(const Item * pi, const Tree * ptree);