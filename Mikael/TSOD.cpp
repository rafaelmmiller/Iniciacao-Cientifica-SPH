#include <math.h>
#include <stdio.h>

double Mass, Ni;	//massa e constante de proporcionalidade da força, em kg e 1/(kg.s²) (ou J/(kg².m²)).
double dt=1E-6;

class Particle{
	private:
		double Position, Speed, Acceleration;
	public:
		void SetSystem(int c){
			Position=-1.0+(c/100.0);			//distribui 201 partículas igualmente espaçadas em [-1, 1].
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
	
	Particle System[201];
	int i;
	for(i=0; i<201; i++) System[i].SetSystem(i);
	
	int Step;
	
	for(Step=1; Step<2E4; Step++){
		if(Step%2==0){
			int j, k;
			double r=0, a=0;
			for(k=0; k<201; k++, r=0){	//atuaiza a posição num passo par
				r=System[k].GetPosition()+dt*System[k].GetSpeed();
				System[k].SetPosition(r);
			}
			for(k=0; k<201; k++, a=0){	//atualiza a aceleração num passo par
				for(j=0; j<201; j++)
					a += System[k].GetPosition()-System[j].GetPosition();
				a=-Ni*Mass*a;
				System[k].SetAcceleration(a);
			}
		}
		else{
			if(Step%2==1){
				int k;
				for(k=0; k<201; k++){
					double v=0;
					v=System[k].GetSpeed()+dt*System[k].GetAcceleration();
					System[k].SetSpeed(v);
				}
			}
		}
		if(Step%1000==0){
			int k;
			for(k=0; k<201; k++){
				/*
				printf("Position(%d): %6.4lf, ", k, System[k].GetPosition());
				printf("Speed(%d): %6.4lf, ", k, System[k].GetSpeed());
				printf("Acceleration(%d): %6.4lf \n", k, System[k].GetAcceleration());
				*/
				printf("%lf %lf\n", System[k].GetPosition(), Step*dt);
			}			
		}
	}
	return 0;
}
