
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h> 
#define N 4
using namespace std;
const char alfabeto[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
						  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
						  't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 
						  'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
						  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
						  'W', 'X', 'Y', 'Z', ' ', '1', '2', '3', '4', '5', 
						  '6', '7', '8', '9', '0', ',', '.', '%', '\'', '\"',
						  '-', ';', ':', '?', '!'};



bool es_primo(long int n)
{
  for(int i=2;i<n;i++)
    if(n%i==0)
     return false;

   return true;
}

int alf(char a){
	for(int i = 0; i < 73; i++){
		if(a == alfabeto[i]){
			return i+1;
		}
	}
}

unsigned long long  Exponenciacion_Zn(unsigned long long  a,unsigned long long  k,unsigned long long  n)
{
	// convertimos "k" a binario
	unsigned long long numero=k;
	
	unsigned long long bin[300];
	unsigned long long  ind=0;
	while(numero>=2)
	{
		bin[ind++]=numero%2;
		numero/=2;      
	}     
	bin[ind]=numero;    
	unsigned long long  tam=ind+1;
	// for(int i=0;i<tam;i++)
	// cout<<bin[i]<<endl;     
	/////////////////////////////   
	
	unsigned long long  b=1;
	if(k==0)
		return b;
	
	unsigned long long  A=a;   
	for(int i=(tam-1);i>=0;i--)
	{
		b=(b*b)%n;
		if(bin[i]==1)
			b=(A*b)%n;   
		// cout<<"b :"<<b<<endl;   
	}
	return b;
}
int mcd(long int aux_e,long int aux_fi)
{

	long int max, min, r;

	// identificamos el mayor y menor de los numeros
	if (aux_e >= aux_fi) {
		max = aux_e;
		min = aux_fi;
	} else {
		max = aux_fi;
		min = aux_e;
	}

	while (min != 0) {
		r = max % min;
		max = min;
		min = r;
	}
	return max;
}

vector<int> euclides(int a, int b)
{
	vector<int> aux(3,0);
	int d=0,x=0,y=0;
        if(b==0)
        {
            aux[0]=(int)a;
            aux[1]=1;
            aux[2]=0;
            return aux;
        }
        int x2 = 1, x1 = 0, y2 = 0, y1 = 1;
        double q=0, r=0;
        while(b>0)
        {
            q = (a/b);
            r = a%b;
            x = (int) (x2-q*x1);
            y = (int) (y2-q*y1);
            a = b;
            b = r;
            x2 = x1;
            x1 = x;
            y2 = y1;
            y1 = y;
            d=(int) a;
        }
	aux[0]=d;
 	aux[1]=(int)x2;
        aux[2]=(int)y2;
	return aux;
}



int Inverso_Zn(int a,int n)
{
	vector<int> mcd(3,0);
	int y=0;
	if(a>n)
        {
            mcd=euclides(a,n);
        }
        else
        {
            mcd=euclides(n,a);
        }
	if(mcd[0] >1)
        {
            y=0;
        }
        else
        {
            y=(int) mcd[2];
            if(y<0)
            {
                y=(int) (y+n);
            }
        }
	return y;
}


void pasar_numeros(int v[], string &mensaje){
	for(int i=0; i< mensaje.length(); i++){

		v[i]=alf(mensaje[i]);
		//printf("%d\n", v[i]);
	}
}

void encriptar(int argc, char **argv) {
	int   pid, npr;
	//MPI_Status    info; 
	//MPI_Request request;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
	MPI_Comm_size(MPI_COMM_WORLD, &npr); 
	string mensaje="";
	int  tam=0;
	long int p, q, n, fi, e, inverso;
	if(pid==0)
	{
		printf("\n\n         ALGORITMO RSA\n\n");
		
	

	string aux,aux1;
		aux1="";
		mensaje = "";
		
		printf(" ENCRIPTAR:\n\n");
		
		ifstream fe("textoB.txt");
		
		if (fe.is_open()) {
			fe >> aux1;
			while (!fe.eof())
			{
			   /* Escribimos el resultado */
			   mensaje+=aux1 + " ";
			   /* Leemos la siguiente línea */
			   fe >> aux1;
			}
		}
		//printf("mensaje%s\n",mensaje.c_str());
		do {
			p = rand() % 50 + 50;
		} while (!es_primo(p));

		do {
			q = rand() % 50 + 50;
		} while (!es_primo(q));
		printf(" p : %d\n q : %d", p, q);
		n = p * q;
		fi = (p - 1) * (q - 1);
		do {
			e = rand() % (fi - 2) + 2;
		} while (mcd(e, fi) != 1);
		printf("\n fi : %d", fi);
		printf("\n e : %d", e);
		inverso = Inverso_Zn(e, fi);
		printf("\n inverso %d\n", inverso);
		printf("tamaño %d\n", mensaje.length());
		printf("\n\n clave publica : ( %d , %d )", e, n);
		printf("\n clave privada : %d \n", inverso);
		
	}
	
	tam=mensaje.length();
	//printf("tam=%d\n",tam);
	int *mensaje_entero;
	mensaje_entero=(int *)malloc(tam*sizeof(int));
	int puntero[1];
	

	
	if(pid==0)
	{
		pasar_numeros(mensaje_entero, mensaje);
		/*
		cout << "En Numeros" << endl;
		
		for(int i=0; i< mensaje_entero.size(); i++){
			cout  << mensaje_entero[i];	
		}
		*/
		
		

		
		
		/*
		for(int i=0; i< mensaje.length(); i++){

			mens_cifrado[i]=(Exponenciacion_Zn(mensaje_entero[i], e ,n));
			
		}
		printf("\n");
		printf("paso a Descifrado\n");
		for(int  i=0; i<mensaje.length(); i++){

			mens_des[i]=Exponenciacion_Zn(mens_cifrado[i], inverso, n);	
			mens2[i]=alfabeto[mens_des[i]-1];

		}
		gettimeofday(&end, NULL);

		delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
	         end.tv_usec - start.tv_usec) / 1.e6;
		printf("\n Tej = %f s \n", delta);
		for(int i=0;i<tam;i++)
		{
			printf("%c", mens2[i]);
		}
		*/
	}

	int size[4];
	size[0]=tam;
	size[1]=n;
	size[2]=e;
	size[3]=inverso;
	MPI_Bcast(&size[0],4,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	tam=size[0];
	n=size[1];
	e=size[2];
	inverso=size[3];
	
	int *mens_cifrado;
	char *mens2;
	int *mens_des;
	mens_cifrado=(int *)malloc(tam*sizeof(int));
	mens2=(char *)malloc(tam*sizeof(char));
	mens_des=(int *)malloc(tam*sizeof(int));
	//printf("\n\n clave publica : ( %d , %d )  clave privada : %d    desde pid: %d", e, n, inverso, pid);
	MPI_Barrier(MPI_COMM_WORLD);

	
	int tamao=100000;
	int VB[tamao];
	int *VC;
	VC=(int *)malloc(tam*sizeof(int));

	int VE[tamao];
	int VD[tamao];
	struct timeval start, end;
	float delta;
	if(pid==0){
		gettimeofday(&start, NULL);
	}
	for(int j=0; j < tam ; j = j +(tamao*npr)){
		MPI_Scatter(&mensaje_entero[0],tamao,MPI_INT,&VB[0],tamao,MPI_INT,0,MPI_COMM_WORLD);
		//printf("Sin cifrar VB datos: %d %d %d %d   desde pid: %d\n ",VB[0],VB[1],VB[2],VB[3],pid);
		//printf("Cifrado desde pid: %d\n",pid);

		for(int i=0; i < tamao; i++){
			VD[i]=(Exponenciacion_Zn(VB[i], e ,n));
		}
		//printf("Cifrados VD datos: %d %d %d %d   desde pid: %d\n ",VD[0],VD[1],VD[2],VD[3],pid);
		//printf("Desifrado desde pid: %d\n",pid);
		for(int i=0; i < tamao; i++){
			VE[i]=(Exponenciacion_Zn(VD[i], inverso ,n));
		}

		//printf("Descifrados VE datos: %d %d %d %d   desde pid: %d\n ",VE[0],VE[1],VE[2],VE[3],pid);
		//printf("VB:  dato: %d  %d\n ",VB[0],pid);
		//printf("tamaño %d  pid: %d\n",size[0],pid);
		MPI_Gather(&VE[0],tamao,MPI_INT,&VC[j],tamao,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD); 
	}
	
	if(pid==0){

		gettimeofday(&end, NULL);
		delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
	    end.tv_usec - start.tv_usec) / 1.e6;
		printf("\n Tej = %f s \n", delta);


		for(int i=0; i < tam; i++){
			mens2[i]=alfabeto[VC[i]-1];
			//printf("%c", mens2[i]);
		}
	}
	

	

}

int main(int argc, char **argv) {
	srand(time_t(NULL));
	encriptar(argc,argv);
	MPI_Finalize();
  	return 0; 
}
