#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;
//parâmetros
const unsigned short Poly_Index = 10;
const unsigned short Dim = 2;
const unsigned int N_Part = 200;
const unsigned int Max_T = 20;
const float dt = 0.02;
const float Star_Mass = 2.0;
const float Star_Rad = 1.0;
const float Damp = 2;
const float Gravity = 10;
const float Press_K = 0.2;
const double Smooth_Len = 0.02/sqrt(N_Part/1000.0);
const double Ch[3] = {1/(6*Smooth_Len), 15/(14*M_PI*Smooth_Len*Smooth_Len), 1/(4*M_PI*pow(Smooth_Len, 3))};
const char Kernel = 'S';
//estrutura da partícula com propriedades
struct particle_prop{
	double pos[Dim];
	double vel[Dim];
	double acc[Dim];
	double mass;
	double dens;
	double v_ph[Dim];
	double v_mh[Dim];
};
//funções
double kern(double r, const double h = Smooth_Len);
double gradkern(double r, const double h = Smooth_Len);
double v_mag(double pos[]);
void init_particles(particle_prop part[]);
double press(double);
double dist(const double pos1[], const double pos2[]);
void density(particle_prop part[]);
void acceleration(particle_prop part[], const float nu=Damp, const float lambda=Gravity);
