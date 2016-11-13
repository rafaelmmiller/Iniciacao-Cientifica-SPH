//Alterar a função double f(double x)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define	TRUE	1
#define	FALSE	0
#define PI	3.1415926535897932384626433832795

int UnitTestFlag=FALSE;
double dx=1E-5;
double h=1E-2;

double SPH(double , double );

double a(double , double , double );
double b(double , double , double );
double c(double , double , double );
double d(double , double , double );

double f(double );

int main(){
	double x_0;					//valor no qual calcular uma função por dois métodos
	double p, q;				//valores calculado diretamente, e por SPH, para uma dada função
	char k=0;					//verificador de perguntas
	
	printf("Is this a test? (Y/N): ");
	scanf("%c", &k);
	while(k!='Y'&& k!='N'){
		printf("\nOnly (Y/N) answers. Is this a test? (Y/N): ");
		scanf("%c", &k);
	}
	if(k=='Y'){
		UnitTestFlag=TRUE;
		h=1E-4;
		for(x_0=-PI; x_0<=PI && UnitTestFlag==TRUE; x_0=x_0+(PI/10)){
			p=SPH(x_0, h);
			q=f(x_0);
			printf("\nx_0=%10.5lf, SPH[f(x_0)]=%10.5lf, f(x_0)=%10.5lf\n", x_0, p, q);
			if(fabs(p-q)>=1E-3){
				printf("\n\nThis program isn't working correctly for x=%10.5lf, between -pi and pi and the test function.\n", x_0);
				UnitTestFlag=FALSE;
			}
		}
	}
	if(k=='N'){
		printf("\nEnter with h>0 and x_0: ");
		scanf("%lf %lf", &h, &x_0);
		p=SPH(x_0, h);
		q=f(x_0);
		printf("\nx_0=%10.5lf, SPH[f(x_0)]=%10.5lf, f(x_0)=%10.5lf\n", x_0, p, q);
		if(fabs(p-q)>=1E-3){
			printf("\n\nThis program isn't working correctly for x=%10.5lf, and the SPH selected test function.\n", x_0);
		}
		else{
			printf("\nThis program is working correctly to x=%10.5lf and the SPH selected test function.\n", x_0);
		}
	}		
	if(UnitTestFlag==TRUE){
		printf("\nThis program is working correctly to the values tested on the test function.\n");
	}
	if(UnitTestFlag==TRUE) return 0;
	else return 10;
}

//Início: Funções do kernel

double a(double x, double x_0, double h){
	return (pow((2-((x-x_0)/h)), 3)-4*(pow((1-((x-x_0)/h)), 3)))/(6*h);
}

double b(double x, double x_0, double h){
	return (pow((2+((x-x_0)/h)), 3)-4*(pow((1+((x-x_0)/h)), 3)))/(6*h);
}

double c(double x, double x_0, double h){
	return (pow((2-((x-x_0)/h)), 3))/(6*h);
}

double d(double x, double x_0, double h){
	return (pow((2+((x-x_0)/h)), 3))/(6*h);
}

//Fim: Funções do kernel

double SPH(double x_0, double h){
	double I=0, x;
	dx=h/1000;
	for(x=x_0-(2*h); x>=x_0-(2*h) && x< x_0-h; x=x+dx)
		I=I+((f(x))*(d(x, x_0, h))*dx);
	
	for(x=x_0-h; x>=(x_0-h) && x< x_0; x=x+dx)
		I=I+((f(x))*(b(x, x_0, h))*dx);
	
	for(x=x_0; x>=x_0 && x< x_0+h; x=x+dx)
		I=I+((f(x))*(a(x, x_0, h))*dx);
	
	for(x=x_0+h; x>=x_0+h && x< x_0+(2*h); x=x+dx)
		I=I+((f(x))*(c(x, x_0, h))*dx);
	return I;
}

double f(double x){
	if(UnitTestFlag==TRUE){
	/*Função teste, pré-estabelecida*/
		return sin(x);
	}
	/*Função escohida para fazer o SPH*/
	return ((exp(x)+exp(-x))/2);
}
