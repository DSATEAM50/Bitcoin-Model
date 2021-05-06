#include <stdio.h>
#include <stdlib.h>
#include "Blockchain.h"
#define HASHSIZE 10
Record *CreateRecord(lli size)
{
    Record *R = (Record *)malloc(size * sizeof(Record));
    R->size = 0;
    R->capacity = size;
    R->data = (User **)malloc(size * sizeof(User *));
    for (int i = 0; i < size; i++)
    {
        R->data[i] = NULL;
    }
    return R;
}
User *CreateUserNode()
{
    User *U = (User *)malloc(sizeof(User));
    U->History = NULL;
    U->WalletBalance = (lli *)malloc(sizeof(lli));
    return U;
}
Record *ResizeRecord(Record *R)
{
    Record *New = CreateRecord(2 * R->capacity);
    New->size = R->capacity;
    for (int i = 0; i < R->capacity; i++)
    {
        New->data[i] = CreateUserNode();
        New->data[i] = R->data[i];
        free(R->data[i]);
    }
    return New;
}
void AddUser(Record *R)
{
    lli i = R->size;
    if (i >= R->capacity)
    {
        R = ResizeRecord(R);
    }
    R->size += 1;
    R->data[i] = CreateUserNode();
    *(R->data[i]->WalletBalance) = 1000;
    R->data[i]->UserId = i;
}
Chain *CreateChain()
{
    Chain *L = (Chain *)malloc(sizeof(Chain));
    L->Head = NULL;
    L->Tail = NULL;
    L->NumBlock = (lli *)malloc(sizeof(lli));
    *(L->NumBlock) = 0;
    return L;
}
Block *CreateBlock(List TransactionHead)
{
    Block *B = (Block *)malloc(sizeof(Block));
    B->Next = NULL;
    B->Transaction = TransactionHead;
    return B;
}
void AddToChain(Chain *C, List Head)
{
    if (*(C->NumBlock) == 0)
    {
        Block *B = CreateBlock(Head);
        B->Nonce = (rand() % 500) + 1;
        B->BlockNumber = 1;
        B->PrevBlockHash = 0;
        C->Head = B;
        C->Tail = B;
        *(C->NumBlock) = 1;
    }
    else
    {
        Block *B = CreateBlock(Head);
        B->BlockNumber = *(C->NumBlock) + 1;
        *(C->NumBlock) += 1;
        B->Nonce = (rand() % 500) + 1;
        B->PrevBlockHash = (char *)malloc(sizeof(char) * HASHSIZE);
        // *****B->PrevBlockHash = HASH(C->Tail);********
        C->Tail->Next = B;
        C->Tail = B;
    }
}
Node *CreateNode(lli To, lli From, lli Amount)
{
    Node *T = (Node *)malloc(sizeof(Node));
    T->TransactionFrom = From;
    T->TransactionTo = To;
    T->Amount = Amount;
    return T;
}
List CreateHead()
{
    List L = (List)malloc(sizeof(Transactions));
    L->num = (int *)malloc(sizeof(int));
    L->Front = NULL;
    L->Rear = NULL;
    *(L->num) = 0;
}
void TransactionHistory(Node *Temp, List L)
{
    if (L->Front == NULL)
    {
        L->Front = Temp;
        L->Rear = Temp;
        *(L->num) = 1;
    }
    else
    {
        L->Rear->Next = Temp;
        L->Rear = Temp;
        *(L->num) += 1;
    }
}
void UpdateHistory(Record *R, lli To, lli From, lli Amount, Node *T)
{
    //update recievers data
    *(R->data[To]->WalletBalance) = *(R->data[To]->WalletBalance) + Amount;
    TransactionHistory(T, R->data[To]);

    //update sender's data
    *(R->data[From]->WalletBalance) = *(R->data[From]->WalletBalance) - Amount;
    TransactionHistory(T, R->data[From]);
}
void AddTransactionToList(Record *R, List L, lli To, lli From, lli Amount)
{
    Node *Temp = CreateNode(To, From, Amount);
    if (L->Front == NULL)
    {
        L->Front = Temp;
        L->Rear = Temp;
        *(L->num) = 1;
    }
    else
    {
        L->Rear->Next = Temp;
        L->Rear = Temp;
        *(L->num) += 1;
    }
    UpdateHistory(R, To, From, Amount, Temp);
}
int Check(Record *R, lli To, lli From, lli Amount)
{
    //returns 0 if user not present
    //return -1 if user present but insufficient balance
    //returns 1 if user is present AND Sufficient Balance
    if (!(R->size > To && R->size > From))
        return 0;
    else
    {
        if (R->data[From]->WalletBalance < Amount)
            return -1;
    }
    return 1;
}
int Transact(Record *R, lli To, lli From, lli Amount, List CurrList)
{
    int x = Check(R, To, From, Amount);
    if (x != 1)
        return x;
    AddTransactionToList(R, CurrList, To, From, Amount);
    return 1;
}
