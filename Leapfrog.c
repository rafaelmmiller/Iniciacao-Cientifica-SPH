//Alterar a função double f(double x)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define	TRUE	1
#define	FALSE	0
#define PI	3.1415926535897932384626433832795

double dt=1E-6;
int HarmonicOscillator=FALSE; 

void LeapFrog(double *r, double *v, double *a, double K, double M, int *i);
double F(double r, double K, double M);

int main(){
	double r, v, a;				//posição, velocidade e aceleração escalares
	double K, M;				//dureza da mola e massa acoplada
	double t=0, T;				//tempos de simulação
	int i=0;
	char k;
	printf("Do you want to try the harmonic oscillator? (Y/N): ");
	scanf("%c", &k);
	while(k!='Y' && k!='N'){
		printf("Do you want to try the harmonic oscillator? (Y/N): ");
		scanf("%c", k);
	}
	if(k=='Y'){
		HarmonicOscillator=TRUE;
		printf("\n*****************************************************************************\n");
		printf("\n*****************************************************************************\n");
		printf("\n**************************** Harmonic Oscillator ****************************\n");
		printf("\n*****************************************************************************\n");
		printf("\n*****************************************************************************\n\n");
		
		printf("Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: ");
		scanf("%lf %lf", &K, &M);
		while(K<=0||M<=0){
			printf("Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: ");
			scanf("%lf %lf", &K, &M);
		}
		printf("During how much time do you want to simulate it? Enter with T>0: ");
		scanf("%lf", &T);
		while(T<=0){
			printf("Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: ");
			scanf("%lf", &T);
		}
		printf("Give it initial conditions. Enter with r(0) and v(0): ");
		scanf("%lf %lf", &r, &v);
		a=F(r, K, M);
		for(t=0; t<=T; t=t+dt){
			LeapFrog(&r, &v, &a, K, M, &i);
			if(i%2==0) a=F(r, K, M);
			if(i%10000==0){
				printf("%4.2lf %20.10lf\n", t, r);
				//printf("t=%4.2lf: r(t)=%20.10lf\n", t, r);
				//printf("        v(t)=%20.10lf\n", v);
				//printf("        a(t)=%20.10lf\n", a);
			}
		}
	}
	if(k=='N'){
		HarmonicOscillator=FALSE;
		K=0; M=0;
		printf("The kind of motion chosen was the forced oscillation.\n");
		printf("During how much time do you want to simulate it? Enter with T>0: ");
		scanf("%lf", &T);
		while(T<=0){
			printf("Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: ");
			scanf("%lf", &T);
		}
		printf("Give it initial conditions. Enter with r(0) and v(0): ");
		scanf("%lf %lf", &r, &v);
		a=F(r, K, M);
		for(t=0; t<=T; t=t+dt){
			LeapFrog(&r, &v, &a, K, M, &i);
			if(i%2==0) a=F(r, K, M);
			if(i%10000==0){
				printf("%4.2lf %20.10lf\n", t, r);
				//printf("t=%4.2lf: r(t)=%20.10lf\n", t, r);
				//printf("        v(t)=%20.10lf\n", v);
				//printf("        a(t)=%20.10lf\n", a);
			}
		}
	}
	return 0;
}

void LeapFrog(double *r, double *v, double *a, double K, double M, int *i){
	if(*i%2==0){
		*r=*r+(*v)*dt;
	}
	else{
		*v=*v+(*a)*dt;
	}
	*i=*i+1;
}

double F(double r, double K, double M){
	if(HarmonicOscillator==TRUE){
		return (((-K)*r)/M);
	}
	else{
		return 0.01*cos(r);				//inserir uma função arbitrária de r
	}
}
