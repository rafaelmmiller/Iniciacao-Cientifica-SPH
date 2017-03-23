#ifndef PSYS2D_HPP
#define PSYS2D_HPP

#include <vector>
#include <cmath>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/GradKernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Kernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Teste.hpp>
#include <stdlib.h>

/*PARAMETROS 
 * NPART = 1000*/




class ParticleSystem
	{
	public:
		//constructor
		ParticleSystem();
		~ParticleSystem();
		
		//Inicializa a posicao, velocidade e aceleracao das particulas
		void initialize(double dt, double h);
		
		//Faz com que o sistema de um passo temporal
		void Step(double dt, double h);
		void Boundaries();
		//Funcoes calculam densidades e gradientes
		double CalculateDensity(Particle * p1, std::vector<Particle*> Grid, double h);
		double CalculateDensityGradX(Particle * p1, std::vector<Particle*> Grid, double h);
		double CalculateDensityGradY(Particle * p1, std::vector<Particle*> Grid, double h);
		
		//Funcao achar os vizinhos. Retorna vetor
		std::vector<Particle*> Find_Neighbours(Particle* p1, double h);
		
		//Array contendo todas as particulas
		Particle Particles[1000];
};
ParticleSystem::ParticleSystem()
{
}	

///////////ENCONTRA AS PARTICULAS VIZINHAS////////////////////////
std::vector<Particle*> ParticleSystem::Find_Neighbours (Particle* p1, double h)
{

std::vector<Particle*> Grid;	

for (int i = 0; i < 1000; i++)
{
if (Particles[i].GetX0() > (p1->GetX0()-2*h) && Particles[i].GetX0() < (p1->GetX0()+2*h))
	{
		if (Particles[i].GetX1()> p1->GetX1()-2*h && Particles[i].GetX1() < p1->GetX1()+2*h)
		{
			Grid.push_back(&Particles[i]);
		}
	}
}
return Grid;	
}
//FRONTEIRA CHECA O SENTIDO DA VELOCIDADE E TROCA, SE NECESSARIO
void ParticleSystem::Boundaries()
{
	for (int i =0; i < 1000; i++)
	{
				if (Particles[i].GetX0() > 1)
					{
						if (Particles[i].GetV0() > 0)
						{
							Particles[i].SetV0(-0.6*Particles[i].GetV0());
						}
					}
				if (Particles[i].GetX0()< -1)
					{
						if (Particles[i].GetV0() < 0)
						{
							Particles[i].SetV0(-0.6*Particles[i].GetV0());
						}
					}
				if (Particles[i].GetX1() > 1)
					{
						if (Particles[i].GetV1()>0)
						{
							Particles[i].SetV1(-0.6*Particles[i].GetV1());
						}
					}
				if (Particles[i].GetX1() < -1)
					{
						if (Particles[i].GetV1()<0)
						{
							Particles[i].SetV1(-0.6*Particles[i].GetV1());
						}
					}
	}
}

//////////////IMPLEMENTACAO DAS FUNCOES DE DENSIDADE //////////////////////////////////////
double ParticleSystem::CalculateDensity(Particle* p1,std::vector<Particle*> Grid, double h)
{
double A = 0;
for (unsigned int i =0; i < Grid.size(); i++)
{
		A = A + Grid[i]->GetMass()*Kernel2D(sqrt(pow((p1->GetX0()-Grid[i]->GetX0()),2)+pow((p1->GetX1()-Grid[i]->GetX1()),2))/h,h);
}
return A;
}

double ParticleSystem::CalculateDensityGradX(Particle* p1, std::vector<Particle*> Grid, double h)
{
double A =0;
for (unsigned int i =0; i < Grid.size(); i++)
{
		A = A + Grid[i]->GetMass()*GradKernel2DX(p1->GetX0()-Grid[i]->GetX0(),p1->GetX1()-Grid[i]->GetX1(),h);
}
return A;
}

double ParticleSystem::CalculateDensityGradY(Particle* p1,std::vector<Particle*> Grid,double h)
{
double A =0;
for (unsigned int i =0; i <Grid.size(); i++)
{
		A = A + Grid[i]->GetMass()*GradKernel2DY(p1->GetX0()-Grid[i]->GetX0(),p1->GetX1()-Grid[i]->GetX1(),h);
}
return A;
}

void ParticleSystem::initialize(double dt, double h)
{
	srand(time(NULL));
	double m = (pi/2)/1000.0;
	for (int i = 0; i<1000; i++)
	{
				Particle* p1 = &Particles[i];
				
				std::vector<Particle*> Grid = Find_Neighbours(p1,h);
				
				p1->SetMass(m);
				
				p1->SetX0((rand() % 20000)/10000.0 - 1);
				p1->SetX1((rand() % 20000)/10000.0 - 1);
				
				p1->SetA0(-(0.5)*CalculateDensityGradX(p1,Grid,h) -p1->GetX0());
				p1->SetA1(-(0.5)*CalculateDensityGradY(p1,Grid,h) -p1->GetX1());
				
				p1->SetV0(p1->GetA0()*(dt/2));
				p1->SetV1(p1->GetA1()*(dt/2));
	}
}

void ParticleSystem::Step(double dt, double h)
{
	for (int i = 0; i< 1000; i++)
		{
				Particle* p1 = &Particles[i];
				
				std::vector<Particle*> Grid = Find_Neighbours(p1,h);
				
				double X0_prev = p1->GetX0();
				double X1_prev = p1->GetX1();
				
				p1->SetX0(p1->GetX0() + p1->GetV0()*dt);
				p1->SetX1(p1->GetX1() + p1->GetV1()*dt);
				
				p1->SetA0(-(0.5)*CalculateDensityGradX(p1,Grid,h) -X0_prev -0.5*(p1->GetV0()));
				p1->SetA1(-(0.5)*CalculateDensityGradY(p1,Grid,h) -X1_prev -0.5*(p1->GetV1()));
				
				p1->SetV0(p1->GetV0() + p1->GetA0()*dt);
				p1->SetV1(p1->GetV1() + p1->GetA1()*dt);
				
		}
}

#endif
