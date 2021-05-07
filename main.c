#include"header.h"

int main()
{
    int choice = 0;
    int continuecondn = 0;
    int numBlocks = 0;
    int index;
    node* ptr = NULL;
    CreatingBlocks();
    while(continuecondn == 0)
    {
        printf("Enter following Options - \n");
        printf("To Allocate press - 1 | To DeAllocate previous set pointer press - 2\n");
        printf("To DeAllocate by BlockIndex press - 3 | To print FreeList Contents press - 4\n");
        printf("To print AllocList Contents press - 5 | To Exit press - 6\n");
        scanf("%d",&choice);

        if(choice == 1)
        {
            printf("Enter Number of blocks to Allocate - ");
            scanf("%d",&numBlocks);
            ptr = Allocate(numBlocks);
        }
        else if(choice == 2)
        {
            DeAllocate(ptr);
        }
        else if(choice == 3)
        {
            printf("Enter index of Block to DeAllocate - ");
            scanf("%d",&index);
            DeAllocateByIndex(index);
        }
        else if(choice == 4)
        {
            printFreeList();
        }
        else if(choice == 5)
        {
            printAllocList();
        }
        else if(choice == 6)
        {
            continuecondn = 1;
        }
    }


    return 0;
}