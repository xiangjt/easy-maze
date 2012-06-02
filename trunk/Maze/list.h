struct LNode
{
	POINT data;
	LNode *next;
};
typedef LNode *LinkList; // ��һ�ֶ���LinkList�ķ���

int InitList(LinkList &L);
int DestroyList(LinkList &L);
int ClearList(LinkList L);
int ListEmpty(LinkList L);
int ListLength(LinkList L);
int GetElem(LinkList L,int i,POINT &e);
int ListInsert(LinkList L,int i,POINT e);
int ListDelete(LinkList L,int i,POINT &e) ;