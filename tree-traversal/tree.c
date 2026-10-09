#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct treenode
{
    char data;
    struct treenode *leftchild;
    struct treenode *rightchild;
}treenode;

treenode *findtree(char in[],int start, int end, char pre[],int *pindex){
    if(start<=end){
        char root = pre[(*pindex)];
        treenode *newnode = (treenode*)malloc(sizeof(treenode));
        newnode->data = root;
        int i = start;
        while(in[i] != root){
            i++;
        }
        (*pindex)++;
        newnode->leftchild = findtree(in,start,i-1,pre,pindex);
        newnode->rightchild = findtree(in,i+1,end,pre,pindex);
        return newnode;
    }else{
        return NULL;
    }
}

void travel(treenode *tree){
    if(tree){
        travel(tree->leftchild);
        travel(tree->rightchild);
        printf("%c",tree->data);
    }
}

int main(){
    char pre[9] = {"ABCDEFGHI"};
    char in[9] = {"BCAEDGHFI"};
    int preindex = 0;
    treenode *post = findtree(in,0,8,pre,&preindex);
    travel(post);
}
