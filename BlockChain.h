#ifndef __BlockChain
#define __BlockChain
#include <time.h>
typedef long long int lli;
typedef struct Time
{
    int hr;
    int min;
    int dd, mm, yyyy;
} Time;
typedef struct TransactionNode
{
    lli TransactionTo;
    lli TransactionFrom;
    lli Amount;
    Time t;
    struct TransactionNode *Next;
} Node;
typedef struct Transactions
{
    Node *Front;
    Node *Rear;
    int *num;
} Transactions;
typedef Transactions *List;
typedef struct Block
{
    char *PrevBlockHash;
    int Nonce;
    lli BlockNumber;
    struct Block *Next;
    List Transaction;
} Block;
typedef struct BlockList
{
    Block *Head;
    Block *Tail;
    lli *NumBlock;
} Chain;
typedef struct UserNode
{
    lli UserId;
    lli *WalletBalance;
    Transactions *History;
    // tm JoinDateTime;
} User;
typedef struct UserArray
{
    lli size;     //->current number of users
    lli capacity; //Max->size
    User **data;

} Record;
//FUNCTIONS FOR USER DATA_BASE
Record *CreateRecord(lli size);
User *CreateUserNode();
Record *ResizeRecord(Record *R);
void AddUser(Record *R);
//FUNCTIONS FOR CREATINGBLOCKCHAIN
Chain *CreateChain();
Block *CreateBlock(List TransactionHead);
void AddToChain(Chain *C, List Head);
//FUNCTIONS TO TRANSACTION LIST AND FOR UPDATING USER DATA
Node *CreateNode(lli To, lli From, lli Amount);
List CreateHead();
void TransactionHistory(Node *Temp, List L);
void AddTransactionToList(Record *R, List L, lli To, lli From, lli Amount);
void UpdateHistory(Record *R, lli To, lli From, lli Amount, Node *T);
int Check(Record *R, lli To, lli From, lli Amount);

int Transact(Record *R, lli To, lli From, lli Amount, List CurrList);
#endif
