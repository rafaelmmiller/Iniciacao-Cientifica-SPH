#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 		3.1415926535897932384626433832795
#define TRUE	1
#define FALSE	0

double Mass=2E-3, Ni=0.5, Eta=0.1, K=0.1;		//massa das partículas, constante de proporcionalidade do amortecimento, da força e da equação de estado, em kg, 1/s, 1/(kg.s²) (ou J/(kg².m²)) e m^5/(kg.s²).
double dt=4E-3, h=4E-2;							//passo de tempo e "largura" do kernel

double Distance(double a[2], double b[2]);
double Kernel(double q);
void Kernel_Gradient(double q, double a[2], double b[2], double c[2]);
void Damping_Acceleration(double v[2], double a[2]);
void Body_Acceleration(double a[2], double b[2], double c[2]);


class Particle{
	private:
		double Position[2], Velocity[2], Acceleration[2], Previous_Velocity[2];
		double Pressure, Density;
		int Grid_Position[2];
	public:
		void SetSystem(){
			double u[2];
			u[0]=u[1]=1.1;
			while(fabs(u[0])>0.95 || fabs(u[1])>0.95){
				u[0]=pow(-1, rand()%2)*( (double) (rand()%1000) )/(rand()%1000);
				u[1]=pow(-1, rand()%2)*( (double) (rand()%1000) )/(rand()%1000);
			}
			Position[0]=u[0];
			Position[1]=u[1];
			Grid_Position[0]=( (int) ((u[0]+1)/(2*h)) );
			Grid_Position[1]=( (int) ((u[1]+1)/(2*h)) );
			Velocity[0]=0;
			Velocity[1]=0;
		}
		void GetPosition(double a[2]){
			a[0]=Position[0];
			a[1]=Position[1];
		}
		void GetVelocity(double a[2]){
			a[0]=Velocity[0];
			a[1]=Velocity[1];
		}
		void GetAcceleration(double a[2]){
			a[0]=Acceleration[0];
			a[1]=Acceleration[1];
		}
		double GetPressure(){return Pressure;}
		double GetDensity(){return Density;}
		void GetGrid_Position(int a[2]){
			a[0]=Grid_Position[0];
			a[1]=Grid_Position[1];
		}
		void SetPosition(double a[2]){
			Position[0]=a[0];
			Position[1]=a[1];
		}
		void SetVelocity(double a[2]){
			Velocity[0]=a[0];
			Velocity[1]=a[1];
		}
		void SetAcceleration(double a[2]){
			Acceleration[0]=a[0];
			Acceleration[1]=a[1];
		}
		void SetPressure(double a){Pressure=a;}
		void SetDensity(double a){Density=a;}
		void SetGrid_Position(){
			double v[2]={Position[0], Position[1]};
			if(fabs(v[0])<=0.95 && fabs(v[1])<=0.95){
				Grid_Position[0]=( (int) ((v[0]+1)/(2*h)) );
				Grid_Position[1]=( (int) ((v[1]+1)/(2*h)) );
			}
		}
		void SetPrevious_Velocity(double a[2]){
            Previous_Velocity[0]=a[0];
            Previous_Velocity[1]=a[1];
		}
};

/*o tamanho da matriz se refere ao número de células em que se pretende dividir
o espaço [-1, 1]x[-1, 1] (até 25x25 células), e o número máximo permitido
de partículas numa mesma célula (até seis partículas)*/

void Fill_Matrix(int mat[25][25][6], Particle Vector[1001]);
void Finding_Neighbours(int i, int Array[54], int mat[25][25][6], Particle Vector[1001]);
void Pressure_Acceleration(int i, double a[2], int Array[54], int mat[25][25][6], Particle Vector[1001]); //a instância que chamar esta função deve zerar a[2].
double Density_SPH(int i, int Array[54], int mat[25][25][6], Particle Vector[1001]);
void Initialize(int mat[25][25][6], Particle Vector[1001]);
void Compute_Accelerations(int mat[25][25][6], Particle Vector [1001]);
void Leap_Frog(int i, int mat[25][25][6], Particle Vector[1001]);

int main(){
	/*printf("Entre com a constante de proporcionalidade da forca atrativa: ");
	scanf("%lf", &Eta);
	while(Eta<=0){
		printf("A constante deve ser positiva. Tente novamente: ");
		scanf("%lf", &Eta);
	}
	printf("Entre com a constante de proporcionalidade do amortecimento: ");
	scanf("%lf", &Ni);
	while(Ni<=0){
		printf("A constante deve ser positiva. Tente novamente: ");
		scanf("%lf", &Ni);
	}
	printf("Entre com a constante de proporcionalidade da equacao de estado: ");
	scanf("%lf", &K);
	while(K<=0){
		printf("A constante deve ser positiva. Tente novamente: ");
		scanf("%lf", &K);
	}*/
	Particle System[1001];
	int mat[25][25][6];
	Initialize(mat, System);
	int i;
	for(i=1; i<100; i++)
        Leap_Frog(i, mat, System);
    printf("\n\n Mil passos depois: \n\n");
    for(i=1; i<1000; i++){
        double a[2];
		System[i].GetPosition(a);
		printf("%lf %lf\n", a[0], a[1]);
    }
	return 0;
}

double Distance(double a[2], double b[2]){
	return sqrt(pow(a[1]-b[1], 2)+pow(a[2]-b[2], 2));
}

double Kernel(double q){
	double c_h=15/(14*PI*pow(h,2));
	if(q>=0 && q<1) return c_h*(pow(2-q, 3)-4*pow(1-q, 3));
	else if(q>=1 && q<2) return c_h*(pow(2-q, 3));
	else if(q>=2) return 0.0;
}

void Kernel_Gradient(double q, double a[2], double b[2], double c[2]){
	double c_h=15/(14*PI*pow(h,2));
	if(q>=0 && q<1){
		c[0]=c_h*((3.0/h)*pow(2-q, 2)*((a[0]-b[0])/(q*h))-(12.0/h)*pow(1-q, 2)*((a[0]-b[0])/(q*h)));		//O vetor c é o gradiente do kernel.
		c[1]=c_h*((3.0/h)*pow(2-q, 2)*((a[1]-b[1])/(q*h))-(12.0/h)*pow(1-q, 2)*((a[1]-b[1])/(q*h)));		//Os vetores a e b são as posições de duas partículas.
	}
	else if(q>=1 && q<2){
		c[0]=c_h*(3.0/h)*pow(2-q, 2)*((a[0]-b[0])/(q*h));
		c[1]=c_h*(3.0/h)*pow(2-q, 2)*((a[1]-b[1])/(q*h));
	}
	else if(q>=2) c[0]=c[1]=0.0;
}

void Fill_Matrix(int mat[25][25][6], Particle Vector[1001]){
	int p, q, l;
	for(p=0; p<25; p++)
		for(q=0; q<25; q++)
			for(l=0; l<6; l++)
				mat[p][q][l]=0;
	for(l=1; l<=1000; l++){
		int a[2]={0, 0};
		Vector[l].GetGrid_Position(a);
		p=a[0];
		q=a[1];
		int m=0;
		while(mat[p][q][m]!=0) m++;
		mat[p][q][m]=l;
	}
}

void Pressure_Acceleration(int i, double a[2], int Array[54], int mat[25][25][6], Particle Vector[1001]){
	Finding_Neighbours(i, Array, mat, Vector);
	int j;
	double x[2], y[2], c[2];
	Vector[i].GetPosition(x);
	for(j=0; Array[j]!=0 && j<54; j++){
		c[0]=c[1]=0;
		Vector[Array[j]].GetPosition(y);
		double q=(Distance(x, y))/h;
		Kernel_Gradient(q, x, y, c);
		a[0] += c[0];
		a[1] += c[1];
	}
	a[0]=-2*Mass*K*a[0];
	a[1]=-2*Mass*K*a[1];
}

void Damping_Acceleration(double v[2], double a[2]){
	a[0]=-Ni*v[0];
	a[1]=-Ni*v[1];
}

void Body_Acceleration(double a[2], double b[2], double c[2]){
	c[0]=b[0]-a[0];
	c[1]=b[1]-a[1];

	//c é o vetor que aponta de a para b; como a força é atrativa, atua nessa direção e sentido.

	c[0]=-Eta*pow(Mass, 2)*c[0];
	c[1]=-Eta*pow(Mass, 2)*c[1];
}

double Density_SPH(int i, int Array[54], int mat[25][25][6], Particle Vector[1001]){
	Finding_Neighbours(i, Array, mat, Vector);
	int j;
	double x[2], y[2], sum;
	Vector[i].GetPosition(x);
	for(j=0, sum=0; Array[j]!=0 && j<54; j++){
		Vector[Array[j]].GetPosition(y);
		double q=(Distance(x, y))/h;
		sum += Kernel(q);
	}
	return Mass*sum;
}

void Finding_Neighbours(int i, int Array[54], int mat[25][25][6], Particle Vector[1001]){
	int a[2], p, q, r, s=0;
	for(p=0; p<54; p++) Array[p]=0;
	Vector[i].GetGrid_Position(a);

	if(a[0]!=0){
		p=a[0]-1;
		if(a[1]!=0){
		 	q=a[1]-1;
			for(r=0; r<6 && mat[p][q][r]!=0; r++)
				Array[s+r]=mat[p][q][r];
			s += r;
		}
		for(r=0, q=a[1]; r<6 && mat[p][q][r]!=0; r++)
			Array[s+r]=mat[p][q][r];
		s += r;
		if(a[1]!=24){
			q=a[1]+1;
			for(r=0; r<6 && mat[p][q][r]!=0; r++)
				Array[s+r]=mat[p][q][r];
			s += r;
		}
	}

	p=a[0];
	if(a[1]!=0){
		q=a[1]-1;
		for(r=0; r<6 && mat[p][q][r]!=0; r++)
			Array[s+r]=mat[p][q][r];
		s += r;
	}
	for(r=0, q=a[1]; r<6 && mat[p][q][r]!=0; r++)
		Array[s+r]=mat[p][q][r];
	s += r;
	if(a[1]!=24){
		q=a[1]+1;
		for(r=0; r<6 && mat[p][q][r]!=0; r++)
			Array[s+r]=mat[p][q][r];
		s += r;
	}

	if(a[0]!=24){
		p=a[0]+1;
		if(a[1]!=0){
			q=a[1]-1;
			for(r=0; r<6 && mat[p][q][r]!=0; r++)
				Array[s+r]=mat[p][q][r];
			s += r;
		}
		for(r=0, q=a[1]; r<6 && mat[p][q][r]!=0; r++)
			Array[s+r]=mat[p][q][r];
		s += r;
		if(a[1]!=24){
			q=a[1]+1;
			for(r=0; r<6 && mat[p][q][r]!=0; r++)
				Array[s+r]=mat[p][q][r];
		}
	}
}

void Initialize(int mat[25][25][6], Particle Vector[1001]){
	int i;
	for(i=0; i<=1000; i++){
		Vector[i].SetSystem();
		double a[2];
		Vector[i].GetPosition(a);
		printf("%lf %lf\n", a[0], a[1]);
	}
	Fill_Matrix(mat, Vector);
	Compute_Accelerations(mat, Vector);
}

//Esta função calcula a aceleração, a densidade e a pressão na posição de
//cada partícula a partir de uma dada distribuição espacail e de velocidades

void Compute_Accelerations(int mat[25][25][6], Particle Vector[1001]){
    int i;
    for(i=0; i<=1000; i++){
		double a[2], b[2], c[2], d[2];
		int Array[54], j;
		a[0]=a[1]=0;
		Vector[i].GetPosition(b);
		for(j=0; j<54; j++) Array[j]=0;
		Pressure_Acceleration(i, a, Array, mat, Vector);
		for(j=0; j<1000; j++){
            Vector[j].GetPosition(c);
            d[0]=d[1]=0;
            Body_Acceleration(b, c, d);
            a[0] += d[0];
            a[1] += d[1];
		}
		Vector[i].GetVelocity(c);
		d[0]=d[1]=0;
		Damping_Acceleration(c, d);
		a[0] += d[0];
        a[1] += d[1];
		Vector[i].SetAcceleration(a);
		//printf("(%14.10lf, %14.10lf)\n", a[0], a[1]);
	}
	for(i=0; i<=1000; i++){
		int Array[54], j;
		for(j=0; j<54; j++) Array[j]=0;
		Vector[i].SetDensity(Density_SPH(i, Array, mat, Vector));
		Vector[i].SetPressure(K*pow(Density_SPH(i, Array, mat, Vector), 2));
	}
}

void Leap_Frog(int i, int mat[25][25][6], Particle Vector[1001]){
    if(i%2==0){
        int j;
        for(j=0; j<1000; j++){
            double a[2], b[2];
            Vector[j].GetPosition(a);
            Vector[j].GetVelocity(b);
            a[0] += b[0]*dt;
            a[1] += b[1]*dt;
            Vector[j].SetPosition(a);
            Vector[j].SetGrid_Position();
            //printf("(%14.10lf, %14.10lf)\n", a[0], a[1]);

        }
        Fill_Matrix(mat, Vector);
        Compute_Accelerations(mat, Vector);
    }
    if(i%2==1){
        int j;
        for(j=0; j<1000; j++){
            double a[2], b[2];
            Vector[j].GetVelocity(a);
            Vector[j].SetPrevious_Velocity(a);
            Vector[j].GetAcceleration(b);
            a[0] += b[0]*dt;
            a[1] += b[1]*dt;
            Vector[j].SetVelocity(a);
            //printf("(%14.10lf, %14.10lf)\n", a[0], a[1]);
        }
    }
}
