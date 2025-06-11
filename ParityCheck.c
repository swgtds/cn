#include<stdio.h>  
int main()
{
    int data[5];
    int received[6];
    int parity,count=0;
    int i;
    printf("Enter 5 binary bits:\n");
    for(i=0;i<5;i++) 
        scanf("%d",&data[i]);
    for(i=0;i<5;i++) {
        if(data[i]==1)
           count++;
    }
    parity=(count%2==0)?0:1;
    for(i=0;i<5;i++)
        received[i]=data[i];
    received[5]=parity;
    printf("Data with parity:"); 
    for(i=0;i<6;i++) 
        printf("%d",received[i]);
    printf("\n");
    count=0;
    for(i=0;i<6;i++) {
        if(received[i]==1)
            count++;
    }
    if(count%2==0)
        printf("No error detected");
    else
        printf("Error detected");
    return 0;
}
