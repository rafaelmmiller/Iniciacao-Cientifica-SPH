#ifndef GRADKERNEL2D_H
#define GRADKERNEL2D_H

#include <cmath>
const double pi = 3.14159265358979323846264338328;


double GradKernel2DX (double x, double y, double h)
{
	double R = 0;
	double q = sqrt(pow(x,2)+pow(y,2))/h;
	if (q == 0)
	{
		return (0);
	} 
	if (q>=0 && q<1.0 )
		{	
			R = ((-3*x/(q*pow(h,2)))*pow((2-q),2)+(12*x/(q*pow(h,2)))*pow(1-q,2))*(15/(14.0*pi*pow(h,2)));
		}
	if ( 1<=q && q<2)
		{
			R = (-3*x/(q*pow(h,2)))*pow(2-q,2)*(15/(14*pi*pow(h,2)));
		}
	if (2<=q)
		{
			R = 0;
		}
	return (R);
}

double GradKernel2DY (double x, double y, double h)
{
	double R = 0;
	double q = sqrt(pow(x,2)+pow(y,2))/h;
	if ( q == 0);
	{
		return (0);
	}
	if (q<1 && q>=0)
		{
			R = ((-3*y/(q*pow(h,2)))*pow((2-q),2)+(12*y/(q*pow(h,2)))*pow(1-q,2))*(15/(14*pi*pow(h,2)));
		}
	if ( q>=1 && q<=2)
		{
			R = (-3*y/(q*h))*pow(2-q,2)*(15/(14*pi*pow(h,2)));
		}
	if (2<=q)
		{
			R = 0;
		}
	return (R);
}
#endif //GRADKERNEL2D_H
