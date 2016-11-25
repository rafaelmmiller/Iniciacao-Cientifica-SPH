//Alterar a função double f(double x)

#include <iostream>
#include <cmath>
#define	TRUE	1
#define	FALSE	0
#define PI	3.1415926535897932384626433832795

using namespace std;

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
	cout << "Do you want to try the harmonic oscillator? (Y/N): " << endl;
	cin >> k;
	while(k!='Y' && k!='N'){
		cout << "Do you want to try the harmonic oscillator? (Y/N): " << endl;
		cin >> k;
	}
	if(k=='Y'){
		HarmonicOscillator=TRUE;
		cout << "\n*****************************************************************************\n" << endl;
		cout << "\n*****************************************************************************\n" << endl;
		cout << "\n**************************** Harmonic Oscillator ****************************\n" << endl;
		cout << "\n*****************************************************************************\n" << endl;
		cout << "\n*****************************************************************************\n\n" << endl;
		
		cout << "Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: " << endl;
		cin >> K >> M;
		while(K<=0||M<=0){
			cout << "Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: " << endl;
			cin >> K >> M;
		}
		cout << "During how much time do you want to simulate it? Enter with T>0: " << endl;
		cin >> T;
		while(T<=0){
			cout << "During how much time do you want to simulate it? Enter with T>0: " << endl;
			cin >> T;
		}
		cout << "Give it initial conditions. Enter with r(0) and v(0): " << endl;
		cin >> r >> v;
		a=F(r, K, M);
		for(t=0; t<=T; t=t+dt){
			LeapFrog(&r, &v, &a, K, M, &i);
			if(i%2==0) a=F(r, K, M);
			if(i%100000==0){
				cout << t << " " << r << " " << v << " " << a << endl;
				//cout << "t=" << t << ": r(t)=" << r << endl;
				//cout << "       v(t)=" << v << endl;
				//cout << "       a(t)=" << a << endl;
			}
		}
	}
	if(k=='N'){
		HarmonicOscillator=FALSE;
		K=0; M=0;
		cout << "The kind of motion chosen was the forced oscillation.\n" << endl;
		cout << "During how much time do you want to simulate it? Enter with T>0: " << endl;
		cin >> T;
		while(T<=0){
			cout << "Enter with K>0, the spring stiffness, and M>0, the mass attached to the spring: " << endl;
			cin >> T;
		}
		cout << "Give it initial conditions. Enter with r(0) and v(0): " << endl;
		cin >> r, v;
		a=F(r, K, M);
		for(t=0; t<=T; t=t+dt){
			LeapFrog(&r, &v, &a, K, M, &i);
			if(i%2==0) a=F(r, K, M);
			if(i%100000==0){
				cout << t << " " << r << " " << v << " " << a << endl;
				//cout << "t=" << t << ": r(t)=" << r << endl;
				//cout << "       v(t)=" << v << endl;
				//cout << "       a(t)=" << a << endl;
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
		return sqrt(r);
	}
	else{
		return sqrt(r);				//inserir uma função arbitrária de r
	}
}
