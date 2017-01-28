#include <iostream>

class Particle
{
	
	
	public:
	const double GetMass();
	void SetMass(double m);
	
	const double GetX0();
	const double GetX1();
	
	const double GetV0();
	const double GetV1();
	
	const double GetA0();
	const double GetA1();
	
	void SetX0(double x0);
	void SetX1(double x1);
	
	void SetV0(double v0);
	void SetV1(double v1);
	
	void SetA0(double a0);
	void SetA1(double a1);
	
	Particle();
	~Particle();
	
	private:
	double mass;
	
	double x_t[2];
	double velo_t[2];
	double a_t[2];
};
void Particle::SetX0(double x0)
{
		x_t[0] = x0;
}

void Particle::SetX1(double x1)
{
		x_t[1] = x1;
}

void Particle::SetV0(double v0)
{
	velo_t[0] = v0;
}

void Particle::SetV1(double v1)
{
	velo_t[1] = v1;
}

void Particle::SetA0(double a0)
{
	a_t[0] = a0;
}

void Particle::SetA1(double a1)
{
	a_t[1] = a1;
}

const double Particle::GetX0()
{
	return x_t[0];
}

const double Particle::GetX1()
{
	return x_t[1];
}

const double Particle::GetV0()
{
	return velo_t[0];
}

const double Particle::GetV1()
{
	return velo_t[1];
}

const double Particle::GetA0()
{
	return a_t[0];
}

const double Particle::GetA1()
{
	return a_t[1];
}

void Particle::SetMass(double m)
{
	mass = m;
}
const double Particle::GetMass()
{
	return mass;
}

Particle::Particle()
	{
		mass = 0.0015707963;
	}
