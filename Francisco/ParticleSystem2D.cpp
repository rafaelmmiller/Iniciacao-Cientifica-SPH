#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/GradKernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Kernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Particle2D.hpp>

using namespace std;

class ParticleSystem
	{
	public:
		//constructor
		ParticleSystem();
		~ParticleSystem();
		
		//Inicializa a posicao, velocidade e aceleracao das particulas
		void initialize();
		
		//Faz com que o sistema de um passo temporal
		void Step(double dt);
		void Boundaries();
		///////////////////////////////////
		double CalculateDensity(Particle * p1, double h);
		double CalculateDensityGradX(Particle * p1, double h);
		double CalculateDensityGradY(Particle * p1, double h);
		Particle Particles[1000];
};
ParticleSystem::ParticleSystem()
{
}	

//FRONTEIRA CHECA O SENTIDO DA VELOCIDADE E TROCA, SE NECESSARIO
void ParticleSystem::Boundaries()
{
	for (int i =0; i < 1000; i++)
	{
				Particle* p1 = &Particles[i];
				if (p1->GetX0() > 1)
					{
						if (p1->GetV0() > 0)
						{
							p1->SetV0(-1*p1->GetX0());
						}
					}
				if (p1->GetX0()<-1)
					{	
						if (p1->GetV0() < 0)
						{
							p1->SetV0(-1*p1->GetV0());
						}
					}
				if (p1->GetX1() > 1)
					{
						if (p1->GetV1()>0)
						{
							p1->SetV1(-1*p1->GetV1());
						}
					}
				if (p1->GetX1() < -1)
					{
						if (p1->GetV1()<0)
						{
							p1->SetV1(-1*p1->GetV1());
						}
					}
	}
}

//INICIALIZA ALEATORIAMENTE A POSICAO DAS PARTICULAS NUM QUADRADO DE LADO 2		
void ParticleSystem::initialize()
{
	srand(time(NULL));
	double m = pi/2000.0;
	for (int i = 0; i<1000; i++)
	{
				Particle* p1 = &Particles[i];
				
				p1->SetMass(m);
				
				p1->SetX0((rand() % 20000)/10000.0 - 1);
				p1->SetX1((rand() % 20000)/10000.0 - 1);
				
				p1->SetV0(0);
				p1->SetV1(0);
				
				p1->SetA0(0);
				p1->SetA1(0);
	}
}

double ParticleSystem::CalculateDensity(Particle* p1, double h)
{
double A = 0;
for(int i =0; i<1000; i++)
{
	if (Particles[i].GetX0() > (p1->GetX0()-2*h) && Particles[i].GetX0() < (p1->GetX0()+2*h))
			{
				if (Particles[i].GetX1()> p1->GetX1()-2*h && Particles[i].GetX1() < p1->GetX1()+2*h)
				{
						A = A + Particles[i].GetMass()*Kernel2D(sqrt(pow((p1->GetX0()-Particles[i].GetX0()),2)+pow((p1->GetX1()-Particles[i].GetX1()),2))/h,h);
				}
			}
}
return A;
}

double ParticleSystem::CalculateDensityGradX(Particle* p1,double h)
{
double A =0;
for (int i =0; i <1000; i++)
{
	if (Particles[i].GetX0() > (p1->GetX0()-2*h) && Particles[i].GetX0() < (p1->GetX0()+2*h))
			{
				if (Particles[i].GetX1()> p1->GetX1()-2*h && Particles[i].GetX1() < p1->GetX1()+2*h)
				{
					A = A + Particles[i].GetMass()*GradKernel2DX(p1->GetX0()-Particles[i].GetX0(),p1->GetX1()-Particles[i].GetX1(),h);
				}
			}
}
return A;
}

double ParticleSystem::CalculateDensityGradY(Particle* p1,double h)
{
double A =0;
for (int i =0; i <1000; i++)
{
	if (Particles[i].GetX0() > (p1->GetX0()-2*h) && Particles[i].GetX0() < (p1->GetX0()+2*h))
			{
				if (Particles[i].GetX1()> p1->GetX1()-2*h && Particles[i].GetX1() < p1->GetX1()+2*h)
				{
					A = A + Particles[i].GetMass()*GradKernel2DY(p1->GetX0()-Particles[i].GetX0(),p1->GetX1()-Particles[i].GetX1(),h);
				}
			}
}
return A;
}

void ParticleSystem::Step(double dt)
{
	for (int i = 0; i< 1000; i++)
		{
				Particle* p1 = &Particles[i];
				double A1 = p1->GetA0();
				double A2 = p1->GetA1();
	
				p1->SetA0(-(0.5)*CalculateDensityGradX(p1,0.12) - p1->GetX0()-0.2*(p1->GetV0()));
				p1->SetA1(-(0.5)*CalculateDensityGradY(p1,0.12) - p1->GetX1()-0.2*(p1->GetV1()));
				
				p1->SetV0(p1->GetV0() + 0.5*(A1 + p1->GetA0())*dt);
				p1->SetV1(p1->GetV1() + 0.5*(A2 + p1->GetA1())*dt);
				
				p1->SetX0(p1->GetX0() + p1->GetV0()*dt + 0.5*A1*pow(dt,2));
				p1->SetX1(p1->GetX1() + p1->GetV1()*dt + 0.5*A2*pow(dt,2));
				
				//ITERACAO TEMPORAL NO METODO LEAPFROG FEITO EM UMA UNICA ETAPA
			}
}


int main()
{
ParticleSystem * PS1 = new ParticleSystem;
PS1->initialize();
for (int i = 0; i<1000; i++)
	{
		
		PS1->Step(0.01);
		PS1->Boundaries();
	}

	


//EXPORTA OS DADOS PARA UM ARQUIVO
ofstream myfile2 ("/home/francisco/myfile2.txt");
if (myfile2.is_open())
{
	for (int j = 0; j<1000; j++)
		{
		
		myfile2 << PS1->Particles[j].GetX0()<< "," << PS1->Particles[j].GetX1() << "," << PS1->CalculateDensity(&PS1->Particles[j],0.12) << "\n";
		}
myfile2.close();
}

}
