//Format.h -电话簿信息结构定义
#pragma once
#define MAXLINE 20
#define PHONENUMBERLINE 11 + 2
//#define _DEBUG_MODE_ _DEBUG_MODE_ //debug模式开关-只使用name字段

#ifndef _DEBUG_MODE_
typedef struct item
{
    char name[MAXLINE];
    char phone[PHONENUMBERLINE];
    char workplace[MAXLINE];
    char address[MAXLINE];
} Item;

#else
typedef struct item
{
    char name[MAXLINE];
} Item;

#endif