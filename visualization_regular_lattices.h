/********************************************************************
***                        Snapshots                              ***
********************************************************************/
void snap_gnuplot(int state[N], int topologia[N], int t)	//    ./a.out | gnuplot
{

	int i,j;
	
	//printf("set title \"MCS = %d\" \n",t);
	printf("set autoscale keepfix\n");
	printf("set palette model RGB\n");
	printf("unset colorbox\n");
	printf("unset border\n");
	printf("unset xtics\n");
	printf("unset ytics\n");
	printf("set palette defined ( 0 \"red\", 1 \"blue\",  2 \"green\",  3 \"dark-green\", 4 \"dark-red\", 5 \"green\", 7 \"blue\",  9 \"red\")\n");
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
			printf("%d ",topologia[j+i*L]);
		}*/
		printf("\n");
	}
	printf("\n"); 
	printf("e\n");    printf("pause(%lf)\n",fps);
	
}
void snap_gif(int state[N], int topologia[N], int t) // use with gif.sh
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
void snap_kagome(int state[N], int topologia[N], int t)   //    ./a.out | gnuplot
{

	int n,i,j; //n = i + 2j N
	double x,y;

	printf("set title \"MCS = %d\" \n",t);
	printf("set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back\n");
	printf("set object 1 rect fc rgb \"black\" fillstyle solid 1.0 \n");
	printf("set view map\n");
	printf("set size ratio .9\n");
	printf("set xrange[0:70]\n");
	//printf("set yrange[000:50]\n");
	printf("set palette defined ( 0 \"red\", 1 \"blue\",  2 \"green\")\n");
	printf("set cbrange[0:2]\n");
	printf("unset key\n");
	printf("splot \"-\" using 1:2:3 with points pointtype 7 pointsize 1. palette linewidth 10\n");

	for(n=0;n<N;n++)
	{
	//int L = 100;
	int d = 1;
	double h = d*pow(3/4,1/2);
	int n_cell = (int) (n/3);
	int cell_row = (int) (n_cell/L);
	int cell_column = (n_cell%(L));	
		if(n % 3 == 0)
		{
			x = 2*d*cell_column + cell_row*d;
			y = 0 + 2*h*cell_row;
		}
		if(n % 3 == 1)
		{
			x = d + 2*d*cell_column  + cell_row*d;
			y = 0 + 2*h*cell_row;
		}
		if(n % 3 == 2)
		{
			x = (d/2.) + 2*d*cell_column  + cell_row*d;
			y = h + 2*h*cell_row;
		}
		printf("%lf %lf %d\n",x,y,state[n]);
		//printf("%lf %lf %d %lf %lf\n",x,y,a,h,2*h* (int) (n_celula/L));
	}
	printf("\n"); 
	printf("e\n");    printf("pause(%lf)\n",fps);
}
void snap_hexa(int state[N], int topologia[N], int t)   //    ./a.out | gnuplot
{

	int n,i,j; //n = i + 2j N
	double x,y,d=1,h=1.;

	printf("set title \"MCS = %d\" \n",t);
	printf("set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back\n");
	printf("set object 1 rect fc rgb \"black\" fillstyle solid 1.0 \n");
	printf("set view map\n");
	printf("set size ratio .9\n");
	//printf("set xrange[200:500]\n");
	//printf("set yrange[0:1500]\n");
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
