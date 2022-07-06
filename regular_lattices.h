/********************************************************************
***                          Lattices                             ***
*********************************************************************
***                                                		  ***
***    define in main.c:                         		  ***
***                                				  ***                                      		
***		int G;  //group size				  ***
***		int N;	//population size			  ***
***		int L;	//linear population size		  ***
***		int **viz; // or viz[N][G]			  ***
***		viz = create_2d_int_pointer_h(N,G);		  ***
***		lattice_name(viz,L);  				  ***
***		free_2d_int_pointer(viz,N,G)  //end of main 	  ***
***                                                 		  ***
********************************************************************/
;

/********************************************************************
***                      1D lattice                               ***
********************************************************************/
void unidimensional_lattice (int **viz, int L)
{
	int x;
	int N  = L;

	for(x=0;x<N;x++)
	{	
		viz[x][0] = x; // self-interaction
		viz[x][1] = x - 1; //left neighbor
		viz[x][2] = x + 1; //right neighbor
				
		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/
		if(x == 0)	{ viz[x][1] = L - 1;} //left boundary
		if(x == L - 1)	{ viz[x][2] = 0;} //right boundary
	}
	
	return;		
}

/********************************************************************
***                     Square von Neumann                        ***
********************************************************************/
void square_lattice (int **viz, int L)
{
	int x;
	long int L2 = L*L;
	long int N  = L2;
	//G=5;
	for(x=0;x<N;x++)
	{	
		viz[x][0] = x; //self-interaction
		viz[x][1] = x - L; //top neighbor
		viz[x][2] = x + 1; //right neighbor
		viz[x][3] = x + L; //botton neighbor
		viz[x][4] = x - 1; //left neighbor
				
		/****************************************************
		***                     P.B.C.                   ***
		****************************************************/
		if (x < L)		{ viz[x][1] = x + (L - 1) * L; } //top boundary
		if (x % L == 0)		{ viz[x][4] = x + (L - 1); } 	//left boundary
		if (x >= L2-L)		{ viz[x][3] = x - (L - 1) * L; } //botton boundary
		if ((x-L+1) % L == 0)	{ viz[x][2] = x - (L - 1); } //right boundary

		//printf("%d %d %d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4],viz[x][5],viz[x][6]);
	}
	
	return;		
}

/********************************************************************
***                      cubic lattice                            ***
********************************************************************/
void cubic_lattice (int **viz, int L)
{
	int x;
	long int L2 = L*L;
	long int L3 = L2*L;
	long int N  = L3;
	//G=7;
	
	for(x=0;x<N;x++)
	{	
		viz[x][0] = x; //self-interaction
		viz[x][1] = x - L; //top neighbor
		viz[x][2] = x + 1; //right neighbor
		viz[x][3] = x + L; //botton neighbor
		viz[x][4] = x - 1; //left neighbor
				
		viz[x][5] = x + L2; //back neighbor
		viz[x][6] = x - L2; //front neighbor

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/
		if (x % (L2) < L) 	{ viz[x][1] = x + (L - 1) * L; } //top boundary
		if (x % L == 0) 	{ viz[x][4] = x + (L - 1); } //left boundary
		if (x % (L2) >= L2-L)	{ viz[x][3] = x - (L - 1) * L; } //botton boundary
		if ((x-L+1) % L == 0)	{ viz[x][2] = x - (L - 1); } //right boundary
		if (x < L2)		{ viz[x][6] = x + L3 - L2; } //front boundary
		if (x >= L3 - L2)	{ viz[x][5] = x - L3 + L2; } //back boundary
	

		//printf("%d %d %d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4],viz[x][5],viz[x][6]);
	}
	return;		
}

/********************************************************************
***                   	   4D hypercubic    		            ***
********************************************************************/
void hypercubic_lattice (int **viz, int L)
{
	int x;
	long int L2 = L*L;
	long int L3 = L2*L;
	long long int L4 = L3*L;
	long long int N = L4;
	//G=9;

	for(x=0;x<N;x++)
	{	
		viz[x][0] = x; // self-interaction
		viz[x][1] = x - L; //top neighbor
		viz[x][2] = x + 1; //right neighbor
		viz[x][3] = x + L; //botton neibhor
		viz[x][4] = x - 1; //left neighbor
				
		viz[x][5] = x + L2; //back neighbor
		viz[x][6] = x - L2; //front neighbor

		viz[x][7] = x + L3; //front front neighbor
		viz[x][8] = x - L3; //back back neighbor

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/
		if (x % (L2) < L)		{ viz[x][1] = x + (L - 1) * L; } //top boundary
		if (x % L == 0)			{ viz[x][4] = x + (L - 1); } //left boundary
		if (x % (L2) >= L2-L)		{ viz[x][3] = x - (L - 1) * L; } //botton boundary
		if ((x-L+1) % L == 0)		{ viz[x][2] = x - (L - 1); } //right boundary
		if (x % (L3) < L2)		{ viz[x][6] = x + L3 - L2; } //front boundary
		if (x % (L3) >= L3 - L2)	{ viz[x][5] = x - L3 + L2; } //back boundary
		if (x < L3)			{ viz[x][8] = x + L4 - L3; } //front front boundary
		if (x >= L4 - L3)		{ viz[x][7] = x % (L3); } //back back boundary


		//printf("%d %d %d %d %d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4],viz[x][5],viz[x][6],viz[x][7],viz[x][8]);
	}
	return;		
}

/********************************************************************
***                     kagome lattice                            ***
********************************************************************/
void kagome_lattice (int **viz, int L)
{
	int x;
	long L2 = L*L;
	long N  = 3*L2;
	//G=5;
	
   	for(x=0; x < N; x++)
	{
		if (x % 3 == 0) 
		{
			viz[x][0] = x; // self-interaction
			viz[x][1] = x + 1; //right neighbor
			viz[x][2] = x - 2; //left neighbor
			viz[x][3] = x + 2; //right neighbor
			viz[x][4] = x - 3*L + 2; //botton neighbor
		}	
		if (x % 3 == 1) 
		{
			viz[x][0] = x; // self-interaction
			viz[x][1] = x - 1; //left neighbor
			viz[x][2] = x + 1; //right neighbor
			viz[x][3] = x + 2; //right neighbor
			viz[x][4] = x - 3*L + 4; //botton neighbor				
		}
		if (x % 3 == 2) 
		{
			viz[x][0] = x; // self-interaction
			viz[x][1] = x - 1; //left neighbor
			viz[x][2] = x - 2; //right neighbor
			viz[x][3] = x + 3*L - 2; //right neighbor
			viz[x][4] = x + 3*L - 4; //botton neighbor			
		}

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/
		if((x+2) % (3*L) == 0 && x % 3 == 1) //right boundary
		{
			viz[x][3] = x - 3*L + 2;
			viz[x][4] = x - 6*L + 4;
		} 
		if((x) % (3*L) == 0 && x % 3 == 0) //left boundary
		{
			viz[x][2] = x + 3*L - 2; 
		}
		if((x+3*L-2) % (3*L) == 0 && x % 3 == 2) //left boundary
		{ 
			viz[x][4] = x + (3*L - 2) * 2; 
		} 
		if(x > 3*L2 - 3*L && x % 3 == 2) //top boudnary		
		{
			viz[x][3] = x - 3*L2 + 3*L - 4; 
			viz[x][4] = x - 3*L2 + 3*L - 2; 
		}			
		if(x < 3*L) //botton boundary
		{			
			if(x % 3 == 0) {viz[x][4] = x + 3*L2 - 3*L + 2; } 
			if(x % 3 == 1) {viz[x][4] = x + 3*L2 - 3*L + 4; } 
		}
		if(x == 3*L2 - 3*L + 2) //vertice
		{
			viz[x][4] = 3*L - 2;
		 	viz[x][3] = 0;  
		} 
		if(x == 3*L - 2) //vertice
		{
			viz[x][4] = 3*L2 - 3*L + 2; 
		} 

		//printf("%d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4]);
	}
	return;		
}

/********************************************************************
***                     honeycomb lattice                         ***
********************************************************************/
void honeycomb_lattice (int **viz, int L)
{
	int x;
	long L2 = L*L;
	long N  = 2*L2;
	//G=4;
	
   	for(x=0; x < N; x++)
	{
		if (x % 2 == 0) //even site
		{
			viz[x][0] = x; // self-interaction
			viz[x][1] = x - 1; //left neighbor
			viz[x][2] = x + 1; //right neighbor
			viz[x][3] = x + 2 * L - 1; //botton neighbor
		}	
		else //if (x % 2 != 0) //odd site
		{
			viz[x][0] = x; // self-interaction
			viz[x][1] = x - 1; //left neighbor
			viz[x][2] = x + 1; //right neighbor
			viz[x][3] = x - 2 * L + 1; //top neighbor				
		}

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/	
		if((x-2*L+1) % (2*L) == 0) //right boundary
		{
			viz[x][2] = x - 2 * L + 1;
		 	viz[x][3] = x - 4 * L + 1; 	
		} 
		if(x % (2*L) == 0 || x == 0) //left boundary
		{
			viz[x][1] = x + 2 * L - 1; 
		 	viz[x][3] = x + 4 * L - 1; 
		} 
		if(x < 2*L && x % 2 != 0) //top boundary		
		{
			viz[x][3] = x + 2 * L * (L-1) + 1; 
		} 
		if(x > 2*L2 - 2*L - 1 && x % 2 == 0) //botton boundary
		{
			viz[x][3] = x - 2 * L * (L-1) - 1; 
		} 
		if(x == 2*L2 - 2*L) //left-botton vertice 
		{
			viz[x][3] = 2*L - 1; 
		} 
		if(x == 2*L - 1) //top-right vertice
		{
			viz[x][3] = 2*L2 - 2*L; 
		} 

		//printf("%d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3]);
	}
	return;
	
}

/********************************************************************
***                   triangular lattice                          ***
********************************************************************/
void triangular_lattice (int **viz, int L)
{
	int x;
	long int L2 = L*L;
	long int N  = L2;
	//G=7;
	
	for(x=0;x<N;x++)
	{	

		viz[x][0] = x; // self-interaction
		viz[x][1] = x - L; //top neighbor
		viz[x][2] = x + 1; //right neighbor
		viz[x][3] = x + L; //botton neighbor
		viz[x][4] = x - 1; //left neighbor
		viz[x][5] = x - L + 1; //top-right neighbor
		viz[x][6] = x - 1 + L; //botton-left neighbor

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/	
		if(x % (L2) < L) //top boundary
		{
			viz[x][1] = x + (L - 1) * L; 
			viz[x][5] = x + (L - 1) * L + 1;
		}
		if(x % L == 0) //left boundary
		{
			viz[x][4] = x + (L - 1); 
			viz[x][6] = x + (L - 1) + L; 
		}
		if(x % (L2) >= L2-L) //botton boundary
		{
			viz[x][3] = x - (L - 1) * L; 
			viz[x][6] = x - (L - 1) * L - 1; 
		}
		if((x-L+1) % L == 0) //right boundary
		{
			viz[x][2] = x - (L - 1);
			viz[x][5] = x - (L - 1) - L;
		}
		if(x == L-1) //top-right vertice
		{
			viz[x][5] = L * L - L ; 
		}
		if(x == L2-L) //botton-left vertice
		{
			viz[x][6] = L - 1;
		}

		//printf("%d %d %d %d %d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4],viz[x][5],viz[x][6],viz[x][7],viz[x][8]);
	}
	return;			
}

/********************************************************************
***                        square Moore                           ***
********************************************************************/
void moore_lattice (int **viz, int L)
{
	int x;
	long int L2 = L*L;
	long int N  = L2;
	//G=9;
	
	for(x=0;x<N;x++)
	{	

		viz[x][0] = x; // self-interaction
		viz[x][1] = x - L; //top neighbor
		viz[x][2] = x + 1; //right neighbor
		viz[x][3] = x + L; //botton neighbor
		viz[x][4] = x - 1; //left neighbor
		viz[x][5] = x - L + 1; //top-right neighbor
		viz[x][6] = x - 1 + L; //botton-left neighbor
		viz[x][7] = x - 1 - L; //top-left neighbor
		viz[x][8] = x + 1 + L; //botton-right neighbor

		/****************************************************
		***                     P.B.C.                    ***
		****************************************************/	
		if (x % (L2) < L) //top boundary
		{
			viz[x][1] = x + (L - 1) * L; 
			viz[x][5] = x + (L - 1) * L + 1;
			viz[x][7] = x + (L - 1) * L - 1;
		}
		if (x % L == 0) //left boundary
		{
			viz[x][4] = x + (L - 1); 
			viz[x][6] = x + (L - 1) + L; 
			viz[x][7] = x + (L - 1) - L; 
		}
		if (x % (L2) >= L2-L) //botton boundary
		{
			viz[x][3] = x - (L - 1) * L; 
			viz[x][6] = x - (L - 1) * L - 1; 
			viz[x][8] = x - (L - 1) * L + 1; 
		}
		if ((x-L+1) % L == 0) //right boundary
		{
			viz[x][2] = x - (L - 1);
			viz[x][5] = x - (L - 1) - L;
			viz[x][8] = x - (L - 1) + L;
		}
		if (x == 0) //top-left vertice
		{
			viz[x][7] = L * L - 1;
		}
		if (x == L-1) //top-right vertice
		{
			viz[x][5] = L * L - L ; 
		}
		if (x == L2-L) //botton-left vertice
		{
			viz[x][6] = L - 1;
		}
		if (x == L2-1) //botton-right vertice
		{
			viz[x][8] = 0;
		}

		//printf("%d %d %d %d %d %d %d %d %d\n",viz[x][0],viz[x][1],viz[x][2],viz[x][3],viz[x][4],viz[x][5],viz[x][6],viz[x][7],viz[x][8]);
	}
	return;			
}
