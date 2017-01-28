
#ifndef KERNEL2D_H
#define KERNEL2D_H

#include <cmath>


double Kernel2D(double q, double h)
	{	
		double R = 0;
		if (0<=q && q<1)
			{
				R = (pow((2-q),3)-4*pow((1-q),3))*(15/(14*pi*pow(h,2)));
			}
		if ((1<=q && q<2 ))
			{
				R = (pow((2-q),3))*(15/(14*pi*pow(h,2)));
			}
		if (2<=q)
			{
				R = 0;
			}
		return (R);
	}

#endif

