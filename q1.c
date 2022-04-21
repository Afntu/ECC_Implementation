#include<stdio.h>
void swap(int x1, int x2,int n)
{
	int t;
	t=((x2^x1)&(-n));
	x2=(t^x2);
	x1=(t^x1);
	printf("x1=%d, x2=%d\n",x1,x2);	
}
void main()
{
	int x1,x2,n;
	int c, d;
	printf("Enter the values of x1 and x2\n");
	scanf("%d%d",&x1,&x2);
	printf("Enter the value of n either 0 or 1\n");
	scanf("%d",&n);
	printf("x1=%d, x2=%d\n",x1,x2);
	swap(x1,x2,n);
}	
