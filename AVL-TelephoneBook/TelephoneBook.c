//TelephoneBook.c -程序主函数&图形化界面
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "AVLTree.h"
#include "Format.h"
//#define DATABASE diyname

//局部函数声明
static int ShowGUI(void);
static bool ShowAllContacts(Tree * ptree);
static bool UpdateContact(Tree * ptree, char *phone);
static int IsContact(const Trnode * root, char *name, FILE * fp);

int main(void)//日后可加启动参数
{
    printf("Initializing GUI.....Done!\n");

    FILE * fpr = fopen(DATABASE, "r");
    FILE * fpw = NULL;
    Tree * ptree = InitializeTree();
    char line[MAXLINE] = {'\0'};
    if(!TreeReadf(ptree, fpr))
    {
        fprintf(stderr, "FAIL TO READ FROM FILE\n");
        exit(EXIT_FAILURE);
    }
    int choice = 7;
    while(true)
    {   
        printf("\n\nPress Enter....");
        getchar();
        choice = ShowGUI();
        if(choice == 0)
        {
            fprintf(stderr, "FAIL TO SHOW GUI\n");
            exit(EXIT_FAILURE);
        }
        switch (choice)
        {
        case 1:
            if(!ShowAllContacts(ptree))
                printf("FAIL TO SHOW ALL CONTACTS");
            break;
        case 2:
            printf("\nEnter the contact's phone to search: ");
            scanf("%s", line);getchar();
            if(IsContact(ptree->root, line, stdout) == 0)
                printf("\nNo data found.\n");
            break;
        case 3:
            printf("\nEnter the contact's name to search: ");
            scanf("%s", line);getchar();
            UpdateContact(ptree, line);
            break;
        case 4:
            printf("\nStart adding :\n");
            char ch = 'Y';
            while(true)
            {
                TreeReadOne(ptree);
                printf("\nContinue to add? (Y/N) :");
                getchar();scanf("%c", &ch);getchar();
                if(ch == 'Y')
                    continue;
                else
                    break;
            }
            break;
        case 5:
            printf("\nEnter the contact's name to delete: ");
            scanf("%s", line);getchar();
            Item item;
            strcpy(item.name, line);
            Trnode *node;
            node = SeekItem(&item, ptree).child;
            if(node == NULL)
            {
                printf("\nFind no data.\n");
                break;
            }
            else
                DelItem(&(node->item), ptree);
            printf("\nDeleted.\n");
            break;
        case 6:
            printf("\nCurrent number in phone book : %d \n", TreeItemCount(ptree));
            break;
        case 7:
            fpw = fopen(DATABASE, "w");//清空文件
            DelAll(ptree);
            break;
        case 8:
            fpw = fopen(DATABASE, "w");
            TreeWritef(ptree, fpw, "ASC"); //默认升序写入
            DelAll(ptree);
            return 0;
            break;
        case 114514:
            ShowTree(ptree, stdout);
            getchar();
            break;
        default:
            printf("Wrong choice!Try again.\n");
            break;
        }
    }
    return 0;
}

static int ShowGUI(void)
{
    printf("\n**************************************\n");
    printf("*   1. Show all contacts' info       *\n");
    printf("*   2. Inquire contacts' info        *\n");
    printf("*   3. Update contacts' info         *\n");
    printf("*   4. Add a contacts                *\n");
    printf("*   5. Delete a contact              *\n");
    printf("*   6. Counting number of contacts   *\n");
    printf("*   7. Delete the telephone book     *\n");
    printf("*   8. Exit                          *\n");
    printf("**************************************\n");
    printf("Enter a number to make your choice: ");

    int n;
    scanf("%d", &n);
    return n;
}

static bool ShowAllContacts(Tree * ptree)
{
    char order = 0;
    while(true)
    {
        printf("\nChoose the order DESC or ASC (D/A): ");
        getchar();scanf("%c", &order);getchar();
        if(order == 'D')
        {
            TreeWritef(ptree, stdout, "DESC");
            return true;
        }
        else if(order == 'A')
        {
            TreeWritef(ptree, stdout, "ASC");
            return true;
        }
        printf("WRONG ORDER (A/D)\n");
    }
    return false;
}

static int IsContact(const Trnode * root, char *phone, FILE * fp)
{   
    static int count = -1;
    if(root != NULL)
    {
        IsContact(root->left, phone, fp);
        if(!strcmp((root->item.phone), phone))
            PrintItem(root->item, fp);
        IsContact(root->right, phone, fp);
    }
    ++count;
    return count;
}

static bool UpdateContact(Tree * ptree, char *name)
{   
    Item item;
    strcpy(item.name, name);
    Trnode *node = SeekItem(&item, ptree).child;
    if(node == NULL)
    {
        printf("\nFind no data. Add a contact?");
        return false;
    }
    else
    {
        DelItem(&(node->item), ptree);
        printf("\nnew: \n");
        TreeReadOne(ptree);
    }
    return true;
}