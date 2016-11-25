#include <iostream>
#include <cmath>
#include <fstream>
///////////////////////////////
//Codigo para aproximacao de uma funcao pelo metodo SPH

using namespace std;


//kernel parece com funcao delta de dirac
double Kernel(double q, double h)
	{
		double R = 0;
		if (0<=q && q<1)
			{
				R = (pow((2-q),3)-4*pow((1-q),3))/(6*h);
			}
		if ((1<=q && q<2 ))
			{
				R = (pow((2-q),3))/(6*h);
			}
		if (2<=q)
			{
				R = 0;
			}
			return (R);
	}
//funcao a ser aproximada
double func(double y)
	{
		return (sin(y));
	}
//funcao Aprox aproxima a quantidade desejada
double Aprox(double x, double h)
	{
	double A = 0;
	double dx = h*0.001;
	for ( int i = (-2/dx); i < (2/dx); i++)
		{
			A = A + func(x+i*dx)*Kernel(std::abs(i*dx/h), h)*dx;
		}
	return A;
	}
int main()
{
	//ofstream out ("output.txt")
	
	double Error = abs((Aprox(3.1415/6,0.1)-func(3.1415/6)));
	cout << Aprox(3.1415/6,0.1) << " Foi o valor obtido atraves da aproximacao SPH"<<"\n";
	cout << func(3.1415/6) << " Foi o valor obtido pela funcao matematica \n";
	cout << "O erro cometido na aproximacao foi de " << Error ;
}


