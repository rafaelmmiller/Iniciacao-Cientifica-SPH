#include <iostream>
#include <fstream>
using namespace std;

//leap frog method

enum Sistemas {linear,harmonico};

class Particle
	{
	private:
	float x_t;
	float velo_t;
	float a_t;
	float mass;
	
	public: 
	float Get_x_t(){return x_t;}
	float Get_velo_t(){return velo_t;}
	float Get_a_t(){return a_t;}
	float Get_mass(){return mass;}
	
	void Set_mass(float m) {mass = m;}
	void Set_x_t( float x) {x_t = x;}
	void Set_velo_t (float v){velo_t = v;}
	void Set_a_t (float a){a_t = a;}
	
	void walk(float dt, float k_stiff);
	void initialize(float x, float v, float a, float dt, float);
	
	Particle();
	~Particle();
	};
	
	Particle::Particle()
	{
	}
	
	void Particle::initialize(float x, float v, float a, float dt, float mass)
	{
		Set_a_t(a);
		Set_x_t(x);
		Set_velo_t(v-(a*dt/2));
		Set_mass(mass);
		cout << "inicializa";
	}
	void Particle::walk( float dt, float k_stiff)
	{
	
	//Calcula v
	Set_velo_t(velo_t + a_t*dt);
	
	//Calcula a nova posicao
	Set_x_t(x_t + velo_t*dt);
	
	//Calcula a aceleracao 
	Set_a_t(-k_stiff*Get_x_t()/(Get_mass()));
	
	}
	
int main()
{

double T;
double dt = 0.01;

double Position [500];
double Time [500];

cout << "Voce deseja saber a posicao da particula em que tempo? (No maximo 5. Duas casas decimais.)\n";
cin >> T;

//Cria a "particula"
Particle *P1 = new Particle();
P1->initialize(0,0.5,0,dt, 0.5);

// Loop de iteracao

for (int i; i<(T/dt); i++)
	{
		//Faz com que a particula ande
		P1->walk(dt, 100);
		
		//Coloca em arrays a posicao e o tempo
		Position [i] = P1->Get_x_t();	
		Time [i] = dt*i;
	}
//Imprime os arrays num arquivo de texto
ofstream myfile ("/home/francisco/myfile.txt");	
if (myfile.is_open())
{

	for (int count = 0; count < (T/dt); count++)
	{
		myfile << Time[count]<< " " << Position[count] << " \n";
	}
myfile.close();
}
}
