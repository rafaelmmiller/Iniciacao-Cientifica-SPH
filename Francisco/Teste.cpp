#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/GradKernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Kernel2D.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/Teste.hpp>
#include </home/francisco/Documents/Fisica/Mecflu_IC/Programas/Headers/P_System2D.hpp>

using namespace std;

/*PARAMETROS DA SIMULACAO
 * lambda = 1
 * k = 1/4
 * N_part = 1000
 * M = pi/2
 * h = 0.1
*/


int main()
{
clock_t tStart = clock();
//smoothing lenght
double h = 0.1;

ParticleSystem * PS1 = new ParticleSystem;
PS1->initialize(0.004, h);
for (int i = 0; i< 5000; i++)
	{
		PS1->Step(0.004, h);
		PS1->Boundaries();
	}

//////////////////////////////PARTE DE EXPORTAR OS DADOS EM TXT/////////////////////////////	
printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


//EXPORTA OS DADOS PARA UM ARQUIVO
ofstream myfile2 ("/home/francisco/myfile4.txt");
if (myfile2.is_open())
{
	for (int j = 0; j<1000; j++)
		{
		std::vector<Particle*> Neighbours = PS1->Find_Neighbours(&PS1->Particles[j],h);
		myfile2 << PS1->Particles[j].GetX0()<< "," << PS1->Particles[j].GetX1() << "," << PS1->CalculateDensity(&PS1->Particles[j], Neighbours, h) << "\n";
		}
myfile2.close();
}

ofstream myfile3 ("/home/francisco/myfile5.txt");
if (myfile3.is_open())
{
	for (int k = 0; k<1000; k++)
	{
		std::vector<Particle*> Neighbours = PS1->Find_Neighbours(&PS1->Particles[k],h);
		double r = sqrt(pow(PS1->Particles[k].GetX0(),2)+pow(PS1->Particles[k].GetX1(),2));
		myfile3 << r << "," << PS1->CalculateDensity(&PS1->Particles[k],Neighbours,h) << "\n"; 
	}
}

ofstream myfile4 ("/home/francisco/myfile6.txt");
{
if (myfile4.is_open())
{	
	for (int l = 0; l<1000; l++)
	{
		myfile4 << PS1->Particles[l].GetX0() << ";" << PS1->Particles[l].GetX1() << "\n";
	}
}
}

}
