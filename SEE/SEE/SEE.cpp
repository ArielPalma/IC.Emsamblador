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
	float   sumax, sumay, sumaxy, sumax2;
	float m, b;
	float mediaX, mediaY, covarianza, varianza;
	sumaxy = Sumaxy(n, x, y);				// sumar los productos  
	sumax2 = Sumax2(n, x);					// sumar los valores de x^2 

	sumax = SumaVector(n, x);				// sumar los valores de x   

	sumay = SumaVector(n, y);				 // sumar los valores de y

	mediaX = (float)sumax / n;
	mediaY = (float)sumay / n;

	covarianza = Covarianza(sumaxy, n, mediaX, mediaY);
	varianza = Varianza(sumax2, n, mediaX);

	m = covarianza / varianza;

	b = Interseccion(m, mediaX, mediaY);
	// Mostrar los valores de la pendiente y de la interseccion
	printf("\n\npendiente(m) = %f \nInterseccion(b) = %f\n", m, b);

	// mostrar la formula
	printf("\nRecta de regresión sobre Y = %f*X+ (%f)\n", m, b);
}

float  Sumaxy(long  n, float x[], float y[]) {

	float suma = 0;
	int i = 0;

	for (int contador = n; contador >= 1; contador--, i++) {
		suma += (x[i] * y[i]);
	}
	return(suma);

}

float  Sumax2(long n, float x[])
{
	float suma = 0;
	for (int contador = 0; contador != n; contador++) {
		suma = (x[contador] * x[contador]) + suma;
	}
	return (suma);
}

float   SumaVector(long  n, float  aux[]) {


	long  suma = 0;

	for (int contador = 0; contador != n; contador++) {
		suma += aux[contador];
	}

	return (suma);
}
float Covarianza(float sumaxy, long n, float mediaX, float mediaY) {
	float result;
	float aux;

	result = (sumaxy / n);
	aux = mediaX * mediaY;
	result = result - aux;

	return result;
}
float Varianza(float sumax2, long  n, float mediaX) {
	float result;
	float aux = mediaX;

	result = (float)(sumax2 / n);
	aux = aux*mediaX;
	result = result - aux;

	return result;
}

float Interseccion(float pendiente, float mediaX, float mediaY) {
	float result;

	result = (pendiente*mediaX) - mediaY;

	return result;

}
