#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Bitcoin.h"
//UTILITY HASH FUNCTION==>SUB-ROUTINE FUNCTION FOR CALCULATING THE BLOCK HASH//
lli HornerRule(char *string, int nonce)
{
    nonce = 1009 * nonce;
    //HORNER'S RULE HASH FUNCTION IMPLEMENTATION FOR STRINGS//
    lli hashvalue = 0;
    lli string_length = HASHSIZE;
    for (int i = 0; i < string_length; i++)
    {
        hashvalue = 1009 * hashvalue + string[i];
    }
    hashvalue %= nonce;
    if (hashvalue < 0) //incase of overflow//
        hashvalue += nonce;

    return hashvalue;
}
//UTILITY HASH FUNCTION==>SUB-ROUTINE FUNCTION FOR CALCULATING THE BLOCK HASH//
lli HashTransaction(List L)
{
    lli count = *(L->num);
    lli amount = 0;
    Node *Temp = L->Front;
    while (count != 0)
    {
        amount = amount + (Temp->TransactionFrom + Temp->TransactionTo) % (Temp->Amount);
        Temp = Temp->Next;
        count--;
    }
    amount = amount % 1009;
    return amount;
}
//HASH FUNCTION==>MAIN FUNCTION FOR CALCULATING THE BLOCK HASH//
char *CreateHash(Block *B)
{
    char *BlockHash = (char *)malloc(HASHSIZE * sizeof(char));
    //Initialised the hash value
    for (int i = 0; i < HASHSIZE / 2; i++)
    {
        BlockHash[i] = '0';      //BlockHash[0-15]
        BlockHash[i + 16] = 'x'; //BlockHash[16-31]
    }
    lli HASH1 = HashTransaction(B->Transaction);
    //  printf("\nHASH1:%lld\n", HASH1);
    int i = 0;
    //Assigning first 4 bits of our 32 bit hash string====>BLOCKHASH[0-3]//
    for (; i < 4; i++)
    {
        if (HASH1 != 0)
        {
            BlockHash[i] = (HASH1 % 10) + '0';
            HASH1 = HASH1 / 10;
        }
    }
    // i=4;
    char *PrevBlockHash = B->PrevBlockHash;
    //Assigning next 16 bits of our 32 bit hash string====>BLOCKHASH[4-19]//
    for (int j = 0; j < 16; j++)
    {
        BlockHash[i] = ((PrevBlockHash[j + 16]) ^ (PrevBlockHash[j] >> 1));
        i++;
    }
    //i=20;
    // printf("\nHASH2222:::::\n");//
    /*for (int k = 0; k < 20; k++)
    {
        printf("%c", BlockHash[k]);
    }*/

    lli HASH2 = HornerRule(B->PrevBlockHash, *(B->Nonce)); //CALCULATES HASH VARIABLE FOR NEXT 6 BITS//

    //printf("\n\nHASH333333:%lld\n", HASH2);

    //Assigning next 6 bits of our 32 bit hash string====>BLOCKHASH[20-25]//
    for (; i < 26; i++)
    {
        if (HASH2 != 0)
        {
            BlockHash[i] = (HASH2 % 10) + '0';
            HASH2 /= 10;
        }
    }

    //CALCULATES HASH VARIABLE FOR NEXT 6 BITS//
    lli HASH3 = HornerRule(B->PrevBlockHash, B->BlockNumber + *(B->Nonce));

    //printf("\n\nHASH333333:%lld\n", HASH3);//

    //Assigning next 6 bits of our 32 bit hash string====>BLOCKHASH[26-31]//
    for (; i < HASHSIZE; i++)
    {
        if (HASH3 != 0)
        {
            BlockHash[i] = (HASH3 % 10) + '0';
            HASH3 /= 10;
        }
    }
    for (int i = 0; i < HASHSIZE; i++)
    {
        if (BlockHash[i] >= 0 && BlockHash[i] <= 31) //IF CHARACTER IS IN NON-PRINTABLE CHARACTER RANGE//
            BlockHash[i] = i + 'a';
    }

    //APPEND NULL CHARACTER TO THE END OF THE BLOCK HASH//
    BlockHash[HASHSIZE - 1] = '\0';
    return BlockHash;
}
//sub routine function for finding the height of AVL tree data structure used for storing user data//
lli max(lli a, lli b)
{
    if (a > b)
        return a;
    return b;
}
//Get Height of the AVL tree//
lli Height(Position P)
{
    if (P == NULL)
        return -1;
    return P->Height;
}
//Create Node of the AVL Tree//
AVLuser CreateUserNode(lli Id)
{
    AVLuser Temp = (AVLuser)malloc(sizeof(UserNode));
    Temp->WalletBalance = (lli *)malloc(sizeof(lli));
    *(Temp->WalletBalance) = 1000; //BONUS WALLET BALANCE//
    Temp->UserId = Id;
    Temp->Right = NULL;
    Temp->Left = NULL;
    Temp->Height = 0;
    Temp->History = CreateHead(); //CREATES EMPTY LIST HEADER//
    Temp->JoinTime = time(&Temp->JoinTime);
    return Temp;
}
//STANDARD OPERATIONS FOR AVL TREE ROTATION====>REFERENCE : ********DSA-COURSE GITHUB REPO********//
Position SingleRotateWithLeft(Position k2)
{
    //Insertion of node has taken place in left child of left subtree
    Position k1 = k2->Left;
    k2->Left = k1->Right;
    k1->Right = k2;

    k2->Height = max(Height(k2->Left), Height(k2->Right)) + 1;
    k1->Height = max(Height(k1->Left), Height(k1->Right)) + 1;

    return k1; //new root
}
Position SingleRotateWithRight(Position k2)
{
    //Insertion of node has taken place in right child of right subtree

    Position k1 = k2->Right;
    k2->Right = k1->Left;
    k1->Left = k2;

    k2->Height = max(Height(k2->Left), Height(k2->Right)) + 1;
    k1->Height = max(Height(k1->Left), Height(k1->Right)) + 1;

    return k1; //new root
}
Position DoubleRotateWithLeft(Position k2)
{
    //Rotation between k3 and k1
    k2->Left = SingleRotateWithRight(k2->Left);
    //Rotation between k1 and k2
    return SingleRotateWithLeft(k2);
}
Position DoubleRotateWithRight(Position k2)
{
    k2->Right = SingleRotateWithLeft(k2->Right);

    return SingleRotateWithRight(k2);
}
//ADDS USER TO THE AVL USER TREE//
UserNode *AddUser(AVLuser Tree, lli Id)
{
    if (Tree == NULL)
    {
        //CREATES USER NODE WHEN FOUND ITS CORRECT POSITION FOR INSERTION//
        Tree = CreateUserNode(Id);
    }
    else if (Id < Tree->UserId)
    {
        Tree->Left = AddUser(Tree->Left, Id);
        Tree->Height = 1 + max(Height(Tree->Left), Height(Tree->Right));
        //Check if AVL Property has been violated after inserion into the left subtree//
        if ((Height(Tree->Left) - Height(Tree->Right)) > 1)
        {
            //If insertion has taken place into the left child of the left subtree//
            if (Id < Tree->Left->UserId)
            {
                Tree = SingleRotateWithLeft(Tree);
            }
            else
            {
                Tree = DoubleRotateWithLeft(Tree);
            }
        }
    }
    else if (Tree->UserId < Id)
    {
        Tree->Right = AddUser(Tree->Right, Id);
        Tree->Height = 1 + max(Height(Tree->Left), Height(Tree->Right));

        if ((Height(Tree->Right) - Height(Tree->Left)) > 1)
        {
            //Insertion in left child of right subtree
            if (Id < Tree->Right->UserId)
            {
                Tree = DoubleRotateWithRight(Tree);
            }
            else
            {
                Tree = SingleRotateWithRight(Tree);
            }
        }
    }

    return Tree;
}
//STANDARD BST SEARCH OPERATION//
UserNode *Search(AVLuser root, lli SearchId)
{
    //return NULL is no user exists, else returns pointer to the user Node//
    if (root == NULL)
        return NULL;

    else if (root->UserId == SearchId)
        return root;

    else if (root->UserId < SearchId)
        return Search(root->Right, SearchId);

    else
        return Search(root->Left, SearchId);
}
//PRINTS USER INFORMATION//
void PRINTUSERDETAILS(AVLuser root, lli Id)
{
    UserNode *U = Search(root, Id);
    if (U == NULL) //IF USER DOES'NT EXISTS//
    {
        printf("\nERROR : INVALID USER ID\n");
    }
    else
    {
        //PRINTS THE USER DETAILS//
        printf("\nUSER ID:%lld\t\tWALLET BALANCE:%lld\t\tUSER SINCE:%s\n", U->UserId, *(U->WalletBalance), ctime(&(U->JoinTime)));
    }
}

void PrintUserInfo(AVLuser root, lli Id)
{
    UserNode *U = Search(root, Id);
    if (U == NULL) //IF USER DOES'NT EXISTS//
    {
        printf("\nERROR : INVALID USER ID\n");
    }
    else
    {
        //PRINTS THE USER DETAILS//
        printf("\nUSER ID:%lld\t\tWALLET BALANCE:%lld\n", U->UserId, *(U->WalletBalance));
    }
}
//Functions For BlockChain//
Chain *CreateChain()
{
    //RETURNS EMPTY BLOCK HEADER//
    Chain *L = (Chain *)malloc(sizeof(Chain));
    L->Head = NULL;
    L->Tail = NULL;
    L->NumBlock = (lli *)malloc(sizeof(lli));
    *(L->NumBlock) = 0;
    return L;
}
Block *CreateBlock(List TransactionHead)
{
    //CREATES A BLOCK NODE AND RETURNS POINTER TO THE BLOCK//
    Block *B = (Block *)malloc(sizeof(Block));
    B->Nonce = (int *)malloc(sizeof(int));
    B->Next = NULL;
    B->Transaction = TransactionHead;
    return B;
}
//FUNCTION TO ADD BLOCK TO THE CHAIN//
void AddToChain(Chain *C, List Head)
{
    //IF THERES NO BLOCK THAT HAS BEEN ADDED//
    if (*(C->NumBlock) == 0)
    {
        Block *B = CreateBlock(Head);
        *(B->Nonce) = (rand() % 500) + 1; //randomly generated number b/w 1&500//
        B->BlockNumber = 1;
        B->PrevBlockHash = (char *)malloc(HASHSIZE * sizeof(char));
        for (int i = 0; i < HASHSIZE; i++)
        {
            B->PrevBlockHash[i] = '0';
        }
        B->PrevBlockHash[HASHSIZE - 1] = '\0';
        C->Head = B;
        C->Tail = B;
        *(C->NumBlock) = 1; //UPDATES THE NO OF BLOCKS IN THE CHAIN//
    }
    else
    {
        Block *B = CreateBlock(Head);
        B->BlockNumber = *(C->NumBlock) + 1; //UPDATES BLOCK NUMBER//

        *(C->NumBlock) += 1;              //UPDATES THE NO OF BLOCKS IN THE CHAIN//
        *(B->Nonce) = (rand() % 500) + 1; //randomly generated number b/w 1&500

        B->PrevBlockHash = CreateHash(C->Tail);
        //   printf("\n\nTC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");==>FLAG MESSAGE//
        //UPDATE BLOCK POINTERS//

        C->Tail->Next = B;
        C->Tail = B;
    }
}
//Functions for making a Transaction list//
Node *CreateNode(lli To, lli From, lli Amount, time_t t)
{
    Node *T = (Node *)malloc(sizeof(Node));
    T->TransactionFrom = From;
    T->TransactionTo = To;
    T->Amount = Amount;
    T->Next = NULL;
    T->TIME = t;
    return T;
}
//RETURNS EMPTY TRANSACTION LIST HEAD//
List CreateHead()
{
    List L = (List)malloc(sizeof(Transactions));
    L->num = (int *)malloc(sizeof(int));
    *(L->num) = 0;
    L->Front = NULL;
    L->Rear = NULL;
    return L;
}
//UTILITY FUNCTION TO ADD TRANSACTION NODE TO A GIVEN LIST HEADER OF TRANSACTIONS//
void AddTransactionToList(List L, lli To, lli From, lli Amount, time_t t)
{
    Node *Temp = CreateNode(To, From, Amount, t);

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
        *(L->num) = *(L->num) + 1;
    }
}
//FUNCTION FOR UPDATING USER HISTORY//
void UpdateHistory(AVLuser root, lli To, lli From, lli Amount, time_t t)
{
    UserNode *Sender = Search(root, From);
    UserNode *Reciever = Search(root, To);
    //ADDS TRANSACTION TO THE USER NODES//
    AddTransactionToList(Sender->History, To, From, Amount, t);
    AddTransactionToList(Reciever->History, To, From, Amount, t);
}
//UPDATE THE USER balance FOR A TRANSACTION//
int Transact(AVLuser root, lli To, lli From, lli Amount, time_t t)
{
    //Transaction is posible only if amount is greater than zero
    if (Amount <= 0)
    {
        printf("\nINVALID TRANSACTION: ENTER A VALID TRANSACTION AMOUNT!!!!!\n");

        return 0;
    }

    //Searches for the Sender and the Receiver
    //Returns a NULL pointer if not valid
    UserNode *Sender = Search(root, From);
    UserNode *Receiver = Search(root, To);

    //Checks if the UIDs exist
    if (!Sender || !Receiver)
    {
        if (!Sender)
            printf("\nINVALID UID: %lld\nTRANSACTION FAILED!!!!!\n", From);
        if (!Receiver)
            printf("\nINVALID UID: %lld\nTRANSACTION FAILED!!!!!\n", To);

        return 0;
    }

    //Checks if Sender has enough Wallet Balance
    else if (*(Sender->WalletBalance) < Amount)
    {
        printf("\nINSUFFICIENT WALLET BALANCE!!!!!\n");
        printf("CURRENT WALLET BALANCE: %lld\n", *(Sender->WalletBalance));
        printf("ADD %lld COINS TO YOUR WALLET\n", Amount - *(Sender->WalletBalance));

        return 0;
    }

    //Transaction is valid

    //Prints Wallet Balance
    //Before Transfer
    printf("\nORIGINAL WALLET BALANCE:\n\n");
    printf("RECEIVER: ");
    PrintUserInfo(root, To);
    printf("SENDER: ");
    PrintUserInfo(root, From);

    //Updates Wallet Balance of both the Users
    *(Sender->WalletBalance) = *(Sender->WalletBalance) - Amount;
    *(Receiver->WalletBalance) = *(Receiver->WalletBalance) + Amount;

    //Updates Transaction History List of both the Users
    UpdateHistory(root, To, From, Amount, t);

    //Prints Wallet Balance
    //After Transfer
    printf("\nUPDATED WALLET BALANCE:\n\n");
    printf("RECEIVER: ");
    PrintUserInfo(root, To);
    printf("SENDER: ");
    PrintUserInfo(root, From);

    return 1;
}
//UTILITY FUNCTION FOR PRINTING DETAILS OF TRANSACTIONS STORED IN A LIST NODES//
void PrintTransactionList(List L)
{
    int count = *(L->num);
    if (count == 0)
    {
        printf("\nTRANSACTION LIST EMPTY-NO TRANSACTION TO SHOW!!\n");
    }

    else
    {
        Node *Temp = L->Front;
        while (count != 0)
        {
            printf("\n%lld\t\t%lld\t\t%lld\t\t%s\n", Temp->TransactionTo, Temp->TransactionFrom, Temp->Amount, ctime(&(Temp->TIME)));
            Temp = Temp->Next;
            count--;
        }
    }
}
//FUNCTION ATTACK->GENERATES RANDOM ID , AND UPDATES VALUE OF IT'S NONCE//
//RETURNS 1 IF ATTACK IS SUCCESSFULL ELSE -1//
int Attack(Chain *mylist)
{
    Block *p;
    int x;
    int random = (rand() % (50)) + 1; //randomly generating a numner b/w 1 & 50//
    //if block with the random number generated exists returns 1->attack succesfull else returns -1 ->attack succefull//
    if (*(mylist->NumBlock) >= random)
    {
        for (p = mylist->Head; p->BlockNumber != random;)
        {
            if (p == NULL) //IF REACHES END OF THE BLOCK LIST AND BLOCK WITH RANDOM USER ID IS'NT FOUND//
                break;
            p = p->Next;
        };             //iterates until we find the block-//with the randomly generated number
        if (p != NULL) //==>BLOCK EXISTS AND P POINTS TO THAT BLOCK//
        {
        L1:
            x = (rand() % (500)) + 1;
            if (x == *(p->Nonce))
                goto L1; //UNTIL A NEW NONCE ISN'T FOUND//
            else
            {
                *(p->Nonce) = x; //UPDATES THE BLOCK NONCE//
            }
            //randomly generating a number b/w 1 & 500//
            return 1;
        }
        else
            return -1;
    }
}
//FUNCTION TO VERIFY THE BLOCK CHAIN STRUCTURE//
int Validate_BlockChain(Block *startingBlock, int *x) //==> WE PASS THE FRONT BLOCK OF BLOCK CHAIN   ***C->FRONT*** as starting block in main.c function file
{
    //IF BLOCK IS NULL OR ONLY ONE BLOCK EXISTS IN THE BLOCK CHAIN==>BLOCK CHAIN IS VALID(NOTHING TO CHECK)//
    //RETURNS 1 IF VALID STRUCTURE//
    if (startingBlock == NULL || startingBlock->Next == NULL)
        return 1;
    else
    {
        if (strcmp(CreateHash(startingBlock), startingBlock->Next->PrevBlockHash) == 0) //if hash matches check for next block in the list//
            return Validate_BlockChain(startingBlock->Next, x);
        else
        {
            int i = 1;
            //FLAG MESSAGES IF NODE IS FOUND TO BE ALTERED//
            printf("\n\nATTACK DETECTED!!!!\n\n");
            printf("\nINVALID NONCE: %d\nCORRUPTEDHASH: %s ORIGINAL HASH: %s\n", *(startingBlock->Nonce), CreateHash(startingBlock), startingBlock->Next->PrevBlockHash);
            //ROUTINE FOR FINDINING THE CORRECT NONCE AND RESTORE THE ORIGINAL BLOCK IN THE CHAIN//
            while (strcmp(CreateHash(startingBlock), startingBlock->Next->PrevBlockHash) != 0)
            {
                //to find the correcc=t nonce for the corrupted block//
                *(startingBlock->Nonce) = i;
                i++;
                if (i > 501)
                    break;
            }
            //FLAG MESSAGES//
            printf("\nVALID NONCE: %d\nFIXEDHASH: %s\n", *(startingBlock->Nonce), CreateHash(startingBlock));
            printf("\n\nATTACK FIXED!!!!\n\n");
            //*x stores the number of attacks fixed by the validate function//
            *x = *x + 1;
            return Validate_BlockChain(startingBlock->Next, x);
            //check for validation of other blocks present in the list//
        }
    }
}
