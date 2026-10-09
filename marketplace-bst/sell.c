/*
 * sell.c — 拍賣平台（資料結構：二元搜尋樹 + Heap Sort + 檔案 I/O）
 *
 * 每項商品是 BST 的一個節點，節點內記錄所有賣家與價格；
 * 依 input.txt 的指令進行新增、查詢、購買（以 Heap 找出最低價賣家）、刪除與排序，
 * 結果輸出成 Buy／Search／Sort／Log 報表。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct seller
{
    char ID[20];
    int price;
}seller;

typedef struct commodity
{
    char name[20];
    seller *owner;
    int owner_num;
    struct commodity *lchild;
    struct commodity *rchild;
}commodity;

int heightBST(commodity* tree);
commodity* deleteBST(commodity** tree ,char name[]);
int heapsort(seller *heap, int i ,int num);
void insert(commodity **tree,char name[],char ID[],int price);
void search(commodity *tree,char name[]);
void buy(commodity **tree,char name[]);
void sort(commodity *tree);
void report();
FILE *fpin = NULL;
FILE *fbuy = NULL;
FILE *flog = NULL;
FILE *fsearch = NULL;
FILE *fsort = NULL;
int insertcount = 0;
int searchcount = 0;
int searchwa = 0;
int buycount = 0;
int buywa = 0;
int node = 0;

int main() {
    char open[10];
    scanf("%s",open);
    fpin = fopen(open, "r");
    fbuy = fopen("BuyTable.txt", "w");
    flog = fopen("LogTable.txt", "w");
    fsearch = fopen("SearchTable.txt", "w");
    fsort = fopen("SortTable.txt", "w");
    char order[7];
    char name[15];
    char ID[15];
    int price = 0;
    commodity *tree = NULL;
    while (fscanf(fpin,"%s",order)!= EOF){
        if (strcmp(order,"insert") == 0){
            //printf("insert\n");
            insertcount++;
            fscanf(fpin,"%s %s %d",name,ID,&price);
            insert(&tree,name,ID,price);
            printf("\n");
            //printf("%s %s %d\n",name,ID,price);
        }else if(strcmp(order,"search") == 0){
            //printf("search\n");
            searchcount++;
            fscanf(fpin,"%s",name);
            search(tree,name);
            //printf("%s\n",name);
        }else if (strcmp(order,"buy") == 0){
            //printf("buy\n");
            buycount++;
            fscanf(fpin,"%s",name);
            buy(&tree,name);
            //printf("%s\n",name);
        }else if(strcmp(order,"sort") == 0){
            //printf("sort\n");
            sort(tree);
            fprintf(fsort,"----------------------------\n");
        }else if (strcmp(order,"report") == 0){
            //printf("report\n");
            report(tree);
        }else{
            //printf("wrong\n");
        }

    }
}

void insert(commodity** root,char name[],char ID[],int price) {
    if((*root) == NULL){
        node++;
        //printf("nothing\n");
        //printf("%s %s %d\n",name,ID,price);
        (*root) = (commodity*)malloc(sizeof(commodity));
        (*root)->owner_num = 1;
        (*root)->lchild = NULL;
        (*root)->rchild = NULL;
        strcpy((*root)->name,name);
        (*root)->owner = (seller*)malloc(sizeof(seller)*7);
        strcpy((*root)->owner[0].ID,ID);
        (*root)->owner[0].price = price;
        //printf("%s %d %s %d\n",(*root)->name,(*root)->owner_num,(*root)->owner[0].ID,(*root)->owner[0].price);
    }else{
        if(strcmpi((*root)->name,name)==0){
            //printf("find\n");
            (*root)->owner_num++;
            int n = (*root)->owner_num;
            strcpy((*root)->owner[n-1].ID,ID);
            (*root)->owner[n-1].price = price;
            //printf("%s %d %s %d\n",(*root)->name,(*root)->owner_num,(*root)->owner[n-1].ID,(*root)->owner[n-1].price);
            for(int i =(*root)->owner_num/2 ; i >= 0 ; i--){ //sort from bottom to top
                heapsort((*root)->owner,i,(*root)->owner_num);
            }
            /*
            for(int j = 0 ; j < (*root)->owner_num ; j++){
                printf("%s %d\n",(*root)->owner[j].ID,(*root)->owner[j].price);
            }
            */
        }else{
            //printf("!find\n");
            if(strcmpi((*root)->name,name)<0){
                //printf("goright\n");
                insert(&(*root)->rchild,name,ID,price);
            }else{
                //printf("goleft\n");
                insert(&(*root)->lchild,name,ID,price);
            }
        }
    }
}

void search(commodity *tree,char name[]) {
    //printf("search\n");
    int flag = 0;
    while(tree){
        if(strcmp(tree->name,name) == 0){
            flag = 1 ;
            fprintf(fsearch,"%s\n",tree->name);
            for(int i = 0 ; i < tree->owner_num ; i++){
                fprintf(fsearch,"%s %d\n",tree->owner[i].ID,tree->owner[i].price);
            }
            fprintf(fsearch,"----------------------------\n");
        }
        if(strcmp(tree->name,name) < 0){
            tree = tree->rchild;
        }else{
            tree = tree->lchild;
        }
    }
    if(flag == 0){
        searchwa++;
        fprintf(fsearch,"%s doesn't exist!\n",name);
        fprintf(fsearch,"----------------------------\n");
    }
}

void buy(commodity **tree,char name[]) {
    //printf("buy\n");
    if((*tree) == NULL){
        buywa++;
        fprintf(fbuy,"%s doesn't exist!\n",name);
    }else{
        if(strcmpi((*tree)->name,name) == 0){
            fprintf(fbuy,"%s %s %d\n",(*tree)->name,(*tree)->owner[0].ID,(*tree)->owner[0].price);
            if((*tree)->owner_num <= 1){
                node--;
                (*tree)->owner_num = 0;
                if((*tree)->lchild == NULL && (*tree)->rchild == NULL){ //no child
                    (*tree) = NULL;
                }else if((*tree)->lchild == NULL){ //have right child
                    (*tree) = (*tree)->rchild;
                }else if((*tree)->rchild == NULL){ //have left child
                    (*tree) = (*tree)->lchild;
                }else{                             //have many childs
                    commodity** temp = &(*tree)->lchild;
                    while((*temp)->rchild){
                        (*temp) = (*temp)->rchild;
                    }
                    strcpy((*tree)->name,(*temp)->name);
                    (*tree)->owner_num = (*temp)->owner_num;
                    (*tree)->owner = (*temp)->owner;
                    (*temp) = NULL;
                }
            }else{
            //printf("find\n");
            printf("%s %d\n",(*tree)->name,(*tree)->owner_num);
            int n = (*tree)->owner_num;
            (*tree)->owner[0] = (*tree)->owner[n-1];//root change to the last node
            //(*tree)->owner[n-1].price = 0; // last node reset to 0
            (*tree)->owner_num--;
            heapsort((*tree)->owner,0,(*tree)->owner_num); //sort from top to bottom
            
            /*
            for(int j = 0 ; j < (*tree)->owner_num ; j++){
                printf("%s %s %d\n",(*tree)->name,(*tree)->owner[j].ID,(*tree)->owner[j].price);
            }
            */
           }
        }else{
            //printf("!find\n");
            if(strcmpi((*tree)->name,name)<0){
                //printf("goright\n");
                buy(&(*tree)->rchild,name);
            }else{
                //printf("goleft\n");
                buy(&(*tree)->lchild,name);
            }
        }
    }
}
void sort(commodity *tree) {
    //printf("sort\n");
    if(tree){
        sort(tree->lchild);
        fprintf(fsort,"%s\n",tree->name);
        sort(tree->rchild);
    }
}
void report(commodity* tree) {
    //fprintf(flog,"report\n");
    fprintf(flog,"insert %d\n",insertcount);
    fprintf(flog,"search %d %d\n",searchcount,searchwa);
    fprintf(flog,"buy %d %d\n",buycount,buywa);
    fprintf(flog,"node_num %d\n",node);
    fprintf(flog,"height %d\n",heightBST(tree));
}

int heapsort(seller *heap, int i,int num){
    //printf("heapsort\n");
    int smallest = i; //root node
    int left = i*2 +1; //left child
    int right = i*2 +2; //right child
    if(left < num){
        if(heap[left].price < heap[smallest].price){ //left_data is smaller than node_data
            smallest = left;
        }
    }  
    if(right < num){
        if(heap[right].price < heap[smallest].price){ //right_data is smaller than node_data
            smallest = right;
        }
    }
    if(smallest != i){ //left_data or right_data is smaller than node_data
        seller temp = heap[i];
        heap[i] = heap[smallest];;
        heap[smallest] = temp;
        heapsort(heap,smallest,num); //check the change node left_data and right_data
    }
}

int heightBST(commodity* tree){
    if(tree==NULL) return 0;
    int l = heightBST(tree->lchild);
    int r = heightBST(tree->rchild);
    if(l>r){
        return l+1;
    }else{
        return r+1;
    }
}
