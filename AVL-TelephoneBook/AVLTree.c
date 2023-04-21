//AVLTree.c -AVL树ADT基本代码
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "AVLTree.h"
#include "Format.h"

//////////

/*局部函数声明*/
static bool ToLeft(const Item * i1, const Item * i2);
static bool ToRight(const Item * i1, const Item * i2);
static Pair RecuSeek(const Item * pi, Pair look);
static Trnode * MakeNode(const Item * pi);
static Trnode * AddNode(Trnode * new_node, Trnode * root);
static Trnode * Rotation(Pair current);
static int GetHeight(const Trnode * root);
static int GetHeightFactor(const Trnode * root);
static void ChangeHeight(Trnode * root, int val);
static Trnode * RightRotate(Trnode * root);
static Trnode * LeftRotate(Trnode * root);
static void InOrder(const Trnode * parent, void(*pfun)(Item item));
static void DelAllNodes(Trnode * root);
static void DeleteNode(Trnode **ptr);
static bool ShowTreeStr(Trnode * root, int indent, FILE * fp);
static int Islegal(Item * pi);
static bool PrintDESC(const Trnode * root, FILE * fp);
static bool PrintASC(const Trnode * root, FILE * fp);
#define MAX(a, b) (a) > (b) ? (a) : (b)
/*函数代码*/

Tree * InitializeTree(void)
{
    Tree *ptree;
    ptree = (Tree *) malloc(1 * sizeof(Tree));
    ptree->root = NULL;
    ptree->size = 0;
    return ptree;
}

bool IsTreeEmpty(const Tree * ptree)
{
    if(ptree->root == NULL)
        return true;
    else
        return false;
}

bool IsTreeFull(const Tree * ptree)
{
    if(ptree->size >= MAXITEM)
        return true;
    else
        return false;
}

int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}

bool InTree(const Item * pi, const Tree * ptree)
{
    return (SeekItem(pi, ptree).child == NULL) ? false : true;
}

//节点导航
static bool ToLeft(const Item * i1, const Item * i2)
{
    int comp1;
    if((comp1 = strcmp(i1->name, i2->name)) < 0)
        return true;
    else
        return false;
}

static bool ToRight(const Item * i1, const Item * i2)
{
    int comp1;
    if((comp1 = strcmp(i1->name, i2->name)) > 0)
        return true;
    else
        return false;
}

//搜索节点(递归方法)
Pair SeekItem(const Item * pi, const Tree * ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;
    if(look.child == NULL)
        return look;
    else
        return RecuSeek(pi, look);
}

static Pair RecuSeek(const Item * pi, Pair look)
{
    if(look.child == NULL)
        return look;
    else if(ToLeft(pi, &(look.child->item)))
    {
        look.parent = look.child;
        look.child = look.child->left;
        return RecuSeek(pi, look);
    }
    else if(ToRight(pi, &(look.child->item)))
    {
        look.parent = look.child;
        look.child = look.child->right;
        return RecuSeek(pi, look);
    }
    else 
        return look;
}

//插入节点
bool AddItem(const Item * pi, Tree * ptree)
{
    Trnode * new_node;
    if(IsTreeFull(ptree))
    {
        fprintf(stderr, "MEMORY FULL\n");
        return false;
    }
    if(SeekItem(pi, ptree).child != NULL)
    {
        fprintf(stderr, "DUPLICATE DATE\n");
        return false;
    }
    //用MakeNode函数创建节点并将new_node指向新节点
    new_node = MakeNode(pi);
    if(new_node == NULL)
    {
        fprintf(stderr, "CANNOT CRATE A NODE");
        return false;
    }
    if(ptree->root == NULL)
        ptree->root = new_node;
    else
        ptree->root = AddNode(new_node, ptree->root);
    ptree->size++;
    return true;
}

static Trnode * MakeNode(const Item * pi)
{
    Trnode * new_node;
    new_node = (Trnode *) malloc(sizeof(Trnode));
    if(new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 0; //节点高度初始化为0
    }
    return new_node;
}

static Trnode * AddNode(Trnode * new_node, Trnode * root)
{   
    Pair current;
    current.parent = root;
    new_node->height = root->height + 1;  
    if(ToLeft(&new_node->item, &root->item))
    {  
        if(root->left == NULL)
        {
            root->left = new_node;
            current.child = root->left;
        }
        else
        {
            current.child = AddNode(new_node, root->left);//递归查找子树
            current.parent->left = current.child;
        }
    }
    else if(ToRight(&new_node->item, &root->item))
    {
        if(root->right == NULL)
        {
            root->right = new_node;
            current.child = root->right;
        }
        else
        {
            current.child = AddNode(new_node, root->right);//递归查找子树
            current.parent->right = current.child;
        }
    }
    else
    {
        fprintf(stderr, "FAIL TO ADD A NODE");
        exit(EXIT_FAILURE);
    }
    //添加完成后检测父节点高度因子,从下到上逐次旋转
    //高度因子为左最长子树高度-右,若为正数则R或LR,若为负数则L或RL
    return Rotation(current);
}

//执行旋转操作,返回新的根节点
static Trnode * Rotation(Pair current)
{
    int Rotate = GetHeightFactor(current.parent);
    if(Rotate > 1)
    {   
        if(GetHeightFactor(current.child) < 0)
            current.parent->left = LeftRotate(current.child);
        return RightRotate(current.parent);
    }
    else if(Rotate < -1)
    {   
        if(GetHeightFactor(current.child) > 0)
            current.parent->right = RightRotate(current.child);
        return LeftRotate(current.parent);
    }
    else
        return current.parent;
}

static int GetHeight(const Trnode * root)
{   
    if(root == NULL)
        return 0;
    if(root->left == NULL && root->right == NULL)
        return root->height;
    else if(root->left == NULL && root->right != NULL)
        return GetHeight(root->right);
    else if(root->left != NULL && root->right == NULL)
        return GetHeight(root->left);
    else
        return MAX(GetHeight(root->left), GetHeight(root->right));
}

static int GetHeightFactor(const Trnode * root)
{   
    if(root == NULL)
        return 0;
    else if(root->left == NULL && root->right != NULL)
        return root->height - GetHeight(root->right);
    else if(root->left != NULL && root->right == NULL)
        return GetHeight(root->left) - root->height;
    else
        return GetHeight(root->left) - GetHeight(root->right);
}

static void ChangeHeight(Trnode * root, int val)
{
    if(root != NULL)
    {
        ChangeHeight(root->left, val);
        root->height += val;
        ChangeHeight(root->right, val);
    }
}

static Trnode * RightRotate(Trnode * root)
{   
    Trnode * new_root = NULL;
    new_root = root->left;
    //左子树高度全部减1,右子树和根节点高度全部加1
    ChangeHeight(root->left, -1);
    root->height += 1;
    ChangeHeight(root->right, 1);
    //移动指针
    root->left = (new_root->right == NULL) ? NULL : new_root->right;
    new_root->right = root;
    return new_root;
}

static Trnode * LeftRotate(Trnode * root)
{   
    Trnode * new_root = NULL;
    new_root = root->right;
    //右子树高度全部减1,左子树和根节点高度全部加1
    ChangeHeight(root->right, -1);
    root->height += 1;
    ChangeHeight(root->left, 1);
    //移动指针
    root->right = (new_root->left == NULL) ? NULL : new_root->left;
    new_root->left = root;
    return new_root;
}

//中序遍历-默认按首字母升序
void Traverse(const Tree * ptree, void (*pfun)(Item item))
{
    if(ptree != NULL)
        InOrder(ptree->root, pfun);
}

static void InOrder(const Trnode * parent, void(*pfun)(Item item))
{
    if(parent != NULL)
    {
        InOrder(parent->left, pfun);
        (*pfun)(parent->item);
        InOrder(parent->right, pfun);
    }
}

//完全清空树
void DelAll(Tree * ptree)
{   
    if(ptree != NULL)
        DelAllNodes(ptree->root);
    else
        return;
    ptree->root = NULL;
    ptree->size = 0;
    free(ptree);
}

static void DelAllNodes(Trnode * root)
{
    //中序遍历清空项
    Trnode * pright;
    if(root != NULL)
    {
        pright = root->right;
        DelAllNodes(root->left);
        free(root);
        DelAllNodes(pright);
    }
}

//删除某一元素
bool DelItem(const Item * pi, Tree * ptree)
{
    Pair look;
    look = SeekItem(pi, ptree);
    if(look.child == NULL)
        return false;
    if(look.parent == NULL)//根节点情形
        DeleteNode(&ptree->root);
    else if(look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;
    //检查是否平衡
    if(Rotation(look) != NULL)
        return true;
    else
        return false;
}

static void DeleteNode(Trnode **ptr)
{
    Trnode * temp;
    if((*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else
    //此时被删除的节点有两个子节点
    //在被删除节点的右支树中找到最近的空位并连上去
    {
        for(temp = (*ptr)->left; temp->right != NULL; temp = temp ->right)
            continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}

bool TreeWritef(const Tree * ptree, FILE * fp, const char * order)
{
    if(ptree == NULL)
    {
        return false;
        fprintf(stderr, "TREE DOES NOT EXISTS");
    }
    if(strcmp(order, "DESC"))
        return PrintDESC(ptree->root, fp);
    if(strcmp(order, "ASC"))
        return PrintASC(ptree->root, fp);
    return false;
}

static bool PrintASC(const Trnode * root, FILE * fp)
{   
    if(root != NULL)
    {
        PrintASC(root->right, fp);
        fprintf(fp, "Item:\n");
        PrintItem(root->item, fp);
        PrintASC(root->left, fp);
    }
}

static bool PrintDESC(const Trnode * root, FILE * fp)
{   
    if(root != NULL)
    {
        PrintDESC(root->left, fp);
        fprintf(fp, "Item:\n");
        PrintItem(root->item, fp);
        PrintDESC(root->right, fp);
    }
}

bool PrintItem(const Item item, FILE * fp)
{
    fprintf(fp, "NAME:\t%s\n", item.name);
    fprintf(fp, "\tPHONE:\t\t%s\n", item.phone);
    fprintf(fp, "\tWORKPLACE:\t%s\n", item.workplace);
    fprintf(fp, "\tADDRESS:\t%s\n\n", item.address);

    return true;
}

bool TreeReadf(Tree * ptree, FILE * fp) 
{
    char line[MAXLINE + 50] = {'\0'};//这里挺容易溢出的
    Item item;
    int count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) 
    {   // 逐行读取文件内容
        int i = 0;
        while(line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
                ++i;//跳过空白符
        if (line[i] == 'N' && line[i + 1] != 'o')
            sscanf(line + i, "NAME: %s", item.name); 
        else if (line[i] == 'P') 
            sscanf(line + i, "PHONE: %s", item.phone); 
        else if (line[i] == 'W')
            sscanf(line + i, "WORKPLACE: %s", item.workplace);
        else if (line[i] == 'A')
        {
            sscanf(line + i, " ADDRESS: %s", item.address); 
            if(!AddItem(&item, ptree))
                return false;
            ++count;
        }
        else
            continue;
    }
    fprintf(stdout, "Success to read %d piece(s) from "DATABASE"\n", count);
    return true;
}

bool TreeReadOne(Tree * ptree)
{
    if(ptree == NULL)
    {
        return false;
        fprintf(stderr, "TREE DOES NOT EXISTS");
    }
    Item * pi = (Item *) malloc(sizeof(Item));
    fputs("Enter the item data.\"RE_\" to restart.\n", stdout);
    const char *filed[] = {"EMPTY", "name", "phone", "workplace", "address"};
    int i = 1;
    for(i = 1; i <= 4; ++i)
    {
        fprintf(stdout, "%s:", *(filed+i));        
        char temp[MAXLINE] = {'\0'};
        fscanf(stdin, "%s", temp);
        if(strcmp(temp, "RE_") == 0)
        {
            i = 0;
            continue;
        }
        if(i == 1)
            strcpy(pi->name, temp);
        if(i == 2)
            strcpy(pi->phone, temp);
        if(i == 3)
            strcpy(pi->workplace, temp);
        if(i == 4)
            strcpy(pi->address, temp);
    }
    int wrong = Islegal(pi);
    if(wrong > 0)
    {
        fprintf(stdout, "ILLEGAL INPUT OF _%s_ TRY AGAIN\n\n", filed[wrong]);
        TreeReadOne(ptree);
    }
    else if(!AddItem(pi, ptree))
    {
        fprintf(stderr, "FAIL TO INSERT\n");
        free(pi);
        return false;
    }
    free(pi);
    fprintf(stdout,"\nDone!\n");
    return true;
}

//检测输入是否合法,若有错误则返回错误项标号,否则返回0
static int Islegal(Item * pi)
{
    if(strlen(pi->phone) != 11)
        return 2;
    for(int i = 0; i < 11; ++i)
        if((pi->phone)[i] < '0' || (pi->phone)[i] > '9')
            return 2;
    return 0;
    //目前只要求检测电话号码的合法性
}

bool ShowTree(Tree *ptree, FILE * fp)
{
    if(ptree == NULL)
    {
        return false;
        fprintf(stderr, "TREE DOES NOT EXISTS");
    }
    else if(!ShowTreeStr(ptree->root, 0, fp))
        fprintf(stdout, "TREE IS EMPTY");
    return true;
}

static bool ShowTreeStr(Trnode * root, int indent, FILE * fp)
{
if (root == NULL) {
        return false;
    }
    if (root->left != NULL) {
        ShowTreeStr(root->left, indent + 4, fp);
    }
    if (indent > 0) {
        fprintf(fp, "%*c", indent, ' ');
    }
    if (root->right != NULL) {
        fprintf(fp, "/\n");
        fprintf(fp, "%*c", indent, ' ');
        ShowTreeStr(root->right, indent + 4, fp);
    } else {
        fprintf(fp, "--");
    }
    fprintf(fp, "%-8s\n", root->item.name);
    return true;
}