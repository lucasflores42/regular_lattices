#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "pointers.h"
#include "regular_lattices.h"
#include "mc.h"

/********************************************************************
***                          C.I.                                 ***
********************************************************************/	//gcc dilema.c -lm -lgsl -lgslcblas ---- ./a.out r gama delta rede L ruido
#define dens_aleat
#define dens_inic_c  0.25
#define dens_inic_p  0.25
#define dens_inic_pc 0.
#define dens_inic_dc 0.

//#define dens_quad
//#define dens_quad2
//#define dens_listras

/********************************************************************
***                          Plot                                 ***
********************************************************************/
#define snapshot_gnuplot    
//#define snapshot_gif
//#define snapshot_hexa
//#define snapshot_kagome
#define fps 000.0100005 // fps^-1
//#define densidade_arquivo
//#define densidade_terminal	


/********************************************************************
***                        Topologia                              ***
********************************************************************/
//#define viz_well_mixed
#define viz_espacial

//#define topo_aleat
#define topo_fixa

/********************************************************************
***                          Jogo                                 ***
********************************************************************/
//#define PRISONERS_DILEMMA
#define PGG_FOCAL
//#define PGG



enum tipo_rede {UNIDIMENSIONAL,QUADRADA, CUBICA, QUADRIDIMENSIONAL, HEXAGONAL, KAGOME,
					TRIANGULAR, MOORE};
enum tipo_rede REDE_ATUAL;
enum tipo_estrategia {DEFECTOR, COOPERATOR, PUNISHER};


int L;

//#define NORMALIZED_PAYOFFS

//#define RUIDO 0.001
double RUIDO;
//#define DEBUG

#define prob_mobil 0.
#define EPSILON (1e-8)

const int tmax=100000;//5000;
unsigned long rseed;
const gsl_rng_type * T;
gsl_rng * rand_vec;
double measure_time = 1;


long long int N;
long int L2;//= L*L; 
long int L3;// = L*L*L;
long long int L4;// = L*L*L*L; 

int G; // =4 (rede hexagonal) =5 (rede quadrada / kagome) =7 (rede triangular / quadrada 3D) =9 (rede moore / quadrada 4D)
double INV_G_RUIDO; //1/(G*RUIDO)
double INV_RUIDO;
#ifdef PRISONERS_DILEMMA
double TT, RR, PP, SS;
#endif

int ND=0;
int NC=0;
int NP=0;
int NPC=0;
int NDC=0;

int nd=0;
int nc=0;
int np=0;
int ndc=0;
int npc=0;

double investimento_total;

void set_gsl_rng(void)
{
#ifdef DEBUG
	rseed=424244432323222;
#else
	rseed=time(NULL);
#endif
  
	gsl_rng_env_setup();
	T    = gsl_rng_default;
	rand_vec = gsl_rng_alloc (T);
	gsl_rng_set (rand_vec, rseed);

  return;
}
/********************************************************************
***                            C.I.                               ***
********************************************************************/		
void calculo_ci(int state[N], int **viz)
{
	int n;

	for(n=0; n < N; n++)
	{	

		#ifdef dens_aleat
		double temp = gsl_rng_uniform(rand_vec);
		
		if(temp < dens_inic_c ) state[n] = COOPERATOR;
		else 
		{
			if(temp < dens_inic_c + dens_inic_p) state[n] = PUNISHER;
			else 
			{
				if(temp < dens_inic_c + dens_inic_p + dens_inic_pc ) state[n] = 3;
				else
				{	
					if(temp < dens_inic_c + dens_inic_p + dens_inic_pc + dens_inic_dc) state[n] = 4;
					else state[n] = DEFECTOR;
				}
			}			
		}
		#endif
	
		#ifdef dens_quad
		int i,j;
		j=n%L;
		i=n/L;
		state[n] = DEFECTOR;
		if(j>1*L/5 && j<2*L/5 && i>1*L/5 && i<2*L/5){state[j+i*L] = COOPERATOR;}
		if(j>3*L/5 && j<4*L/5 && i>1*L/5 && i<2*L/5)
		{
			double temp = gsl_rng_uniform(rand_vec);
			if(temp < 0.5) {state[j+i*L] = PUNISHER;}
			else{state[j+i*L] = COOPERATOR;}
		}
		if(j>1*L/5 && j<2*L/5 && i>3*L/5 && i<4*L/5)
		{
			double temp = gsl_rng_uniform(rand_vec);
			if(temp < 0.5) {state[j+i*L] = PUNISHER;}
			else{state[j+i*L] = COOPERATOR;}
		}
		if(j>3*L/5 && j<4*L/5 && i>3*L/5 && i<4*L/5){state[j+i*L] = PUNISHER;}
		#endif

		#ifdef dens_quad2
		int i,j;
		j=n%L;
		i=n/L;
		state[n] = DEFECTOR;
		if(j>L/3 && j<2*L/3 && i>L/3 && i<2*L/3){state[j+i*L] = PUNISHER;}
		#endif

		#ifdef dens_listras
		int i,j;
		j=x%L;
		i=x/L;
		if(i<L/3){state[j+i*L] = DEFECTOR;}
		if(i>=L/3 && i<2*L/3){state[j+i*L] = PUNISHER;}	
		if(i>=2*L/3){state[j+i*L] = DEFECTOR;}
		#endif
		
	//state[n] = 0;
	}
	int ss = 4; //state[ss] = 0;
	//quadrada
	/*state[ss] = 1;
	state[viz[ss][1]] = 1;
	state[viz[ss][2]] = 1;
	state[viz[ss][3]] = 1;
	state[viz[ss][4]] = 1;		
	state[viz[viz[ss][1]][2]] = 1;
	state[viz[viz[ss][1]][4]] = 1;
	state[viz[viz[ss][3]][4]] = 1; 	
	state[viz[viz[ss][2]][3]] = 1;
	state[viz[viz[ss][1]][1]] = 1; 	
	state[viz[viz[ss][2]][2]] = 1; 
	state[viz[viz[ss][4]][4]] = 1; 
	state[viz[viz[ss][3]][3]] = 1;*/

	//kagome	
	/*state[ss] = 1;
	state[viz[ss][1]] = 1;
	state[viz[ss][2]] = 1;
	state[viz[ss][3]] = 1;
	state[viz[ss][4]] = 1;		
	state[viz[viz[ss][1]][2]] = 1;
	state[viz[viz[ss][1]][4]] = 1;
	state[viz[viz[ss][3]][1]] = 1; 
	state[viz[viz[ss][2]][3]] = 1;
	state[viz[viz[ss][2]][4]] = 1; 	
	state[viz[viz[ss][4]][1]] = 1; 
	state[viz[viz[ss][4]][2]] = 1; 
	state[viz[viz[ss][3]][3]] = 1;*/
	
	return;
	
}
/********************************************************************
***                  distribuição da topologia                    ***
********************************************************************/
void calculo_topologia(int topologia[N])
{
	int i;
	for(i=0;i<N;i++)
	{
		#ifdef topo_aleat
		double temp = gsl_rng_uniform(rand_vec);
		if( temp < 1/3. ) topologia[n] = QUADRADA; //ou KAGOME
		else
		{
			if( temp < 2/3.) topologia[n] = TRIANGULAR; // ou CUBICA
			else 
				topologia[n] = MOORE;
		}
		#endif

		#ifdef topo_fixa		
		topologia[i] = G;
		#endif
		//printf("%d\n",topologia[i]);
	}
}


/********************************************************************
***                  distribuição da contribuiçao                 ***
********************************************************************/
void calculo_ci_investimento(double  *investimento, int state[N])
{	// investimento do P dividir por valor do estado (2)
	int i;
	for(i=0;i<N;i++)
	{
		//double gaussian = 1*gsl_rng_uniform(rand_vec) + 1*gsl_rng_uniform(rand_vec);// + 1*gsl_rng_uniform(rand_vec) + 1*gsl_rng_uniform(rand_vec) + 1*gsl_rng_uniform(rand_vec);
		//double k = 1*gsl_rng_uniform(rand_vec);
		//investimento[i] = 1.;
		
		if(state[i] == 0){investimento[i] = 0.;}
		if(state[i] == 1){investimento[i] = 1.;}
		if(state[i] == 2){investimento[i] = 0.5;}		
		
		/*int jj,j;
		j=i%L;
		jj=i/L;
		if(jj<L/3){investimento[j+jj*L] = 1;}
		if(jj>=L/3 && jj<2*L/3){investimento[j+jj*L] = k;}	
		if(jj>=2*L/3){investimento[j+jj*L] = 0;}*/
		
		//printf(" %d %d %lf\n",i,state[i],investimento[i]);
	}
}
/********************************************************************
***           		          pool do grupo                       ***
********************************************************************/
void calculo_investimento( int state[N], int sitio2, int sitio, int vizinho, int **viz, int topologia[N], double *investimento)
{

	int i;
	investimento_total = 0.;

	#ifdef viz_espacial

		for(i=0;i<topologia[sitio2];i++)
		{
			investimento_total += state[viz[sitio2][i]]*investimento[viz[sitio2][i]];
		}
	#endif
	#ifdef viz_well_mixed

		for(i=2;i<topologia[sitio2];i++) //i=1 se nao conta o invadido e i=2 se sim
		{
			int k;
			k = (int) N*gsl_rng_uniform(rand_vec);
			investimento_total += state[k]*investimento[k];

		}
		//conta sitio invadido
		//comentar if/else se i=1
		if(sitio2 != viz[sitio][G-1])
		{
			int k;
			k = (int) N*gsl_rng_uniform(rand_vec);
			investimento_total += state[k]*investimento[k];				

		}
		else 
		{
			investimento_total += state[vizinho]*investimento[vizinho];
			//printf("%d %d\n",sitio2,viz[sitio][G-1]);
		}
		//conta o proprio sitio
		investimento_total += state[sitio]*investimento[sitio];

	#endif

	return;
}
/********************************************************************
***                  numero de cada estrategia                    ***
********************************************************************/
void calculo_numero( int state[N], int sitio2, int sitio, int **viz, int topologia[N])
{
	int i=0;
	
	nd=0;
	nc=0;
	np=0;
	ndc=0;
	npc=0;

	#ifdef viz_espacial
	for(i=0;i<topologia[sitio2];i++) // vizinho 0 e o proprio sitio
	{
		switch(state[viz[sitio2][i]])
		{
			case DEFECTOR:   ++nd; break;
			case COOPERATOR: ++nc; break;
			case PUNISHER:   ++np; break;
			case 3: ++npc; break;
			case 4: ++ndc; break;
			default: 
				fprintf(stderr,"ERRO calculo vizinhos\n");
				fflush(stderr);
		}
	}
	#endif

	#ifdef viz_well_mixed
	for(i=2;i<topologia[sitio2];i++) //i=1 se nao conta o invadido
	{
		int k;
		k = (int) N*gsl_rng_uniform(rand_vec);
		
		if(k != sitio2)
		{
			if(state[k]==0) 
			{
			 	nd = nd+1; 		
		  	}
			if(state[k]==2) 
			{
			 	np = np+1; 	
		  	}
			if(state[k]==1) 
			{
			 	nc = nc+1; 	
		  	}
		}
	}

	//conta o proprio sitio
	switch(state[viz[sitio][0]])
	{
		case DEFECTOR:   ++nd; break;
		case COOPERATOR: ++nc; break;
		case PUNISHER:   ++np; break;
		default:
			fprintf(stderr,"ERRO contagem estrategia proprio sitio\n");
			fflush(stderr);	
	}
	//conta o invasdido sitio
	int aaa = abs(state[sitio] - 1);
	if(aaa==0) 
	{
	 	nd = nd+1; 		
  	}
	if(aaa==1) 
	{
	 	nc = nc+1; 	
  	}
  	
	#endif
	//printf("nc=%d nd=%d\n",state[sitio],aaa);
	return;
}

/********************************************************************
***                          Payoff                               ***
********************************************************************/

void calculo_payoff ( double *payoff, double r, double gama, double delta, int x, int topologia[N], int state[N], double *investimento, int **viz )
{

	/*//puniçao c/ propina
	double q = gsl_rng_uniform(rand_vec);
	double p = gsl_rng_uniform(rand_vec);

	payoff[COOPERATOR] = (1./G)*r*c* (np+nc) - c; //C
	payoff[PUNISHER]   = (1./G)*r*c* (np+nc) - c + p*q*delta*nd - (1 - p*q)*gama*nd; //P
	payoff[DEFECTOR]   = (1./G)*r*c* (np+nc) - (1 - p*q)*gama*np - p*q*delta*np; //D*/


	/*//Ising
	payoff[COOPERATOR] = 2*1*(nc-nd + 0.0); 
	payoff[DEFECTOR]   = 2*(-1)*(nc-nd + 0.0); //W = exp(-payoff/T)*/
	
	//puniçao 
	#if defined(PGG) || defined(PGG_FOCAL)

	double pool = (r/topologia[x])*investimento_total;

	payoff[COOPERATOR] = pool;
	payoff[PUNISHER]   = pool - gama*nd;
	payoff[DEFECTOR]   = pool - delta*np;

	/*//corrupçao com propina
	double pool = (r/topologia[x])*(nc+np);

	payoff[COOPERATOR] = pool - (r/topologia[x])*gama*np + delta*gama*np/nc;
	payoff[DEFECTOR]   = pool - (r/topologia[x])*gama*np;
	
	if(nc==0)
	{
	payoff[PUNISHER]   = pool - (r/topologia[x])*gama*np + (1)*gama;
	}
	else
	{
	payoff[PUNISHER]   = pool - (r/topologia[x])*gama*np + (1-delta)*gama;
	}*/
	
	

	#endif
	
	
	#ifdef PRISONERS_DILEMMA
	double b=3.6;
	double g=0.1;	
	switch(state[x])
	{		

		case COOPERATOR:
			//payoff[COOPERATOR] = nc;   // <- com autointeracao   //RR*(nc - 1) + SS*nd; // sem autointeracao
			payoff[COOPERATOR] = (b-g)*(nc-1) - g*nd;
			break;
		case DEFECTOR: 
			//payoff[DEFECTOR]   = TT*nc;// <- com autointeracao  //TT*nc + PP*(nd-1);   // sem autointeracao
			payoff[DEFECTOR]   = b*nc ;
			break;
	}
	#endif
	
	/*//recompensa
	payoff[COOPERATOR] = (1./G)*r*c* (np+nc) - c + delta*np;
	payoff[PUNISHER]   = (1./G)*r*c* (np+nc) - c - gama*nc;
	payoff[DEFECTOR]   = (1./G)*r*c* (np+nc);*/



	return;
}

/********************************************************************
***                         Update rule                           ***
********************************************************************/
void update_rule( int x, int vizinho, int state[N], double Px, double Py ,int topologia[N], double *investimento)
{
#ifdef NORMALIZED_PAYOFFS
	double Wxy = 1.0/(1.0 + exp(-(Py-Px)*INV_G_RUIDO)); // INV_G_RUIDO = 1.0/G*RUIDO
#else
	double Wxy = 1.0/(1.0 + exp(-(Py-Px)*INV_RUIDO));//Wxy = 1.0/(1.0 + exp(-(Py-Px)/RUIDO));
#endif
	//double Wxy = exp(-Px/r); //Ising

	double l = gsl_rng_uniform(rand_vec);

	//printf("Px=%lf Py=%lf delta=%lf W=%lf \n",Px,Py,Py-Px,Wxy);

	//if((state[n]+state[vizinho])==3){Wxy=0.;}
	//if(state[n]==4 && state[vizinho]==3){Wxy=0.;}
	//troca de estado
	if(Wxy > l)
	//if(Py > Px)
	{      
		state[x] = state[vizinho];
		topologia[x] = topologia[vizinho];	
		investimento[x] = investimento[vizinho]; // tirar pra fixa investimento com local da rede
		//state[x] = fabs(state[x]-1); //Ising
	}
}


/********************************************************************
***                         MCS                              ***
********************************************************************/
void calculo_mcs(double *payoff, int state[N], int **viz, double r, double gama, double delta, int topologia[N], double *investimento)
{

	int n;
	int x,y,sitio,vizinho;
	double Px, Py;
	int sitio2, vizinho2;	

	for(n=0; n<N; n++)
	{					
		Px = 0;
		Py = 0;

		x = (int) N*gsl_rng_uniform(rand_vec);
   		sitio=x;

		#ifdef viz_espacial
		y = 1 + (int) (topologia[x]-1)*gsl_rng_uniform(rand_vec); 
		vizinho = viz[x][y];
		#endif

		#ifdef viz_well_mixed
		y = (int) N*gsl_rng_uniform(rand_vec);
		vizinho = y;
		#endif
		//sitio = 4; vizinho = 5;		
		if (state[vizinho] != state[x])
		{
		#if defined(PGG_FOCAL)

			#ifdef viz_espacial
			sitio2 = sitio;
			vizinho2 = vizinho;			
			#endif		
			#ifdef viz_well_mixed
			sitio2 = viz[sitio][G-1];
			vizinho2 = viz[vizinho][G-1];
			#endif

			calculo_numero(state,sitio2,sitio,viz,topologia);
			calculo_investimento(state,sitio2,sitio,vizinho,viz,topologia,investimento);
			calculo_payoff(payoff,r,gama,delta,sitio,topologia,state,investimento,viz);
			Px = payoff[state[sitio]] - state[sitio]*investimento[sitio];

			calculo_numero(state,vizinho2,vizinho,viz,topologia);
			calculo_investimento(state,vizinho2,vizinho,sitio,viz,topologia,investimento);
			calculo_payoff(payoff,r,gama,delta,vizinho,topologia,state,investimento,viz);
			Py = payoff[state[vizinho]] - state[vizinho]*investimento[vizinho];

		#endif	

		#ifdef PGG

			for(y=0;y<topologia[sitio];y++)
			{
				sitio2 = viz[sitio][y];
				calculo_numero(state,sitio2,sitio,viz,topologia);
				calculo_investimento(state,sitio2,sitio,vizinho,viz,topologia,investimento);
				calculo_payoff(payoff,r,gama,delta,sitio2,topologia,state,investimento,viz);
				Px += payoff[state[sitio]] - state[sitio]*investimento[sitio];
				//printf("%d %d %d %d %lf \n", sitio, state[sitio],sitio2, state[sitio2],Px);
			}

			for(y=0;y<topologia[vizinho];y++)
			{			
				vizinho2 = viz[vizinho][y];
				calculo_numero(state,vizinho2,vizinho,viz,topologia);
				calculo_investimento(state,vizinho2,vizinho,sitio,viz,topologia,investimento);
				calculo_payoff(payoff,r,gama,delta,vizinho2,topologia,state,investimento,viz);
				Py += payoff[state[vizinho]] - state[vizinho]*investimento[vizinho];
				//printf("%d %d %d %d %lf \n", vizinho, state[vizinho],vizinho2, state[vizinho2],Py);

			}
			
		#endif
		
		#if defined(PRISONERS_DILEMMA)

			#ifdef viz_espacial
			sitio2 = sitio;
			vizinho2 = vizinho;			
			#endif		
			#ifdef viz_well_mixed
			sitio2 = viz[sitio][G-1];
			vizinho2 = viz[vizinho][G-1];
			#endif

			calculo_numero(state,sitio2,sitio,viz,topologia);
			calculo_investimento(state,sitio2,sitio,vizinho,viz,topologia,investimento);
			calculo_payoff(payoff,r,gama,delta,sitio,topologia,state,investimento,viz);
			Px = payoff[state[sitio]];

			calculo_numero(state,vizinho2,vizinho,viz,topologia);
			calculo_investimento(state,vizinho2,vizinho,sitio,viz,topologia,investimento);
			calculo_payoff(payoff,r,gama,delta,vizinho,topologia,state,investimento,viz);
			Py = payoff[state[vizinho]];

		#endif
		
			update_rule(x,vizinho,state,Px,Py,topologia,investimento);
		}//if (state[vizinho] != state[x])
  	}
}
/********************************************************************
***                          Mobilidade                           ***
********************************************************************/				
void calculo_mobilidade( int state[N], int **viz)
{
	
	double g = gsl_rng_uniform(rand_vec);
	if(g<=prob_mobil)
	{		
		int y = 1 + (int) 4*gsl_rng_uniform(rand_vec);
		int x = (int) N*gsl_rng_uniform(rand_vec);
		int e = state[x];
		state[x] = state[viz[x][y]];
		state[viz[x][y]] = e;
	}
}

/********************************************************************
***                          Densidades                           ***
********************************************************************/
void calculo_densidades(int state[N], int t, FILE *fp)
{
	int k;

	ND=0;
	NC=0;
	NP=0;
	NPC=0;
	NDC=0;
	
	for(k=0;k<N;k++)
	{
		switch (state[k])
		{
			case COOPERATOR: ++NC; break;
			case PUNISHER:   ++NP; break;
			case DEFECTOR:   ++ND; break;
			case 3: ++NPC; break;
			case 4: ++NDC; break;
			default: 
				fprintf(stderr,"ERRO - tipo de estrategia\n");
				fflush(stderr);
		}	
	}	

	#ifdef densidade_terminal
	printf("%d %lf %lf %lf %lf %lf\n", t, (double)NC/(N), (double)ND/(N), (double)NP/(N),(double)NPC/(N),(double)NDC/(N));
	#endif

	#ifdef densidade_arquivo
	fprintf(fp,"%d %lf %lf %lf %lf %lf\n", t, (double)NC/(N), (double)ND/(N), (double)NP/(N),(double)NPC/(N),(double)NDC/(N));	
	#endif

}

/********************************************************************
***                        Snapshots                              ***
********************************************************************/
void snap_gnuplot(int state[N], int topologia[N], double *investimento, int t)	//    ./a.out | gnuplot
{

	int i,j;
	
	//printf("set title \"MCS = %d\" \n",t);
	printf("set autoscale keepfix\n");
	printf("set palette model RGB\n");
	//printf("unset colorbox\n");
	printf("unset border\n");
	printf("unset xtics\n");
	printf("unset ytics\n");
	printf("set palette defined ( 0 \"red\", 1 \"blue\",  2 \"yellow\",  3 \"dark-green\", 4 \"dark-red\", 5 \"green\", 7 \"blue\",  9 \"red\")\n");
	printf("set cbrange[0:8]\n");
	printf("set xrange[0:%d]\n",L);
	printf("set yrange[0:%d]\n",L);
	printf("set size square\n");
			
	printf("plot \"-\" matrix with image\n");
			

	for(i=0;i<L;i++)
	{
		for(j=0;j<L;j++)
		{
			printf("%d ",state[j+i*L]);
		
		}
		/*for(j=0;j<L;j++)
		{
			printf("%lf ",investimento[j+i*L]);
		}*/
		printf("\n");
	}
	printf("\n"); 
	printf("e\n");    printf("pause(%lf)\n",fps);
	
}
void snap_gif(int state[N], int topologia[N], int t)
{

	int i,j;
	
	for(i=0;i<L;i++)
	{
		for(j=0;j<L;j++)
		{
			printf("%d ",state[j+i*L]);
		
		}
		/*for(j=0;j<L;j++)
		{
		printf("%d ",topologia[j+i*L]);
		}*/
		printf("\n");
	}
	printf("\n"); 

}
void snap_kagome(int state[N], int topologia[N], int t)
{

	int n,i,j; //n = i + 2j N
	double x,y;

	printf("set title \"MCS = %d\" \n",t);
	printf("set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back\n");
	printf("set object 1 rect fc rgb \"black\" fillstyle solid 1.0 \n");
	printf("set view map\n");
	printf("set size ratio .9\n");
	//printf("set xrange[30:50]\n");
	//printf("set yrange[30:60]\n");
	printf("set xrange[00:100]\n");
	printf("set yrange[0:40]\n");
	printf("set palette defined ( 0 \"red\", 1 \"blue\",  2 \"green\")\n");
	printf("set cbrange[0:2]\n");
	printf("unset key\n");
	printf("splot \"-\" using 1:2:3 with points pointtype 7 pointsize 1. palette linewidth 10\n");

	for(n=0;n<N;n++)
	{
	//int L = 100;
	int d = 1;
	double h = d*pow(3/4,1/2);
	int n_celula = (int) (n/3);
	int linha_celula = (int) (n_celula/L);
	int coluna_celula = (n_celula%(L));	
		if(n % 3 == 0)
		{
			x = 2*d*coluna_celula + linha_celula*d;
			y = 0 + 2*h*linha_celula;
		}
		if(n % 3 == 1)
		{
			x = d + 2*d*coluna_celula  + linha_celula*d;
			y = 0 + 2*h*linha_celula;
		}
		if(n % 3 == 2)
		{
			x = (d/2.) + 2*d*coluna_celula  + linha_celula*d;
			y = h + 2*h*linha_celula;
		}
		printf("%lf %lf %d\n",x,y,state[n]);
		//printf("%lf %lf %d %lf %lf\n",x,y,a,h,2*h* (int) (n_celula/L));
	}
	printf("\n"); 
	printf("e\n");    printf("pause(%lf)\n",fps);
}
void snap_hexa(int state[N], int topologia[N], int t)
{

	int n,i,j; //n = i + 2j N
	double x,y,d=1,h=70.;

	printf("set title \"MCS = %d\" \n",t);
	printf("set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back\n");
	printf("set object 1 rect fc rgb \"black\" fillstyle solid 1.0 \n");
	printf("set view map\n");
	printf("set size ratio .9\n");
	//printf("set xrange[00:100]\n");
	//printf("set yrange[0:2000]\n");
	printf("set palette defined ( 0 \"red\", 1 \"blue\",  2 \"green\")\n");
	printf("set cbrange[0:2]\n");
	printf("unset key\n");
	printf("splot \"-\" using 1:2:3 with points pointtype 7 pointsize 1. palette linewidth 10\n");

	for(n=0;n<N;n++)
	{
		i = n % (2*L);
		j = n / (2*L);

		x = d*(i+j);
		y = j*(L+h) - h*(i%2);

		printf("%lf %lf %d\n",x,y,state[n]);

	}
	printf("\n"); 
	printf("e\n");    printf("pause(%lf)\n",fps);
}

/********************************************************************
***                     Initialize Network                        ***
********************************************************************/
void initialize_network(char REDE_ATUAL_NAME[], char filename[], double r, double gama, double delta, unsigned long rseed)
{	    
       if (!strcmp(REDE_ATUAL_NAME, "unidimensional") || !strcmp(REDE_ATUAL_NAME, "UNIDIMENSIONAL"))
	{
		N = L; G = 3;
		REDE_ATUAL = UNIDIMENSIONAL;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"unidimensional_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	} 
    	if (!strcmp(REDE_ATUAL_NAME, "quadrada") || !strcmp(REDE_ATUAL_NAME, "QUADRADA"))
	{
		N = L2; G = 5;
		REDE_ATUAL = QUADRADA;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"quadrada_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}
	if (!strcmp(REDE_ATUAL_NAME, "cubica") || !strcmp(REDE_ATUAL_NAME, "CUBICA"))
	{
		N = L3; G = 7;
		REDE_ATUAL = CUBICA;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"cubica_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	} 
	if (!strcmp(REDE_ATUAL_NAME, "quadridimensional") || !strcmp(REDE_ATUAL_NAME, "QUADRIDIMENSIONAL"))
	{
		N = L4;	G = 9;
		REDE_ATUAL=QUADRIDIMENSIONAL;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"quadridim_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}
	if (!strcmp(REDE_ATUAL_NAME, "hexagonal") || !strcmp(REDE_ATUAL_NAME, "HEXAGONAL"))
	{
		N = 2*L2; G = 4;
		REDE_ATUAL=HEXAGONAL;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"hexagonal_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	} 
	if (!strcmp(REDE_ATUAL_NAME, "kagome")  || !strcmp(REDE_ATUAL_NAME, "KAGOME"))
	{
		N = 3*L2; G = 5;
		REDE_ATUAL=KAGOME;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"kagome_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}
	if (!strcmp(REDE_ATUAL_NAME, "triangular")  || !strcmp(REDE_ATUAL_NAME, "TRIANGULAR"))
	{
		N = L2; G = 7;
		REDE_ATUAL=TRIANGULAR;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"triangular_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}	
	if (!strcmp(REDE_ATUAL_NAME, "moore")  || !strcmp(REDE_ATUAL_NAME, "MOORE"))
	{
		N = L2; G = 9;
		REDE_ATUAL=MOORE;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"moore_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}
	/*if (!strcmp(REDE_ATUAL_NAME, "random")  || !strcmp(REDE_ATUAL_NAME, "RANDOM"))
	{
		N = L2; G = 5;
		REDE_ATUAL=RANDOM;
		//fprintf(stderr,"%s escolhida!\n",REDE_ATUAL_NAME); fflush(stderr);
		sprintf(filename,"random_dados_r%f_g%f_d%f_seed%ld.txt",r,gama,delta,rseed);
	}*/	
	
#ifdef NORMALIZED_PAYOFFS
	INV_G_RUIDO = 1.0/(G*RUIDO);
#endif	
	INV_RUIDO = 1.0/RUIDO;
	//fprintf(stderr,"%s escolhida!\n G=%d\nN=%lld\n",REDE_ATUAL_NAME,G,N); fflush(stderr);
	return;
}
/********************************************************************
***                          Main                                 ***
********************************************************************/
int main(int argc, char *argv[])
{	
	//gera arquivo
	char filename[200];
	char REDE_ATUAL_NAME[50];	
	FILE *fp;		
  	int t=0;	
	double payoff[5], gama, delta,r;//,Px,Py;
 		
	if (argc!=7)
	{
		printf("O programa precisa de 6 argumentos:\n1) r\n2) gama\n3) delta\n4) tipo de rede\n5) L\n6) RUIDO");
		exit(1);
	}
	r=atof(argv[1]);
	gama=atof(argv[2]);
	delta=atof(argv[3]);
	sprintf(REDE_ATUAL_NAME,"%s",argv[4]);
	L=atoi(argv[5]);
	RUIDO=atof(argv[6]); 
    
   	L2 = L*L; 
	L3 = L2*L;
	L4 = L3*L; 
    
#ifdef PRISONERS_DILEMMA
	//TT = r; RR = 1.0; PP = 0.0; SS=0.0;
	TT = 2.0-r; RR = 1.0; PP = 0.0; SS=-1.0+r; // 1 parametro apenas
#endif   
	set_gsl_rng();    
	initialize_network(REDE_ATUAL_NAME,filename,r,gama,delta,rseed);  
	int state[N],**viz,topologia[N];
	double investimento[N];
	viz = create_2d_int_pointer_h(N,G);	

	fp=fopen(filename,"w");
	seed = start_randomic(); //mc.h
//------------------------------------------------------------------------------------
	//fprintf(stderr,"REDE_ATUAL=%d\n",REDE_ATUAL);
	switch (REDE_ATUAL)
	{
		case UNIDIMENSIONAL: rede_unidimensional(viz,L);
			break;
		case QUADRADA: rede_quadrada(viz,L);
			break;
		case CUBICA: rede_cubica(viz,L);
			break;
		case QUADRIDIMENSIONAL: rede_quadridimensional(viz,L); 
			break;
		case HEXAGONAL: rede_hexagonal(viz,L);
			break;
		case KAGOME: rede_kagome(viz,L);
			break;
		case TRIANGULAR: rede_triangular(viz,L);
			break;
		case MOORE: rede_moore(viz,L);
			break;
		/*case RANDOM: rede_random(viz,L);
			break;*/
		default:
			fprintf(stderr,"ERRO inicializacao rede!\n");
			fflush(stderr);
	} 
	//fprintf(stderr,"Antes calculo_ci\n"); fflush(stderr);
	calculo_ci(state,viz);
	//fprintf(stderr,"Antes calculo_densidades\n"); fflush(stderr);	
	calculo_densidades(state,t,fp);
	calculo_topologia(topologia);
	calculo_ci_investimento(investimento,state);

	#ifdef snapshot_gnuplot	
	snap_gnuplot(state, topologia,investimento,t);
	#endif 
	#ifdef snaphot_gif	
	snap_gif(state, topologia,t);
	#endif 
	#ifdef snapshot_hexa	
	snap_hexa(state, topologia,t);
	#endif 
	#ifdef snapshot_kagome
	snap_kagome(state, topologia,t);
	#endif 
	
	//MCS		
	for(t=1; t < tmax; t++)
	{			
		calculo_mcs(payoff,state,viz,r,gama,delta,topologia,investimento);
		//if(t==25 || t==100 || t==1000){snapshot(state,topologia,t);}
		//calculo_mobilidade(state,viz);
	
		#ifdef snapshot_gnuplot	
		snap_gnuplot(state, topologia,investimento,t);
		#endif 
		#ifdef snaphot_gif	
		o_gif(state, topologia,t);
		#endif 
		#ifdef snapshot_hexa	
		snap_hexa(state, topologia,t);
		#endif 
		#ifdef snapshot_kagome
		snap_kagome(state, topologia,t);
		#endif 
//------------------------------------------------------------------------------------

		if (t >= measure_time)
		{
			//fprintf(stderr,"t = %d\n",t); fflush(stderr);
			calculo_densidades(state,t,fp);
			measure_time *= 1.1;
			if (floor(measure_time)-t < EPSILON) 
				measure_time=ceil(measure_time);
		}    
  		if((NC/(N)==1) || (NP/(N)==1) || (NPC/(N)==1) || (ND/(N)==1) || ((NC+NP)/(N)==1))
		{
			do
			{
				if ( t >= measure_time)
				{
					fprintf(fp,"%d %lf %lf %lf %lf %lf\n", t, (double)NC/(N), (double)ND/(N), (double)NP/(N),(double)NPC/(N),(double)NDC/(N));
					measure_time *= 1.1;
					if (floor(measure_time)-t < EPSILON) 
					measure_time=ceil(measure_time);
				}
				t=t+1;
			}
			while(t<tmax);
		 break;
		}
	
	}//MCS

fclose(fp);

return 0;
free_2d_int_pointer(viz,N,G);

} //main
