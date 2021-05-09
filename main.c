#include <stdio.h>
#include "Bitcoin.h"
#include <stdlib.h>
#include <string.h>
//Transaction-List Header for storing the current transactions//
List CurrList;
//variable for storing random user id ->in AddUser implementation//
lli id;
//dummy variable for storing usernode poinnter//
UserNode *Temp;
time_t TIME;
Block *TAIL;
//for generating random user id using rand()%MAX//
#define MAX 104729
int main()
{
    id = 1;
    //TASKS ENTERED BY USER
    char op1[9] = "ADDUSER";
    op1[8] = '\0';
    char op2[10] = "TRANSACT";
    op2[9] = '\0';
    char op3[8] = "ATTACK";
    op3[7] = '\0';
    char op4[10] = "VALIDATE";
    op4[9] = '\0';
    char op5[] = "PRINTUSERDETAILS";
    op5[16] - '\0';
    char op6[23] = "PRINTUSERTRANSACTIONS";
    op6[22] = '\0';
    // ***Print user transaction history//
    //Creating Header for storing the chain of Blocks//
    Chain *C = CreateChain();
    // Block *TAIL;==>TESTING VARIABLE//
    //Creating root of the AVL tree for storing the user data//
    AVLuser root = NULL;
    //Seeding time//
    srand(time(0));
    //Initialising the Transaction-List Header//
    CurrList = CreateHead();
    while (1)
    {
        //TAKES INPUT OPERATION TO BE PERFORMED BY THE USER//
        printf("\nENTER COMMAND:\n1->%s\n2->%s\n3->%s\n4->%s\n5->%s\n6->%s\n7->EXIT\n", op1, op2, op3, op4, op5, op6);
        char Operation[30];
        scanf("%s", Operation);
        if (strcmp(Operation, "EXIT") == 0)
        {
            //TERMINATES THE PROGRAM//
            printf("\nPROGRAM-TERMINATED!!!!!!\n");
            break;
        }
        else if (strcmp(Operation, op1) == 0)
        {

        L1:
            //Generating random user id using random number generator//
            id = (rand() % MAX) + 1;
            Temp = Search(root, id);
            //if Temp==NULL-->no such user existing with id,and hence adds user to the AVL TREE//
            //Else again generates a new random id that's not already present in the AVL TREE//
            if (Temp != NULL)
                goto L1;

            root = AddUser(root, id); //insert into the tree//

            printf("\nUSER ADDED SUCCESSFULLY ID:%lld AND BONUS WALLET BALANCE :1000 COINS\n", id);

            /*
            <===FUNCTION TO INSERT 1000 USER NODES INTO THE ROOT===>FOR TESTING OTHER FUNCTIONS
            for (int k = 1; k < 1000; k++)
                         id = id + 1;
            */
        }
        else if (strcmp(Operation, op2) == 0)
        {
            printf("\n<===========ENTER INPUT AS : <RECIEVER's Id> <SENDER's Id> <TRANSACTION AMOUNT>===========>\n");

            lli To, From, Amount;
            int valid;

            scanf("%lld %lld %lld", &To, &From, &Amount);

            //Stores time which is to be added to the transaction information later//
            time_t t = time(&t);

            //Transfers Coins if the transaction is valid//
            valid = Transact(root, To, From, Amount, t); //returns 1 for a valid transaction else 0//

            if (valid)
            {
                //Adds transaction details to the Current Block
                AddTransactionToList(CurrList, To, From, Amount, t);

                //Checks if No of transactions in the Current Block = 50
                if (*(CurrList->num) == 50)
                {
                    //Adds a New Block to the BlockChain
                    AddToChain(C, CurrList);

                    // printf("\nHASH OF BLOCK: %lldis:\n", *(C->NumBlock));
                    // printf("\n%s\n", C->Tail->PrevBlockHash);

                    //Creates a Header for the new Transaction List//
                    CurrList = CreateHead();
                }
            }
        }

        else if (strcmp(Operation, op3) == 0)
        {
            int x;
            // for (int i = 0; i < 100; i++)==>FOR GENERATING RANDOM TEST CASES OF ATTACK==>TESTING FUNCTIONS//
            {
                //ATTACK RETURNS 1 IN CASE OF SUCCESSFUL ATTACK ELSE ATTACK FAILS//
                x = Attack(C);
                if (x == 1)
                {
                    printf("\n\nATTACK SUCCESSFULL\n\n");
                }
                else
                    printf("\n\nATTACK FAILED\n\n");
            }
        }
        else if (strcmp(Operation, op4) == 0)
        {
            int *x = (int *)malloc(sizeof(int));
            *x = 0;
            Validate_BlockChain(C->Head, x);
            //(*x) == 0 ==>THE BLOCK CHAIN IS VALID NO ERROR//
            if (*x == 0)
                printf("\n\nBLOCK CHAIN IS VALID!!!!!!!\n\n"); //FLAG MESSAGE//
            else
            {
                printf("\n\n***** %d ATTACKS DETECTED AND FIXED*****\n", *x); //FLAG MESSAGE//
            }
            free(x);
        }
        else if (strcmp(Operation, op5) == 0)
        {
            lli ID;
            printf("ENTER USER ID:");
            scanf("%lld", &ID);
            PRINTUSERDETAILS(root, ID);

            //HELPER FUNCTION TO PRINT USER DETAILS//
        }
        else if (strcmp(Operation, op6) == 0)
        {
            lli ID;
            printf("ENTER USER ID:");
            scanf("%lld", &ID);

            UserNode *Temp = Search(root, ID);
            if (Temp == NULL) //IF NO SUCH USER EXISTS//
                printf("\n\nINVALID USER!!!!\n\n");

            //PRINTS TRANSACTION HISTORY OF USER ID ENTERED//
            else
            {
                //IF THE TRANSACTION HISTORY IS NULL//
                if (Temp->History->Front == NULL)
                {
                    printf("\nTRANSACTION LIST EMPTY-NO TRANSACTION TO SHOW!!\n");
                }
                else
                {
                    printf("\n\nTRANSACTIONS MADE BY USER :%lld\n\n", Temp->UserId);

                    printf("TO\t\tFROM\t\tAMOUNT\t\tTIME\n");

                    PrintTransactionList(Temp->History);
                }
            }
        }
        else
        {
            //IF NO VALID COMMAND IS ENTERED//
            printf("\n\nENTER A VALID COMMAND!!!!\n\n");
            continue;
        }
    }

    return 0;
}