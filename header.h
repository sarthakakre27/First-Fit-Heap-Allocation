#include<stdio.h>
#include<stdlib.h>

typedef struct node_tag
{
    struct node_tag* next;
    struct node_tag* prev;
    int indexStartAddress;
    int length;
}node;

void merge(node* mergeptr);
void CreatingBlocks();
void AddToAllocList(int Blockind,int BlockSize);
node* Allocate(int blockSize);
void DeAllocate(node* DeAllocptr);
void DeAllocateByIndex(int ind);
void printFreeList();
void printAllocList();