#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/GradKernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Kernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Particle2D.hpp>

using namespace std;

/*double d(Particle* p1, Particle* p2)
{
	//double d = sqrt(pow((p1->x_t[0]-p2->x_t[0]),2)+pow((p1->x_t[1]-p2->x_t[1]),2));
	return d;
}*/

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

void ParticleSystem::Boundaries()
{
	for (int i =0; i < 1000; i++)
	{
				Particle* p1 = &Particles[i];
				if (p1->GetX0() > 1)
					{
						p1->velo_t[0] = -1*p1->velo_t[0];
					}
				if (p1->GetX0()<-1)
					{
						p1->velo_t[0] = -1*p1->velo_t[0];
					}
				if (p1->GetX1() > 1)
					{
						p1->velo_t[1] = -1*p1->velo_t[1];
					}
				if (p1->GetX1() < -1)
					{
						p1->velo_t[1] = -1*p1->velo_t[1];
					}
	}
}
		
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
				
				p1->velo_t[0] = 0;
				p1->velo_t[1] = 0;
				
				p1->a_t[0] = 0;
				p1->a_t[1] = 0;
// INITIALIZE FOI TESTADA ESTA FUNCIONANDO BEM
	}
}

double ParticleSystem::CalculateDensity(Particle* p1, double h)
{
double A = 0;
for(int i =0; i<1000; i++)
{
						//A = A + Particles[i].GetMass()*Kernel2D(sqrt(pow((p1->x_t[0]-Particles[i].x_t[0]),2)+pow((p1->x_t[1]-Particles[i].x_t[1]),2))/h,h);
						cout << "A " << A << " d " << Particles[i].GetX1();
}
return A;
}
//kernel esta ok
/*
double ParticleSystem::CalculateDensityGradX(Particle* p1,double h)
{
double A =0;
for (int i =0; i <1000; i++)
{
		if (Particles[i].x_t[0] > (p1->x_t[0]-2*h) && Particles[i].x_t[0] < (p1->x_t[0]+2*h))
			{
				if (Particles[i].x_t[1] > p1->x_t[1]-2*h && Particles[i].x_t[1] < p1->x_t[1]+2*h)
				{	
					//cout << "posicao " << p1->x_t[0] << " grad "   << GradKernel2DX(p1->x_t[0]-Particles[i].x_t[0],p1->x_t[1]-Particles[i].x_t[1],h) << "\n";
					A = A + Particles[i].GetMass()*GradKernel2DX(p1->x_t[0]-Particles[i].x_t[0],p1->x_t[1]-Particles[i].x_t[1],h);
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
		if (Particles[i].x_t[0] > (p1->x_t[0]-2*h) && Particles[i].x_t[0] < (p1->x_t[0]+2*h))
			{
				if (Particles[i].x_t[1] > (p1->x_t[1]-2*h) && Particles[i].x_t[1] < (p1->x_t[1]+2*h))
				{
						A = A + Particles[i].GetMass()*GradKernel2DY(p1->x_t[0]-Particles[i].x_t[0],p1->x_t[1]-Particles[i].x_t[1],h);
				}
			}
}
return A;
}
*/
void ParticleSystem::Step(double dt)
{
	for (int i = 0; i< 1000; i++)
		{
				Particle* p1 = &Particles[i];
				double A1 = p1->a_t[0];
				double A2 = p1->a_t[1];
	
				p1->a_t[0] = -(0.5)*CalculateDensityGradX(p1,0.12) - p1->GetX0()-0.2*(p1->velo_t[0]);
				p1->a_t[1] = -(0.5)*CalculateDensityGradY(p1,0.12) - p1->GetX1()-0.2*(p1->velo_t[1]);
				
				p1->velo_t[0] = p1->velo_t[0] + 0.5*(A1 + p1->a_t[0])*dt;
				p1->velo_t[1] = p1->velo_t[1] + 0.5*(A2 + p1->a_t[1])*dt;
				
				p1->SetX0(p1->GetX0() + p1->velo_t[0]*dt + 0.5*A1*pow(dt,2));
				p1->SetX1(p1->GetX1() + p1->velo_t[1]*dt + 0.5*A2*pow(dt,2));
			}
}


int main()
{
ParticleSystem * PS1 = new ParticleSystem;
PS1->initialize();
for (int i = 0; i<200; i++)
	{
		
		PS1->Step(0.01);
		PS1->Boundaries();
	}
for (int j =0; j<2; j++)
{
	Particle* p1= &PS1->Particles[j];
	cout << "densidade" << PS1->CalculateDensity(p1,0.1) << endl;
	
}

/*
ofstream myfile2 ("/home/francisco/myfile2.txt");
if (myfile2.is_open())
{
	for (int j = 0; j<1000; j++)
		{
		myfile2 << PS1->Particles[j].x_t[0] << "," << PS1->Particles[j].x_t[1] << "," << PS1->CalculateDensity(&PS1->Particles[j],0.12) << "\n";
		}
myfile2.close();
}
*/
}


		
		
		
		
	
	
	
	
	
	
	
	
	
	
	
