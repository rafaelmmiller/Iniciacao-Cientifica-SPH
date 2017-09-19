#include "header.hpp"

int main()
{
	particle_prop ps[N_Part]={{0},{0},{0},0,0,{0},{0}};
	double maxdens;
	srand(static_cast <unsigned> (time(0)));
	init_particles(ps);
	acceleration(ps);
	cout << fixed << "start{";
	for(double t=0; t<Max_T; t+=dt){
		maxdens=0;
		for(unsigned int i=0; i<N_Part; i++)
			if(ps[i].dens > maxdens)
				maxdens = ps[i].dens;
		cout << "{";
		for(unsigned int i=0; i<N_Part; i++){  //printar para o Mathematica
			cout << "{" << ps[i].pos[0] << "," << ps[i].pos[1] << "," << ps[i].dens/maxdens << "}";
			if(i<N_Part-1) cout << ",";
		}
			for(unsigned int i=0; i<N_Part; i++)   //leapfrog
				for(int j=0; j<Dim; j++){
					ps[i].v_ph[j] = ps[i].v_mh[j] + ps[i].acc[j]*dt;
					ps[i].pos[j] += ps[i].v_ph[j]*dt;
					ps[i].vel[j] = (ps[i].v_mh[j] + ps[i].v_ph[j])/2;
					ps[i].v_mh[j] = ps[i].v_ph[j];
			}
		acceleration(ps);
		cout << "}";
		if(t<Max_T-dt) cout << ",";
	}
	cout << "}end" << endl;
	ofstream profile;
	profile.open("rho_profile.dat");
	for(unsigned int i=0; i<N_Part; i++)
		if(ps[i].dens >= 0.6*maxdens)
			profile << ps[i].pos[1]/Star_Rad << "\t" << ps[i].dens/maxdens << endl;
	profile.close();
	return 0;
}

void acceleration(particle_prop part[], float nu, float lambda)   //aceleração:
{								  //eq.dif. dv/dt para cada partícula
	density(part);
	for(unsigned int i=0; i<N_Part; i++){
		for(int j=0; j<Dim; j++){
			part[i].acc[j] = -nu*part[i].vel[j]-lambda*part[i].pos[j];
		}
	}
	double r, sum[Dim], pressure;
	for (unsigned int i=0; i<N_Part; i++){
		sum[0]=sum[1]=0;
		for (unsigned int j=0; j<N_Part; j++)
			if(j!=i){
				r = dist(part[i].pos,part[j].pos);
				pressure = press(part[i].dens)/(part[i].dens*part[i].dens)+press(part[j].dens)/(part[j].dens*part[j].dens);
				for(int d=0; d<Dim; d++)
					sum[d] += part[j].mass*pressure*gradkern(r)*(part[i].pos[d]-part[j].pos[d])/r;
			}
		for(int d=0; d<Dim; d++)
			part[i].acc[d] -= sum[d];
	}

}

double kern(double r, const double h)    //kernel
{
	double W, q=fabs(r)/h;
	if(Kernel == 'S'){
		if(q<1)
			W = pow((2-q),3)-4*pow((1-q),3);
		else if(q<2)
			W = pow((2-q),3);
		else
			W = 0;
		W *= Ch[Dim-1];
	}
	else if(Kernel == 'G')
		W = pow((1/(h*sqrt(M_PI))),Dim)*exp(-q*q);
	return W;
}

double gradkern(double r, const double h)  //gradiente do kernel, faltando dr/dx
{
	double W, q=fabs(r)/h;
	if(Kernel == 'S'){
		if(q<1)
			W = 12*(1-q)*(1-q) - 3*(2-q)*(2-q);
		else if(q<2)
			W = -3*(2-q)*(2-q);
		else
			W = 0;
		W *= Ch[Dim-1];
	}
	else if(Kernel == 'G')
		W = -2*pow((1/(h*sqrt(M_PI))),Dim)*exp(-q*q);
	return W/h;
}

double v_mag(double pos[]) //magnitude de um vetor
{
	double sum=0;
	for(int i=0; i<Dim; i++)
		sum+=pos[i]*pos[i];
	return sqrt(sum);
}

void init_particles(particle_prop part[])  //inicializa (ainda ta quadrado)
{
	for(unsigned int i=0; i<N_Part; i++){
		for(unsigned short j=0; j<Dim; j++){
			part[i].pos[j] = (rand()*2./RAND_MAX-1.)*Star_Rad;
			part[i].vel[j] = 0;
		}
		part[i].mass = Star_Mass/N_Part;
	}
	density(part);
}

double dist(const double pos1[], const double pos2[]) //distância entre dois vetores
{
	double test[Dim];
	for(int i=0; i<Dim; i++)
		test[i]=pos1[i]-pos2[i];
	return v_mag(test);
}

void density(particle_prop part[])   //densidade
{
	double r;
	for(unsigned int i=0; i<N_Part; i++){
		part[i].dens=r=0;
		for(unsigned int j=0; j<N_Part; j++){
			r = dist(part[i].pos, part[j].pos);
			part[i].dens += part[j].mass*kern(r);
		}
	}
}

double press(double rho) //pressão em função da densidade
{
	return Press_K*pow(rho, 1+1./Poly_Index);
}
