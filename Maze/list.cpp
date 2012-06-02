#include "StdAfx.h"
#include "list.h"

int InitList(LinkList &L)
{ // 操作结果：构造一个空的线性表L
	L=(LinkList)malloc(sizeof(LNode)); // 产生头结点,并使L指向此头结点
	L->next=NULL; // 指针域为空
	return 1;
}


int DestroyList(LinkList &L)
{ // 初始条件：线性表L已存在。操作结果：销毁线性表L
	LinkList q;
	while(L)
	{
		q=L->next;
		free(L);
		L=q;
	}
	return 1;
}


int ClearList(LinkList L) // 不改变L
{ // 初始条件：线性表L已存在。操作结果：将L重置为空表
	LinkList p,q;
	p=L->next; // p指向第一个结点
	while(p) // 没到表尾
	{
		q=p->next;
		free(p);
		p=q;
	}
	L->next=NULL; // 头结点指针域为空
	return 1;
}

int ListEmpty(LinkList L)
{ // 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE
	if(L->next) // 非空
		return FALSE;
	else
		return TRUE;
}


int ListLength(LinkList L)
{ // 初始条件：线性表L已存在。操作结果：返回L中数据元素个数
	int i=0;
	LinkList p=L->next; // p指向第一个结点
	while(p) // 没到表尾
	{
		i++;
		p=p->next;
	}
	return i;
}


int GetElem(LinkList L,int i,POINT &e) // 算法2.8
{ // L为带头结点的单链表的头指针。当第i个元素存在时,其值赋给e并返回1,否则返回ERROR
	int j=1; // j为计数器
	LinkList p=L->next; // p指向第一个结点
	while(p&&j<i) // 顺指针向后查找,直到p指向第i个元素或p为空
	{
		p=p->next;
		j++;
	}
	if(!p||j>i) // 第i个元素不存在
		return ERROR;
	e=p->data; // 取第i个元素
	return 1;
}


int ListInsert(LinkList L,int i,POINT e) // 算法2.9。不改变L
{ // 在带头结点的单链线性表L中第i个位置之前插入元素e
	int j=0;
	LinkList p=L,s;
	while(p&&j<i-1) // 寻找第i-1个结点
	{
		p=p->next;
		j++;
	}
	if(!p||j>i-1) // i小于1或者大于表长
		return ERROR;
	s=(LinkList)malloc(sizeof(LNode)); // 生成新结点
	s->data=e; // 插入L中
	s->next=p->next;
	p->next=s;
	return 1;
}


int ListDelete(LinkList L,int i,POINT &e) // 算法2.10。不改变L
{ // 在带头结点的单链线性表L中，删除第i个元素,并由e返回其值
	int j=0;
	LinkList p=L,q;
	while(p->next&&j<i-1) // 寻找第i个结点,并令p指向其前趋
	{
		p=p->next;
		j++;
	}
	if(!p->next||j>i-1) // 删除位置不合理
		return ERROR;
	q=p->next; // 删除并释放结点
	p->next=q->next;
	e=q->data;
	free(q);
	return 1;
}