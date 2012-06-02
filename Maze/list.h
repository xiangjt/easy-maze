struct LNode
{
	POINT data;
	LNode *next;
};
typedef LNode *LinkList; // 另一种定义LinkList的方法

int InitList(LinkList &L);
int DestroyList(LinkList &L);
int ClearList(LinkList L);
int ListEmpty(LinkList L);
int ListLength(LinkList L);
int GetElem(LinkList L,int i,POINT &e);
int ListInsert(LinkList L,int i,POINT e);
int ListDelete(LinkList L,int i,POINT &e) ;