
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/time.h>
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


void pasar_numeros(vector<int> & v, string mensaje){
	for(int i=0; i< mensaje.length(); i++){
		v.push_back(alf(mensaje[i]));
	}
}

void encriptar(int i) {
	long int p, q, n, fi, e, inverso;
	vector<int> mensaje_entero, mensaje_des;
	vector<long long> mensaje_cifrado;
	string mensaje, mens2, aux, archivo,aux1;
	
	ostringstream convert;
	convert << i;
	aux1="";
	archivo = "texto" + convert.str();
	archivo += ".txt";
	
	cout << archivo;
	mensaje = "";
	
	cout << " ENCRIPTAR:\n\n";
	
	ifstream fe(archivo.c_str());
	
	if (fe.is_open()) {
		fe >> aux1;
		while (!fe.eof())
		{
		   /* Escribimos el resultado */
		   mensaje+=aux1;
		   /* Leemos la siguiente línea */
		   fe >> aux1;
		}
	    }
	//cout<<"mensaje"<<mensaje<<endl;
	do {
		p = rand() % 50 + 50;
	} while (!es_primo(p));

	do {
		q = rand() % 50 + 50;
	} while (!es_primo(q));
	cout << " p : " << p << "\n q : " << q;
	n = p * q;
	fi = (p - 1) * (q - 1);
	do {
		e = rand() % (fi - 2) + 2;
	} while (mcd(e, fi) != 1);
	cout << "\n fi : " << fi;
	cout << "\n e : " << e;
	inverso = Inverso_Zn(e, fi);
	cout << "\n inverso " << inverso << endl;
	cout << "\n\n clave publica : (" << e << " , " << n << ")";
	cout << "\n clave privada : " << inverso << endl << endl;

	mens2="";
	cout<<"Tamaño: "<<mensaje.length()<<endl;
	/*cin.get();
	cout<<" Mensaje a encriptar : ";
	getline(cin,mensaje);
	cout << "mensaje: " << mensaje << endl;
	*/
	pasar_numeros(mensaje_entero, mensaje);
	/*
	cout << "En Numeros" << endl;
	
	for(int i=0; i< mensaje_entero.size(); i++){
		cout  << mensaje_entero[i];	
	}
	*/
	cout << endl << "Cifrado" << endl;
	struct timeval start, end;
	srand(time_t(NULL));
	gettimeofday(&start, NULL);
	for(int i=0; i< mensaje_entero.size(); i++){
		mensaje_cifrado.push_back(Exponenciacion_Zn(mensaje_entero[i], e ,n));
		//cout  << mensaje_cifrado[i];	
	}
	
	cout << endl << "Descifrado" << endl;
	
	for(int i=0; i<mensaje_cifrado.size(); i++){
		mensaje_des.push_back(Exponenciacion_Zn(mensaje_cifrado[i], inverso, n));
		mens2 += alfabeto[mensaje_des[i]-1];
	}
	gettimeofday(&end, NULL);
	float delta;
	delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
	cout<<"tiempo= "<<delta<<endl;
	/*
	for(int i=0; i< mensaje_des.size(); i++){
		cout  << mensaje_des[i];	
	}
	*/
	
	//cout << endl << "Mensaje 2: " << mens2 << endl;

}

int main() {

	cout << "\n\n         ALGORITMO RSA\n\n";
	for(int i= 1; i <= 4; i++){
		encriptar(i);
	}

}
