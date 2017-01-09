#include <math.h>
#include <stdio.h>

double Mass, Ni, K;	//massa, constante de proporcionalidade da força e constante de proporcionalidade da equação de estado, em kg, 1/(kg.s²) (ou J/(kg².m²)) e m^5/(kg.s²).
double dt=1E-6, h=1E-2;	//passo de tempo e largura do kernel.
double Array[201];	//armazena todas as posições das partículas num dado instante.

class Particle{
	private:
		double Position, Speed, Acceleration;
	public:
		void SetSystem(int c){
			Position=-1.0+(c/100.0);			//distribui 21 partículas igualmente espaçadas em [-1, 1].
			Speed=0;
			Acceleration=-201*Ni*Mass*Position;		//se as massas são iguais e a distribuição inicial é simétrica, a aceleração é proporcional à posição.
		}
		double GetPosition() {return Position;}
		double GetSpeed() {return Speed;}
		double GetAcceleration() {return Acceleration;}
		void SetPosition(double r){Position=r;}
		void SetSpeed(double v){Speed=v;}
		void SetAcceleration(double a){Acceleration=a;}
};

class Density{
	private:
		double Density_and_Pressure[2][201];
	public:
		void SetDensity_and_Pressure(int a, int b, double s){
			Density_and_Pressure[a][b]=s;
		}
		double GetDensity(int a) {return Density_and_Pressure[0][a];}
		double GetPressure(int a) {return Density_and_Pressure[1][a];}
		void SetDensity(int a, double d){Density_and_Pressure[0][a]=d;}
		void SetPressure(int a, double p){Density_and_Pressure[1][a]=p;}
};

double Kernel(double x, double y);
double SPH(double x, double mat[201]);

int main(){
	
	printf("Entre com a massa das particulas: ");
	scanf("%lf", &Mass);
	while(Mass<=0){
		printf("A massa deve ser positiva. Entre com a massa novamente: ");
		scanf("%lf", &Mass);
	}
	
	printf("Entre com a constante de proporcionalidade: ");
	scanf("%lf", &Ni);
	while(Ni<=0){
		printf("A constante deve ser positiva. Entre com a constante novamente: ");
		scanf("%lf", &Ni);
	}
	
	printf("Entre com a constante da equacao de estado (p=K(d^2)): ");
	scanf("%lf", &K);
	while(K<=0){
		printf("A constante deve ser positiva. Entre com a constante novamente: ");
		scanf("%lf", &K);
	}
	
	Particle System[201];
	Density Grid;
	
	//inicialização do sistema
	
	int i;
	for(i=0; i<201; i++){
		System[i].SetSystem(i);
		Array[i]=System[i].GetPosition();
	
		/*Com h=0,01 e 201 partículas em [-1, 1] (separação inicial também de 0,01), a densidade num ponto
		da grade se resume ao kernel avaliado em três partículas: a da posição observada, e as duas adjacentes.
		Logo, W(-h, h)+W(0, h)+W(h, h)=6/(6h)=1/h, e d(x)=Mass/h. Se a partícula está nas bordas, d(x)=5*Mass/6h.*/
		
		if(i!=0 && i!=200) Grid.SetDensity_and_Pressure(0, i, Mass/h);
		else if(i==0||i==200) Grid.SetDensity_and_Pressure(0, i, 5*Mass/(6*h));
		Grid.SetDensity_and_Pressure(1, i, K*pow(Grid.GetDensity(i), 2));
	}
	
	int Step;
	
	//O número de passos pode (e deve) ser alterado para ajustar o tempo de simulação, conforme a capacidade computacional da máquina que o roda.
	
	for(Step=1; Step<5E5; Step++){
		if(Step%2==0){
			int j, k;
			double r=0, a=0, d=0;
			
			//atualiza a posição num passo par, e armazena a posição atualizada em "Array"
			
			for(k=0; k<201; k++, r=0){
				r=System[k].GetPosition()+dt*System[k].GetSpeed();
				System[k].SetPosition(r);
				Array[k]=r;
			}
			
			//atualiza a densidade e a pressão num passo par, usando a aproximação do SPH
			
			if(Step==7E6){
				for(k=0; k<201; k++, d=0){
					double x=-1.0+(k/100.0);
					d=SPH(x, Array);
					Grid.SetDensity(k, d);
					Grid.SetPressure(k, K*pow(d, 2));
				}
			}
			
			//atualiza a aceleração num passo par
			
			for(k=0; k<201; k++, a=0){
				/*se a distribuição inicial é simétrica, a forma 
				da força conserva a simetria da distribuição.*/
				
				System[k].SetAcceleration(-201*Ni*Mass*System[k].GetPosition());
				
				/*for(j=0; j<201; j++)
					a += System[k].GetPosition()-System[j].GetPosition();
				a=-Ni*Mass*a;
				System[k].SetAcceleration(a);*/
			}
		}
		else{
			
			//segunda parte do Leap Frog, atualizando a velocidade em passos ímpares
			
			if(Step%2==1){
				int k;
				for(k=0; k<201; k++){
					double v=0;
					v=System[k].GetSpeed()+dt*System[k].GetAcceleration();
					System[k].SetSpeed(v);
				}
			}
		}
		
		//este desvio traz as saídas (posição, velocidade etc.) a cada vez que um certo intervalo de tempo se passa
		
		if(Step%10000==0){
			int k;
			for(k=0; k<201; k++){
				printf("Position(%d): %6.4lf, ", k, System[k].GetPosition());
				printf("Speed(%d): %6.4lf, ", k, System[k].GetSpeed());
				printf("Acceleration(%d): %6.4lf \n", k, System[k].GetAcceleration());
				//printf("Pressure(%d): %6.4lf\n", k, Grid.GetPressure(k));
				//printf("Density(%d): %6.4lf\n", k, Grid.GetDensity(k));
			}			
		}
	}
	return 0;
}

double Kernel(double x, double y){
	double q=(fabs(x-y))/h;
	if(q>=0||q<1) return (1/(6*h))*(pow(2-q, 3)-4*pow(1-q, 3));
	else if(q>=1||q<2) return (1/(6*h))*(pow(2-q, 3));
	return 0;
}

double SPH(double x, double mat[201]){
	double I=0;
	int z;
	for(z=0; z<201; z++) I += Kernel(x, mat[z]);
	return Mass*I;
}
