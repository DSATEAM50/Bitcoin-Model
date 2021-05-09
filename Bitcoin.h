#ifndef __BlockChain
#define __BlockChain
#include <time.h>
#define HASHSIZE 32 //size of the block hash//
typedef long long int lli;
//Transactions stored as a linked list
typedef struct TransactionNode
{
    lli TransactionTo;   //Reciever
    lli TransactionFrom; //sender
    lli Amount;
    time_t TIME; //time stamp of the transaction
    struct TransactionNode *Next;
} Node;
typedef struct Transactions //struct for Transaction List Head
{
    Node *Front; //pointer to the frint transaction node
    Node *Rear;
    int *num; //no: of transactions that have been added in the list
} Transactions;
typedef Transactions *List;
typedef struct Block
{
    char *PrevBlockHash;
    int *Nonce;
    lli BlockNumber;
    struct Block *Next;
    List Transaction;
} Block;
typedef struct BlockList //Block header struct
{
    Block *Head;   //pointer to the front blockc in the list of blocks
    Block *Tail;   //pointer to last added block in the list of blocks
    lli *NumBlock; //number of blocks that have been added in the block chain
} Chain;
//User represented asAVL Tree
typedef struct UserNode
{
    lli UserId; //Unique Id for each user
    lli *WalletBalance;
    List History;           //User transaction history
    struct UserNode *Left;  //Left Child of USer
    struct UserNode *Right; //Right Child of USer
    lli Height;
    time_t JoinTime; //STORES USER JOINING TIME STAMP//
} UserNode;
typedef UserNode *AVLuser;
typedef AVLuser Position;
//Functions for storing User Data
lli max(lli a, lli b);
lli Height(Position P);
AVLuser CreateUserNode(lli Id);
Position SingleRotateWithLeft(Position k2);
Position SingleRotateWithRight(Position k2);
Position DoubleRotateWithLeft(Position k2);
Position DoubleRotateWithRight(Position k2);
UserNode *AddUser(AVLuser Tree, lli Id);
UserNode *Search(AVLuser root, lli SearchId);
void PrintUserInfo(AVLuser root, lli Id);
void PRINTUSERDETAILS(AVLuser root, lli Id);
//Functions for Block Chain
char *CreateHash(Block *B);
lli HashTransaction(List L);
lli HornerRule(char *PrevBlockHash, int nonce);
Chain *CreateChain();
Block *CreateBlock(List TransactionHead);
void AddToChain(Chain *C, List Head);
//Functions for Transactions
//FUNCTIONS TO TRANSACTION LIST AND FOR UPDATING USER DATA
Node *CreateNode(lli To, lli From, lli Amount, time_t t);
List CreateHead();
void UpdateHistory(AVLuser root, lli To, lli From, lli Amount, time_t t);
void AddTransactionToList(List L, lli To, lli From, lli Amount, time_t t);
int Transact(AVLuser root, lli To, lli From, lli Amount, time_t t); //returns 1 for a valid transaction else 0
void PrintTransactionList(List L);
int Attack(Chain *mylist);
int Validate_BlockChain(Block *startingBlock, int *NumAttacks);

#endif
