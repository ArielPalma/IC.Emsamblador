#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

float  Sumaxy(long n, float  x[], float y[]);
float  Sumax2(long  n, float x[]);
float  SumaVector(long n, float  aux[]);
float Covarianza(float  sumaxy, long n, float mediaX, float mediaY);
float Varianza(float  sumax2, long  n, float mediaX);
float Interseccion(float pendiente, float mediaX, float mediaY);
float getRandomNumber(int max);
void calcularRecta(float x[], float y[], long n);

const long  N = 100000;

int main(void) {
	int n;
	float *x, *y;
	srand(1);
	long  tinicial = 0, tfinal = 0, tmedia = 0;
	for (int i = 1; i <15; i++) {

		n = powl(3, i);

		x = new float[n];
		y = new  float[n];
		for (long j = 0; j < n; j++) {
			x[j] = getRandomNumber(41);
			y[j] = getRandomNumber(41);
		}
		cout << endl;
		cout << endl << "Prueba " << i << ", " << n << "puntos: " << endl;
		tinicial = clock();
		calcularRecta(x, y, n);
		tfinal = clock();
		tmedia += (tfinal - tinicial);

		cout << "tiempo: " << tmedia << endl;
	}

	system("pause");
}

float getRandomNumber(int max) {
	// Genera un nero aleatorio entre 0 y maximo-1
	int na;

	na = (float)(((float)max)*rand() / (RAND_MAX + 1.0));
	return na;
}


void calcularRecta(float x[], float y[], long n) {
	float  sumax, sumay, sumaxy, sumax2;
	float m, b;
	float mediaX, mediaY, covarianza, varianza;
	sumaxy = Sumaxy(n, x, y);
	// sumar los productos  
	sumax2 = Sumax2(n, x);					// sumar los valores de x^2 

	sumax = SumaVector(n, x);				// sumar los valores de x   

	sumay = SumaVector(n, y);				 // sumar los valores de y

	__asm
	{
		mov eax, n;
		cvtsi2ss xmm6, eax;
		movss xmm0, sumax;
		movss xmm1, sumay;
		divps xmm0, xmm6;
		divps xmm1, xmm6;
		movss mediaX, xmm0;
		movss mediaY, xmm1;
	}

	covarianza = Covarianza(sumaxy, n, mediaX, mediaY);
	varianza = Varianza(sumax2, n, mediaX);
	__asm
	{
		movss xmm0, covarianza;
		movss xmm1, varianza;
		divps xmm0, xmm1;
		movss m, xmm0;

	}
	b = Interseccion(m, mediaX, mediaY);
	// Mostrar los valores de la pendiente y de la interseccion
	printf("\n\npendiente(m) = %f \nInterseccion(b) = %f\n", m, b);

	// mostrar la formula
	printf("\nRecta de regresión sobre Y = %f*X+ (%f)\n", m, b);
}

float  Sumaxy(long  n, float x[], float y[]) {
	float suma = 0.0;
	float suma1 = 0.0;
	__asm {

		mov eax, x;
		mov ebx, y;
		mov ecx, 0;
		movss xmm7, suma;
	for1:
		cmp ecx, n
			jae fin;
		movss xmm0, [eax + 4 * ecx];
		movss xmm1, [ebx + 4 * ecx];
		mulps xmm1, xmm0;
		addps xmm7, xmm1;
		inc ecx;
		jmp for1;
	fin:
		movss suma, xmm7;
	}



	return(suma);

}

float  Sumax2(long n, float x[])

{
	float suma = 0.0;

	__asm {
		mov eax, x;
		mov ecx, 0;
		movss xmm7, suma;
	for1:
		cmp ecx, n
			jae fin;
		movss xmm0, [eax + 4 * ecx];
		movss xmm1, [eax + 4 * ecx];
		mulps xmm1, xmm0;
		addps xmm7, xmm1;
		inc ecx;
		jmp for1;
	fin:
		movss suma, xmm7;


	}
	return (suma);

}



float  SumaVector(long  n, float  aux[]) {

	float suma = 0.0;

	__asm {
		mov eax, aux;
		mov ecx, 0;
		movss xmm7, suma;
	for1:
		cmp ecx, n
			jae fin;
		movss xmm0, [eax + 4 * ecx];
		addps xmm7, xmm0;
		inc ecx;
		jmp for1;
	fin:
		movss suma, xmm7;


	}
	return (suma);
}
float Covarianza(float  sumaxy, long n, float mediaX, float mediaY) {
	float result;
	__asm
	{
		mov eax, n;
		cvtsi2ss xmm6, eax;
		movss xmm0, mediaX;
		movss xmm1, mediaY;
		movss xmm3, sumaxy;

		divps xmm3, xmm6;
		mulps xmm0, xmm1;
		subps xmm3, xmm0;

		movss result, xmm3;
	}
	return result;

}
float Varianza(float sumax2, long  n, float mediaX) {
	float result;
	float aux = mediaX;
	__asm
	{
		mov eax, n;
		cvtsi2ss xmm6, eax;
		movss xmm0, mediaX;
		movss xmm1, mediaX;
		movss xmm3, sumax2;

		divps xmm3, xmm6;
		mulps xmm0, xmm1;
		subps xmm3, xmm0;

		movss result, xmm3;
	}
	return result;
}

float Interseccion(float pendiente, float mediaX, float mediaY) {
	float result;
	__asm
	{
		movss xmm0, pendiente;
		movss xmm1, mediaX;
		movss xmm2, mediaY;
		mulps xmm0, xmm1;
		subps xmm2, xmm0;
		movss result, xmm2;
	}
	return result;

}
