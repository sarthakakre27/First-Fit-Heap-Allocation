#include"header.h"

node* freeList = NULL;
node* freeListLast = NULL;
node* allocList = NULL;
node* allocListLast = NULL;

void CreatingBlocks()
{
    int total;//var for total memory
    int blockIndex = 0;//initializing block index to 0
    int freeSize = 0;//size of free block for user input
    int allocSize = 0;//size of allocated block for user input
    node* newfree = NULL;//new free node allocation
    node* newalloc = NULL;//new free node allocation
    printf("Setting memory blocks as free and allocated\n");
    printf("Enter the Total Memory in KB - ");
    //integral blocks of 1KB totalling the memory to value of variable total
    //blocks indexing to start from 0 so indexes-- (0,total-1)
    scanf("%d",&total);
    //assigning memory as free blocks or allocated blocks for the initialization of two lists
    while(blockIndex <= total - 1)
    {
        printf("Enter free block size - ");
        scanf("%d",&freeSize);
        if(blockIndex + freeSize <= total)
        {
            newfree = (node*)malloc(sizeof(node));
            newfree->indexStartAddress = blockIndex;
            newfree->length = freeSize;
            if(freeList == NULL)
            {
                newfree->next = NULL;
                newfree->prev = NULL;
                freeList = newfree;
                freeListLast = freeList;
            }
            else
            {
                freeListLast->next = newfree;
                newfree->prev = freeListLast;
                freeListLast = newfree;
                freeListLast->next = NULL;
            }
            blockIndex += freeSize;
        }
        else//freesize is greater than memory blocks available
        {
            newfree = (node*)malloc(sizeof(node));
            newfree->indexStartAddress = blockIndex;
            newfree->length = total - blockIndex;
            //allocating remaining space // does'nt matter as we are simulating initial condition
            if(freeList == NULL)
            {
                newfree->next = NULL;
                newfree->prev = NULL;
                freeList = newfree;
                freeListLast = freeList;
            }
            else
            {
                freeListLast->next = newfree;
                newfree->prev = freeListLast;
                freeListLast = newfree;
                freeListLast->next = NULL;
            }
            blockIndex += freeSize;
        }

       if(blockIndex <= total - 1)
       {
            printf("Enter Allocated block size - ");
            scanf("%d",&allocSize);
            if(blockIndex + allocSize <= total)
            {
                newalloc = (node*)malloc(sizeof(node));
                newalloc->indexStartAddress = blockIndex;
                newalloc->length = allocSize;
                if(allocList == NULL)
                {
                    newalloc->next = NULL;
                    newalloc->prev = NULL;
                    allocList = newalloc;
                    allocListLast = allocList;
                }
                else
                {
                    allocListLast->next = newalloc;
                    newalloc->prev = allocListLast;
                    allocListLast = newalloc;
                    allocListLast->next = NULL;
                }
                blockIndex += allocSize;
            }
            else//allocsize is greater than memory blocks available
            {
                newalloc = (node*)malloc(sizeof(node));
                newalloc->indexStartAddress = blockIndex;
                newalloc->length = total - blockIndex;
                if(allocList == NULL)
                {
                    newalloc->next = NULL;
                    newalloc->prev = NULL;
                    allocList = newalloc;
                    allocListLast = allocList;
                }
                else
                {
                    allocListLast->next = newalloc;
                    newalloc->prev = allocListLast;
                    allocListLast = newalloc;
                    allocListLast->next = NULL;
                }
                blockIndex += allocSize;
            }
       }
    }
    printf("Blocks fixed. The FreeList and AllocList contents are - \n");
    printFreeList();
    printAllocList();
    printf("----------------------------------------------------------------------------\n");
}

void AddToAllocList(int Blockind,int BlockSize)
{
    node* newalloc = (node*)malloc(sizeof(node));
    if(allocList == NULL)
    {
        allocList = newalloc;
        allocListLast = allocList;
        newalloc->next = NULL;
        newalloc->prev = NULL;
        newalloc->indexStartAddress = Blockind;
        newalloc->length = BlockSize;
    }
    else
    {
        allocListLast->next = newalloc;
        newalloc->prev = allocListLast;
        newalloc->next = NULL;
        allocListLast = newalloc;
        newalloc->indexStartAddress = Blockind;
        newalloc->length = BlockSize;
    }
}

//this function returns the pointer to the block where blocksize is allocated and combines
//the remaining blocks to the free list if no block found returns NULL
node* Allocate(int blockSize)
{
    //FIRST FIT ALGORITHM--
    node* newalloc = NULL;
    node* trav_ptr = freeList;
    node* tempnode = NULL;
    node* ret_val = NULL;
    node* prevnode = NULL;
    node* nextnode = NULL;
    int found = 0;
    int ind = 0;
    if(trav_ptr == NULL)//free list is null //heap is full
    {
        ret_val = NULL;
    }
    else
    {
        while(trav_ptr != NULL && found == 0)
        {
            if(trav_ptr->length >= blockSize)//block found first fit condition
            {
                printf("found at index %d\n",trav_ptr->indexStartAddress);
                printf("\n\nAllocated at index- %d size- %d\n",trav_ptr->indexStartAddress,blockSize);
                trav_ptr->length -= blockSize;
                ind = trav_ptr->indexStartAddress;
                trav_ptr->indexStartAddress += blockSize;
                if(trav_ptr->length > 0)
                {
                    AddToAllocList(ind,blockSize);
                    found = 1;
                }
                else if(trav_ptr->length == 0)
                {
                    found = 1;
                    if(trav_ptr == freeList && freeListLast == freeList)//single node case
                    {
                        AddToAllocList(ind,blockSize);
                        free(freeList);
                        freeList = freeListLast = NULL;
                    }
                    else if(trav_ptr == freeList && freeListLast != freeList)//at least 2 nodes
                    {
                        tempnode = freeList;
                        freeList = freeList->next;
                        AddToAllocList(ind,blockSize);
                        free(tempnode);
                        freeList->prev = NULL;
                    }
                    else if(trav_ptr == freeListLast && freeListLast != freeList)//at least 2 nodes
                    {
                        tempnode = freeListLast;
                        freeListLast = freeListLast->prev;
                        AddToAllocList(ind,blockSize);
                        free(tempnode);
                        freeListLast->next = NULL;
                    }
                    else//at least 3 nodes
                    {
                        tempnode = trav_ptr;
                        prevnode = tempnode->prev;
                        nextnode = tempnode->next;
                        prevnode->next = nextnode;
                        nextnode->prev = prevnode;
                        AddToAllocList(ind,blockSize);
                        free(tempnode);

                    }
                }
                ret_val = allocListLast;//return value pointing to the last of allocated list as it is added in this case
            }
            trav_ptr = trav_ptr->next;
        }
        if(found == 0)
        {
            printf("No block found\n");
            ret_val = NULL;
        }
    }
    return ret_val;
}

//this function deallocates the allocated memory from previous Allocate function
//the pointer to the block is necessary to pass as argument which was returned by the Allocate function
//Assumes that pointer passed is present in the Allocated list
void DeAllocate(node* DeAllocptr)
{
    node* tempnode = NULL;
    if(DeAllocptr == NULL)
    {
        printf("Bad DeAllocation\n");
    }
    else
    {
        if(DeAllocptr == allocList && allocList == allocListLast)//single node case
        {
            merge(DeAllocptr);
            //free(DeAllocptr);
            allocListLast = allocList = NULL;
        }
        else if(DeAllocptr == allocList && allocList != allocListLast)//at least 2 nodes
        {
            tempnode = DeAllocptr;
            allocList = allocList->next;
            allocList->prev = NULL;
            merge(tempnode);
            //free(tempnode);
            //tempnode = NULL;
        }
        else if(DeAllocptr == allocListLast && allocListLast != allocList)//at least 2 nodes
        {
            tempnode = DeAllocptr;
            allocListLast = allocListLast->prev;
            allocListLast->next = NULL;
            printf("deallocated\n");
            merge(tempnode);
            //tempnode = NULL;
        }
        else if(allocList->next == allocListLast && DeAllocptr != allocList && DeAllocptr != allocListLast)
        {
            printf("Bad Deallocation\n");
        }
        else//at least 3 nodes and end positions do not contain the passed pointer
        {
            node* prevnode = DeAllocptr->prev;
            node* nextnode = DeAllocptr->next;
            tempnode = DeAllocptr;
            prevnode->next = nextnode;
            nextnode->prev = prevnode;
            merge(tempnode);
            //free(tempnode);
            //tempnode = NULL;
        }
    }
}

//this function deallocates by the index number in case we need to deallocate the memory block which were given
//at the starting condition for the allocated list and the free list using the CreatingBlocks() function
//since there no pointers to that memory blocks we can take the block starting index and deallocate them
void DeAllocateByIndex(int ind)
{
    if(allocList == NULL)//no node case
    {
        printf("Allocated List Empty\n");
    }
    else//at least 1 node
    {
        node* tempnode = NULL;
        if(allocList == allocListLast && allocList->indexStartAddress == ind)//single node case
        {
            tempnode = allocList;
            merge(tempnode);
            printf("found and merged\n");
            //free(tempnode);
            allocList = allocListLast = NULL;
        }
        else if(allocList != allocListLast && allocList->indexStartAddress == ind)//at least 2 nodes
        {
            tempnode = allocList;
            allocList = allocList->next;
            allocList->prev = NULL;
            merge(tempnode);
            printf("found and merged\n");
            //free(tempnode);
            //tempnode = NULL;
        }
        else if(allocList != allocListLast && allocListLast->indexStartAddress == ind)//at least 2 nodes
        {
            tempnode = allocListLast;
            allocListLast = allocListLast->prev;
            allocListLast->next = NULL;
            merge(tempnode);
            printf("found and merged\n");
            //free(tempnode);
            //tempnode = NULL;
        }
        else if(allocList->next == allocListLast && allocList->indexStartAddress != ind && allocListLast->indexStartAddress != ind)
        {
            printf("Block Not Found\n");
        }
        else//at least 3 nodes and end positions do not contain the index passed
        {
            node* ptr = NULL;
            node* foundptr = NULL;
            node* prevnode = NULL;
            node* nextnode = NULL;
            int found = 0;
            ptr = allocList;
            while(ptr != NULL && found == 0)
            {
                if(ptr->indexStartAddress == ind)
                {
                    found = 1;
                    foundptr = ptr;
                }
                ptr = ptr->next;
            }
            
            if(found == 1)
            {
                tempnode = foundptr;
                prevnode = tempnode->prev;
                nextnode = tempnode->next;
                prevnode->next = nextnode;
                nextnode->prev = prevnode;
                merge(tempnode);
                printf("found and merged\n");
                //free(tempnode);
                //tempnode = NULL;

            }
            else//found == 0
            {
                printf("Block Not Found\n");
            }
        }
    }

}

//this function merges the passed parameter i.e. merging node approriately to the blocks if they are contiguous 
//and if not the function adds the merging node to the free list at approriate location in sorted order of indexStartingAddress
//of the blocks

void merge(node* mergeptr)
{
    node* prevnode = NULL;
    if(freeList == NULL)
    {
        freeList = mergeptr;
        freeListLast = freeList;
        freeList->next = NULL;
        freeList->prev = NULL;
    }
    else
    {
        if(mergeptr->indexStartAddress < freeList->indexStartAddress)//merge at beginning of the start of the freelist
        {
            if((mergeptr->indexStartAddress + mergeptr->length) == freeList->indexStartAddress)
            {
                freeList->indexStartAddress = mergeptr->indexStartAddress;
                freeList->length += mergeptr->length;
                free(mergeptr);
                mergeptr = NULL;
            }
            else if((mergeptr->indexStartAddress + mergeptr->length) < freeList->indexStartAddress)
            {
                mergeptr->next = freeList;
                freeList->prev = mergeptr;
                freeList = mergeptr;
                freeList->prev = NULL;
            }
        }
        else if(mergeptr->indexStartAddress >= (freeListLast->indexStartAddress + freeListLast->length))//merge at the end of the freelist
        {
            if(mergeptr->indexStartAddress == (freeListLast->indexStartAddress + freeListLast->length))
            {
                freeListLast->length += mergeptr->length;
                free(mergeptr);
                mergeptr = NULL;
            }
            else if(mergeptr->indexStartAddress > (freeListLast->indexStartAddress + freeListLast->length))
            {
                freeListLast->next = mergeptr;
                mergeptr->next = NULL;
                mergeptr->prev = freeListLast;
                freeListLast = mergeptr;
            }
        }
        else//merge in between //at least 3 node with merging not at end positions// or 2 nodes with merging in between
        {
            node* ptr = freeList;
            while(ptr != NULL && ptr->indexStartAddress < mergeptr->indexStartAddress)
            {
                ptr = ptr-> next;
            }
            prevnode = ptr->prev;
            if((prevnode->indexStartAddress + prevnode->length) == mergeptr->indexStartAddress && (mergeptr->indexStartAddress + mergeptr->length) == ptr->indexStartAddress)
            {
                prevnode->length += (mergeptr->length + ptr->length);
                prevnode->next = ptr->next;
                if(ptr->next != NULL)
                {
                    ptr->next->prev = prevnode;
                }
                else if(ptr->next == NULL)
                {
                    freeListLast = prevnode;
                }
                free(ptr);
                ptr = NULL;
                free(mergeptr);
                mergeptr = NULL;
            }
            else if((prevnode->indexStartAddress + prevnode->length) == mergeptr->indexStartAddress && (mergeptr->indexStartAddress + mergeptr->length) < ptr->indexStartAddress)
            {
                prevnode->length += mergeptr->length;
                free(mergeptr);
                mergeptr = NULL;
            }
            else if((prevnode->indexStartAddress + prevnode->length) < mergeptr->indexStartAddress && (mergeptr->indexStartAddress + mergeptr->length) == ptr->indexStartAddress)
            {
                ptr->indexStartAddress = mergeptr->indexStartAddress;
                ptr->length += mergeptr->length;
                free(mergeptr);
                mergeptr = NULL;
            }
            else
            {
                mergeptr->next = ptr;
                mergeptr->prev = prevnode;
                prevnode->next = mergeptr;
                ptr->prev = mergeptr;
            }
        }

    }
}

void printFreeList()
{
    node* ptr = freeList;
    if(ptr == NULL)
    {
        printf("No Free Memory. FreeList Empty\n");
    }
    else
    {
        printf("\nFreeList contents - \n");
        printf("Block ind    length\n");
        while(ptr != NULL)
        {
            printf("  %d            %d\n",ptr->indexStartAddress,ptr->length);
            ptr = ptr->next;
        }
        //printf("%d  %d\n",freeListLast->indexStartAddress,freeListLast->length);
    }
    if(freeListLast == NULL)
    {
        printf("freelistlast = NULL\n");
    }
}

void printAllocList()
{
    node* ptr = allocList;
    if(ptr == NULL)
    {
        printf("No Memory Allocated. AllocList Empty\n");
    }
    else
    {
        printf("\nAllocList contents - \n");
        printf("Block ind    length\n");
        while(ptr != NULL)
        {
            printf("  %d            %d\n",ptr->indexStartAddress,ptr->length);
            ptr = ptr->next;
        }
        if(allocListLast == NULL)
        {
            printf("alloclistlast = NULL\n");
        }
        //printf("%d  %d\n",allocListLast->indexStartAddress,allocListLast->length);
    }
    if(allocListLast == NULL)
    {
        printf("alloclistlast = NULL\n");
    }
}