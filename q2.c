#include <stdio.h>
void add(unsigned long int a[], unsigned long int b[], unsigned long int c[])
{
	unsigned long int carry;
	c[0]=a[0]+b[0];
	c[1]=a[1]+b[1];
	
	carry=c[0]>>31;	c[0]=c[0]&0x7FFFFFFF;	c[1]=c[1]+carry;
	carry=c[1]>>30;	c[1]=c[1]&0x3FFFFFFF;	c[0]=c[0]+carry;
	carry=c[0]>>31;	c[0]=c[0]&0x7FFFFFFF;	c[1]=c[1]+carry;
	carry=c[1]>>30;	c[1]=c[1]&0x3FFFFFFF;
}
void mult(unsigned long int a[], unsigned long int b[], unsigned long int d[])
{	
	unsigned long int carry,c[3];
	c[0] = a[0]*b[0];
	c[2] = a[1]*b[1];
	c[1] = (a[0]+a[1])*(b[0]+b[1])-c[0]-c[2];
	c[2]=c[2]<<1;
	c[0]=c[0]+c[2];
	
	carry=c[0]>>31;	d[0]=c[0]&0x7FFFFFFF;	c[1]=c[1]+carry;
	carry=c[1]>>30;	d[1]=c[1]&0x3FFFFFFF;	d[0]=d[0]+carry;
	carry=d[0]>>31;	d[0]=d[0]&0x7FFFFFFF;	d[1]=d[1]+carry;
	carry=d[1]>>30;	d[1]=d[1]&0x3FFFFFFF;		
}
void subs(unsigned long int a[],unsigned long int b[],unsigned long int c[])
{
	unsigned long int d[2]={0x7FFFFFFF,0x3FFFFFFF},e[2];
	e[0]=d[0]^b[0];	e[1]=d[1]^b[1];
	add(a,e,c);
}
void point_double(unsigned long int x1[],unsigned long int z1[],unsigned long int p[],unsigned long int q[])
{
	unsigned long int P[2],Q[2],R[2],S[2],T[2],U[2],V[2],W[2]={1447112650,898208916};  //W=(A+2)/4(mod p)
	add(x1,z1,P);		//P=(x1+z1)
	subs(x1,z1,Q);		//Q=(x1-z1)
	mult(P,P,S);		//S=P*P=(x1+z1)^2
	mult(Q,Q,T);		//T=Q*Q=(x1-z1)^2
	mult(S,T,p);           //p=S*T=((x1+z1)(x1-z1))^2
	subs(S,T,P);		//P=S-T=(x1+z1)^2-(x1-z1)^2=4*x1*z1
	mult(W,P,R);		//R=W*P=((A+2)/4)(4x1*z1)
	add(T,R,Q);		//Q=T+R=((x1-z1)^2+((A+2)/4)(4x1*z1))
	mult(P,Q,q);		//q=P*Q=(4x1*z1)((x1-z1)^2+((A+2)/4)(4x1*z1))
}
void point_add(unsigned long int x1[],unsigned long int z1[],unsigned long int x2[],unsigned long int z2[],unsigned long int p[],unsigned long int q[])
{
	unsigned long int P[2],Q[2],R[2],S[2],T[2],U[2],V[2],W[2];
	V[0]=486145363;	V[1]=268485549;
	W[0]=1;		W[1]=0;//Z co-ordinate of P-Q which is always the given point({486145363,268485549},.,{1,0})
	add(x1,z1,P);		//P=x1+z1
	subs(x1,z1,Q);		//Q=x1-z1
	add(x2,z2,S);		//S=x2+z2
	subs(x2,z2,T);		//T=x2-z2
	mult(P,T,R);		//R=P*T=(x1+z1)(x2-z2)
	mult(S,Q,U);		//U=S*Q=(x1-z1)(x2+z2)
	add(R,U,P);		//P=R+U=(x1+z1)(x2-z2)+(x1-z1)(x2+z2)
	subs(R,U,Q);		//Q=R-U=(x1+z1)(x2-z2)-(x1-z1)(x2+z2)
	mult(P,P,S);		//S=P*P=((x1+z1)(x2-z2)+(x1-z1)(x2+z2))^2
	mult(Q,Q,T);		//T=Q*Q=((x1+z1)(x2-z2)-(x1-z1)(x2+z2))^2
	mult(S,W,p);		//p=W*S=([1,0])((x1+z1)(x2-z2)+(x1-z1)(x2+z2))^2
	mult(T,V,q);		//q=V*T=([486145363,268485549])((x1+z1)(x2-z2)-(x1-z1)(x2+z2))^2
}
void swap(int b,unsigned long int x[],unsigned long int z[],unsigned long int x1[],unsigned long int z1[])
{
	unsigned long int t[4];
	t[0]=(x[0]^x1[0])&(-b);	t[1]=(x[1]^x1[1])&(-b);
	t[2]=(z[0]^z1[0])&(-b);	t[3]=(z[1]^z1[1])&(-b);
	x[0]=t[0]^x[0];	x[1]=t[1]^x[1];	z[0]=t[2]^z[0];	z[1]=t[3]^z[1];
	x1[0]=t[0]^x1[0];	x1[1]=t[1]^x1[1];	z1[0]=t[2]^z1[0];	z1[1]=t[3]^z1[1];
}
void field_inverse(unsigned long int x[])
{
	int k;
	unsigned long int P[2],Q[2],R[2],S[2];
	P[0]=x[0];	P[1]=x[1];
	mult(P,P,Q);
	for(k=1;k<60;k++)
	{
		mult(Q,Q,R);
		mult(P,R,S);
		P[0]=S[0];	P[1]=S[1];
		Q[0]=R[0];	Q[1]=R[1];
	}
	x[0]=P[0];	x[1]=P[1];	
}
void main()
{
	unsigned long int A[2]={1493483305,371610194},B[2]={1,0},X[2]={486145363,268485549},Z[2]={1,0},N[2],C[2],D[2];
	unsigned long int n;
	printf("Enter the vale of n less than 576460752315733303:\n");
	scanf("%lu",&n);
	N[0]=(unsigned int)(n&0x7FFFFFFF);
	N[1]=(unsigned int)((n>>31)&0x3FFFFFFF);
	printf("Base 2^31 representation of N:[%lu,%lu]\n",N[0],N[1]);
	unsigned char m[64];
	point_double(X,Z,A,B);
	printf("2P=([%lu , %lu],.,[%lu , %lu])\n",A[0],A[1],B[0],B[1]);
	point_add(X,Z,A,B,C,D);
	printf("P+2P=([%lu , %lu],.,[%lu , %lu]\n",C[0],C[1],D[0],D[1]);
	int i=0,k,j,l;
	while(N[0]!=0)
	{
		m[i]=(N[0]&1);
		i++;
		N[0]=N[0]>>1;
	}
	j=31;
	while(N[1]!=0)
	{
		m[j]=(N[1]&1);
		j++;
		N[1]=N[1]>>1;
	}
	if(N[1]==0)
		l=i;
	else
		l=j;
	unsigned long int R_1[2],R_2[2],S_1[2],S_2[2],P_1[2],P_2[2],Q_1[2],Q_2[2];
	R_1[0]=X[0];	R_1[1]=X[1];
	R_2[0]=1;	R_2[1]=0;
	point_double(R_1,R_2,S_1,S_2);
	for(k=l-2;k>=0;k--)		              //Montgomery ladder implemantation by using 'swap' function.
	{
		swap(m[k+1]^m[k],S_1,S_2,R_1,R_2);
		P_1[0]=S_1[0];		P_1[1]=S_1[1];
		P_2[0]=S_2[0];		P_2[1]=S_2[1];
		Q_1[0]=R_1[0];		Q_1[1]=R_1[1];
		Q_2[0]=R_2[0];		Q_2[1]=R_2[1];
		point_double(P_1,P_2,S_1,S_2);
		point_add(P_1,P_2,Q_1,Q_2,R_1,R_2);
	}
	swap(m[0],S_1,S_2,R_1,R_2);
	printf("Projective Space coordinate: Np=([%lu,%lu],.,[%lu,%lu])\n",S_1[0],S_1[1],S_2[0],S_2[1]);
	A[0]=S_2[0];	A[1]=S_2[1];
	field_inverse(S_2);
	printf("Inverse of z coordinate:[%lu,%lu]=[%lu,%lu]\n",A[0],A[1],S_2[0],S_2[1]);
	mult(S_1,S_2,R_1);
	printf("Affine plane coordinate: Np=([%lu,%lu],.)\n",R_1[0],R_1[1]);	
}
