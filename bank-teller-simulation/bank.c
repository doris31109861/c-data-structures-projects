#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CustomerNode
{
    char id[21];    // customer id
    int arr_time;   // customer arrive bank time
    int ser_time;   // customer spend time on bank
    int leave_time; // customer leave bank time
    int window;     // window serve the customer
    struct CustomerNode *next;
} CustomerNode;

typedef struct TellerQ
{
    int status;           // 0:close 1:open
    CustomerNode *front;  // first index
    CustomerNode *rear;   // last index
    int count;            // how many people in line
    int proceessing_time; // previous customer leave time
} TellerQ;

TellerQ *bank;                                                              // the array store all queue front pointer
TellerQ *output;                                                            // store leaved customer informations
int QSizeMin();                                                             // return the least queue
void Push(TellerQ *obj, int min, char idptr[], int arr_time, int ser_time); // add the customer in the counter
int Pop(TellerQ *obj, int n);                                               // minus the customer in the counter && add the customer into the output queue
int TellerNum = 0;                                                          // how many teller

void swap(CustomerNode *a, CustomerNode *b) // swap two node information
{
    int leave = a->leave_time;
    a->leave_time = b->leave_time;
    b->leave_time = leave;

    int window = a->window;
    a->window = b->window;
    b->window = window;

    char *id = (char *)malloc((strlen(a->id) + 1) * sizeof(char));
    strcpy(id, a->id);
    strcpy(a->id, b->id);
    strcpy(b->id, id);
    free(id);
}

void bubbleSort(CustomerNode *start) // leave time from small to large array the customer
{
    int swapped, i;
    CustomerNode *ptr1;
    CustomerNode *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr)
        {
            if (ptr1->leave_time > ptr1->next->leave_time)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            else if (ptr1->leave_time == ptr1->next->leave_time)
            {
                if (ptr1->window > ptr1->next->window)
                {
                    swap(ptr1, ptr1->next);
                    swapped = 1;
                }
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
void PrintfQ(TellerQ *obj) // print out the information if queue
{
    bubbleSort(output->front);
    CustomerNode *p = obj->front;
    while (p != NULL)
    {
        printf("%s %d %d \n", p->id, p->leave_time, p->window);
        p = p->next;
    }
}

int main()
{
    FILE *fp = NULL;
    char filename[15];
    printf("scanf the data name\ndata1:input1.tst data2:input2.tst\n");
    scanf("%s", filename);                       // open which file
    fp = fopen(filename, "r");                   // open the file
    fscanf(fp, "%d", &TellerNum);                // how many teller
    output = (TellerQ *)malloc(sizeof(TellerQ)); // initialize the output queue
    output->front = NULL;
    output->rear = NULL;
    output->count = 0;
    bank = (TellerQ *)malloc(sizeof(TellerQ) * TellerNum); // open teller mounts' queue
    for (int i = 0; i < TellerNum; i++)                    // initialize every teller queue
    {
        bank[i].count = 0;
        bank[i].proceessing_time = 0;
        bank[i].status = 1;
        bank[i].front = NULL;
        bank[i].rear = NULL;
    }
    char name[21] = {0};
    int t1 = 0;
    int t2 = 0;
    int n = 0;
    while (fscanf(fp, "%s %d %d", name, &t1, &t2) != EOF) // before EOF scanf the information
    {
        /*before push customer into queue , pop out the finised customer first*/
        for (int i = 0; i < TellerNum; i++) // check every queue
        {
            CustomerNode *temp = bank[i].front; // check from the head
            while (temp != NULL)                // check before queue end
            {
                if (temp->leave_time <= t1) // teller finished the customer
                {
                    Pop(bank, i); // customer leave the bank
                }
                temp = temp->next; // check the next one
            }
        }
        if (name[0] == '#') // close the counter
        {
            /*before close the window , pop out the finised customer first*/
            for (int i = 0; i < TellerNum; i++)
            {
                CustomerNode *p = bank[i].front;
                while (p != NULL)
                {
                    if (p->leave_time <= t1) // the customer need to leave
                    {
                        bank[i].front->leave_time = bank[i].proceessing_time;
                        bank[i].front->window = i;
                        Pop(bank, i);
                    }
                    p = p->next;
                }
            }
            bank[t2].status = 0;        // close the counter
            if (bank[t2].front != NULL) // customer not finished
            {
                bank[t2].proceessing_time = bank[t2].front->leave_time; // save the first customer
                CustomerNode *close = bank[t2].front->next;
                while (close != NULL) // other pop out && line up again until the tail
                {
                    int min = QSizeMin();                                         // get the shortest queue
                    Push(bank, min, close->id, close->arr_time, close->ser_time); // push the customer into the shortest queue
                    if (bank[min].count == 1 && bank[min].proceessing_time <= t1) // the customer is the first one in new queue
                    {
                        bank[min].proceessing_time = t1 + close->ser_time; // customer leave time = lineupagain time + servetime
                    }
                    else // the customer isnot the first one in new queue
                    {
                        bank[min].proceessing_time = bank[min].proceessing_time + close->ser_time; // customer leave time = the previous customer leave time + servetime
                    }
                    bank[min].rear->leave_time = bank[min].proceessing_time; // refresh the teller processing_time
                    bank[min].rear->window = min;

                    CustomerNode *temp = bank[t2].front->next;         // malloc a Node point to the head
                    bank[t2].front->next = bank[t2].front->next->next; // head pointer point to the next node
                    bank[t2].count--;                                  // customer pop out
                    if (bank[t2].front == NULL)
                    {
                        bank[t2].rear = NULL;
                    }
                    free(temp);
                    close = close->next; // check the next one
                }
            }
        }
        else if (name[0] == '@') // open the counter
        {
            /*before open the window , pop out the finised customer first*/
            for (int i = 0; i < TellerNum; i++)
            {
                CustomerNode *tp = bank[i].front;
                while (tp != NULL)
                {
                    if (tp->leave_time <= t1) // the customer need to leave
                    {
                        bank[i].front->leave_time = bank[i].proceessing_time;
                        bank[i].front->window = i;
                        Pop(bank, i);
                    }
                    tp = tp->next;
                }
            }
            bank[t2].status = 1; // open the counter
        }
        else // new customer in bank
        {
            int qsizemin = QSizeMin();          // get the shortest queue
            Push(bank, qsizemin, name, t1, t2); // push the customer into the shortest queue
            if (bank[qsizemin].count == 1)      // the customer is the first one in new queue
            {
                bank[qsizemin].proceessing_time = t1 + t2; // customer leave time = arrive time + serve time
            }
            else // the customer isnot the first one in new queue
            {
                bank[qsizemin].proceessing_time = bank[qsizemin].proceessing_time + t2; // customer leave time = the previous customer leave time + servetime
            }
            bank[qsizemin].rear->leave_time = bank[qsizemin].proceessing_time; // refresh the teller processing_time
            bank[qsizemin].rear->window = qsizemin;
        }
    }
    /*no one get in the bank every one pop out when they finished */
    for (int i = 0; i < TellerNum; i++) // check every queue
    {
        CustomerNode *sear = bank[i].front; // check from the head
        while (sear != NULL)                // check before queue end
        {
            Pop(bank, i);
            sear = sear->next; // check the next one
        }
    }
    PrintfQ(output); // print out the output after sort
}

void Push(TellerQ *obj, int min, char idptr[], int arr_time, int ser_time)
{
    obj[min].count++;                                                  // add people in counter
    CustomerNode *temp = (CustomerNode *)malloc(sizeof(CustomerNode)); // malloc a Node
    strcpy(temp->id, idptr);                                           // copy the information
    temp->arr_time = arr_time;
    temp->ser_time = ser_time;
    temp->next = NULL;
    if (obj[min].front == NULL)
    {
        obj[min].front = temp;
        obj[min].rear = temp;
    }
    else
    {
        obj[min].rear->next = temp;
        obj[min].rear = temp;
    }
}

int Pop(TellerQ *obj, int n)
{
    obj[n].count--;  // minus people in counter
    output->count++; // add people in output
    /*push the information input the output*/
    CustomerNode *tp = (CustomerNode *)malloc(sizeof(CustomerNode)); // malloc a Node
    strcpy(tp->id, obj[n].front->id);                                // copy the information
    tp->leave_time = obj[n].front->leave_time;
    tp->window = obj[n].front->window;
    tp->next = NULL;
    if (output->front == NULL)
    {
        output->front = tp;
        output->rear = tp;
    }
    else
    {
        output->rear->next = tp;
        output->rear = tp;
    }
    /*pop out the customer from counter*/
    CustomerNode *temp = obj[n].front;
    obj[n].front = obj[n].front->next;
    if (obj[n].front == NULL)
    {
        obj[n].rear = NULL;
    }
    free(temp);
}
int QSizeMin()
{
    int min = 0;
    for (int i = 0; i < TellerNum; i++)
    {
        if (bank[i].count < bank[min].count && bank[i].status == 1)
        {
            min = i;
        }
    }
    return min;
}
