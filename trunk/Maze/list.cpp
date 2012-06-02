#include "StdAfx.h"
#include "list.h"

int InitList(LinkList &L)
{ // �������������һ���յ����Ա�L
	L=(LinkList)malloc(sizeof(LNode)); // ����ͷ���,��ʹLָ���ͷ���
	L->next=NULL; // ָ����Ϊ��
	return 1;
}


int DestroyList(LinkList &L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ�����������������Ա�L
	LinkList q;
	while(L)
	{
		q=L->next;
		free(L);
		L=q;
	}
	return 1;
}


int ClearList(LinkList L) // ���ı�L
{ // ��ʼ���������Ա�L�Ѵ��ڡ������������L����Ϊ�ձ�
	LinkList p,q;
	p=L->next; // pָ���һ�����
	while(p) // û����β
	{
		q=p->next;
		free(p);
		p=q;
	}
	L->next=NULL; // ͷ���ָ����Ϊ��
	return 1;
}

int ListEmpty(LinkList L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ������������LΪ�ձ��򷵻�TRUE�����򷵻�FALSE
	if(L->next) // �ǿ�
		return FALSE;
	else
		return TRUE;
}


int ListLength(LinkList L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ��������������L������Ԫ�ظ���
	int i=0;
	LinkList p=L->next; // pָ���һ�����
	while(p) // û����β
	{
		i++;
		p=p->next;
	}
	return i;
}


int GetElem(LinkList L,int i,POINT &e) // �㷨2.8
{ // LΪ��ͷ���ĵ������ͷָ�롣����i��Ԫ�ش���ʱ,��ֵ����e������1,���򷵻�ERROR
	int j=1; // jΪ������
	LinkList p=L->next; // pָ���һ�����
	while(p&&j<i) // ˳ָ��������,ֱ��pָ���i��Ԫ�ػ�pΪ��
	{
		p=p->next;
		j++;
	}
	if(!p||j>i) // ��i��Ԫ�ز�����
		return ERROR;
	e=p->data; // ȡ��i��Ԫ��
	return 1;
}


int ListInsert(LinkList L,int i,POINT e) // �㷨2.9�����ı�L
{ // �ڴ�ͷ���ĵ������Ա�L�е�i��λ��֮ǰ����Ԫ��e
	int j=0;
	LinkList p=L,s;
	while(p&&j<i-1) // Ѱ�ҵ�i-1�����
	{
		p=p->next;
		j++;
	}
	if(!p||j>i-1) // iС��1���ߴ��ڱ�
		return ERROR;
	s=(LinkList)malloc(sizeof(LNode)); // �����½��
	s->data=e; // ����L��
	s->next=p->next;
	p->next=s;
	return 1;
}


int ListDelete(LinkList L,int i,POINT &e) // �㷨2.10�����ı�L
{ // �ڴ�ͷ���ĵ������Ա�L�У�ɾ����i��Ԫ��,����e������ֵ
	int j=0;
	LinkList p=L,q;
	while(p->next&&j<i-1) // Ѱ�ҵ�i�����,����pָ����ǰ��
	{
		p=p->next;
		j++;
	}
	if(!p->next||j>i-1) // ɾ��λ�ò�����
		return ERROR;
	q=p->next; // ɾ�����ͷŽ��
	p->next=q->next;
	e=q->data;
	free(q);
	return 1;
}