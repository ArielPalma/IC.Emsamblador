/*aa
 * Ejemplo en CUDA de implementación de una dilatacion 
 * para procesamiento de imágenes
 *
 * Adaptacion por Francisco Florez-Revuelta (2016) de un codigo de calculo de mediana
 * desarrollado por Sergio Orts-Escolano
 * Copyright Universidad de Alicante, 2012 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if _WIN32
	#include <Windows.h>
#else
	#include <sys/types.h>
	#include <sys/time.h>
#endif

// Ficheros de inclusión para que funcione el intellisense en Visual Studio
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "EasyBMP.h"

#if _WIN32
	typedef LARGE_INTEGER timeStamp;
	void getCurrentTimeStamp(timeStamp& _time);
	timeStamp getCurrentTimeStamp();
	double getTimeMili(const timeStamp& start, const timeStamp& end);
	double getTimeSecs(const timeStamp& start, const timeStamp& end);
#endif

// Dimensiones de la imagen a procesar
int WIDTH;
int HEIGHT;

// Funciones auxiliares
double get_current_time();
void checkCUDAError(const char*);


// Tamaño del elemento estructurante es 2*SIZE_STRUCTURING_ELEMENT+1
int SIZE_STRUCTURING_ELEMENT;

// Tamaño de grid y bloque CUDA
#define GRID_W  32
#define GRID_H  32
#define BLOCK_W 32
#define BLOCK_H 32

// Buffers con el halo correspondiente. El halo crea un borde alrededor de la imagen de color 0 que permite evitar errores por salirse de la imagen 
unsigned char *host_input;
unsigned char *gpu_output;
unsigned char *host_output;

// CUDA kernel dilatacion
__global__ void dilatacion(unsigned char *d_output, unsigned char *d_input, int width, int SIZE_STRUCTURING_ELEMENT)
{
	
	/*
	* Calculamos la fila y columna global para este hilo a partir de
	* Recuerda sumar SIZE_STRUCTURING_ELEMENT para tener en cuenta el halo introducido
	*/
	// TO DO
	

	// Obtener dilatacion para el pixel correspondiente a este hilo 
	// TO DO
	

}


// El main tiene dos argumentos: nombre del fichero de la imagen (tiene que ser BMP) y el tamaño del elemento estructurante
int main(int argc, char *argv[])
{
	int x, y, xWindow, yWindow, x2, y2;
	int i;
	int errors;
	int maxColor;

	double start_time_inc_data, end_time_inc_data;
	double cpu_start_time, cpu_end_time;

	unsigned char *d_input, *d_output, *d_edge, *tmp;

	unsigned char *input_image;
	unsigned char *output_image;
	int rows;
	int cols;

	// Leemos la imagen 
	BMP Image;
	Image.ReadFromFile(argv[1]);
	
	// Leemos el tamaño del elemento estructurante
	SIZE_STRUCTURING_ELEMENT = atoi(argv[2]);

	// Calculo del tamaño de la imagen
	WIDTH = Image.TellWidth();
	HEIGHT = Image.TellHeight();
	
	// Reserva de memoria para 
	host_input = (unsigned char*)calloc(((HEIGHT + 2 * SIZE_STRUCTURING_ELEMENT) * (WIDTH + 2 * SIZE_STRUCTURING_ELEMENT)), sizeof(unsigned char)); 
	gpu_output = (unsigned char*)calloc(((HEIGHT + 2 * SIZE_STRUCTURING_ELEMENT) * (WIDTH + 2 * SIZE_STRUCTURING_ELEMENT)), sizeof(unsigned char));
	host_output = (unsigned char*)calloc(((HEIGHT + 2 * SIZE_STRUCTURING_ELEMENT) * (WIDTH + 2 * SIZE_STRUCTURING_ELEMENT)), sizeof(unsigned char));

	// Alojamos memoria en el host para alojar la imagen
	input_image = (unsigned char*)calloc(((HEIGHT * WIDTH) * 1), sizeof(unsigned char));

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			input_image[i*WIDTH + j] = Image(i, j)->Red;
		}
	}


	/*
	* Versión CPU dilatacion
	*/
	
	printf("Grid size: %dx%d\n", GRID_W, GRID_H);
	printf("Block size: %dx%d\n", BLOCK_W, BLOCK_H);

	// Calculamos memoria necesaria para alojar la imagen junto con el halo
	size_t memSize = (WIDTH + 2 * SIZE_STRUCTURING_ELEMENT) * (HEIGHT + 2 * SIZE_STRUCTURING_ELEMENT) * sizeof(unsigned char);
		
	/* Reservamos memoria en la GPU */
	cudaMalloc(&d_input, memSize);
	cudaMalloc(&d_output, memSize);

	/* Inicializamos a cero para asegurar que el halo tiene valores correctos */
	for (y = 0; y < HEIGHT + 2*SIZE_STRUCTURING_ELEMENT; y++) {
		for (x = 0; x < WIDTH + 2 * SIZE_STRUCTURING_ELEMENT; x++) {
			host_input[y*WIDTH+x] = 0;
		}
	}

	/* Copiamos la imagen al buffer con el halo */
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			host_input[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH+x + SIZE_STRUCTURING_ELEMENT] = input_image[y*WIDTH + x];
		}
	}

	start_time_inc_data = get_current_time();

	/*
	* Copiamos todos los arrays a la memoria de la GPU
	*/
	cudaMemcpy( d_input, host_input, memSize, cudaMemcpyHostToDevice);
	cudaMemcpy( d_output, host_input, memSize, cudaMemcpyHostToDevice);

	// Ejecutar dilatacion en la GPU
	/* Ejecución kernel 2D */
	// TO DO - Calcular tamaño de bloque y grid para la correcta ejecucion del kernel
	// TO DO - Ejecutar el kernel
	
	// Copiamos de la memoria de la GPU a 
	cudaMemcpy(gpu_output, d_input, memSize, cudaMemcpyDeviceToHost);

	end_time_inc_data = get_current_time();

	checkCUDAError("Dilatacion CUDA: ");

	/*
	* Versión CPU dilatacion
	*/
	cpu_start_time = get_current_time();

	unsigned char temp;
	int numcols = WIDTH + SIZE_STRUCTURING_ELEMENT;
	
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			maxColor = 0;
			for (yWindow = -SIZE_STRUCTURING_ELEMENT; yWindow < SIZE_STRUCTURING_ELEMENT; yWindow++) {

				y2 = y + yWindow + SIZE_STRUCTURING_ELEMENT;
				
				for (xWindow = -SIZE_STRUCTURING_ELEMENT; xWindow < SIZE_STRUCTURING_ELEMENT; xWindow++) {
					x2 = x + xWindow + SIZE_STRUCTURING_ELEMENT;
				
					if (host_input[y2*WIDTH+x2]>maxColor)
						maxColor = host_input[y2*WIDTH + x2];
				}
			}
			host_output[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH+x + SIZE_STRUCTURING_ELEMENT] = maxColor;
		}
	}
  
	cpu_end_time = get_current_time();

  /* Comprobamos que los resultados de la GPU coinciden con los calculados en la CPU */
  errors = 0;
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
		if (host_input[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH + x + SIZE_STRUCTURING_ELEMENT] != gpu_output[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH + x + SIZE_STRUCTURING_ELEMENT]) {
        errors++;
        printf("Error en %d,%d (CPU=%i, GPU=%i)\n", x, y, \
			host_output[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH+x + SIZE_STRUCTURING_ELEMENT], \
			gpu_output[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH + x + SIZE_STRUCTURING_ELEMENT]);
      }
    }
  }
  
  if (errors == 0) printf("\n\n ***TEST CORRECTO*** \n\n\n");
  
  output_image = (unsigned char*)calloc(((WIDTH * HEIGHT) * 1), sizeof(unsigned char));

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
		output_image[y*WIDTH + x] = host_output[(y + SIZE_STRUCTURING_ELEMENT)*WIDTH + x + SIZE_STRUCTURING_ELEMENT];
    }
  }

  cudaFree(d_input);
  cudaFree(d_output);

  printf("Tiempo ejecución GPU (Incluyendo transferencia de datos): %fs\n", \
	 end_time_inc_data - start_time_inc_data);
  printf("Tiempo de ejecución en la CPU                          : %fs\n", \
	 cpu_end_time - cpu_start_time);

	// Copiamos el resultado al formato de la libreria y guardamos el fichero BMP procesado
  for( int i=0 ; i < Image.TellHeight() ; i++)
  {
		for( int j=0 ; j < Image.TellWidth() ; j++)
		{
			Image(i,j)->Red = output_image[i*WIDTH+j];
			Image(i,j)->Green = output_image[i*WIDTH+j];
			Image(i,j)->Blue = output_image[i*WIDTH+j];
		}
  }
	// Guardamos el resultado de aplicar el filtro en un nuevo fichero
	Image.WriteToFile( "resultado_dilatacion.bmp" );

	getchar();
  return 0;
}


/* Funciones auxiliares */

#if _WIN32
	void getCurrentTimeStamp(timeStamp& _time)
	{
			QueryPerformanceCounter(&_time);
	}

	timeStamp getCurrentTimeStamp()
	{
			timeStamp tmp;
			QueryPerformanceCounter(&tmp);
			return tmp;
	}

	double getTimeMili()
	{
			timeStamp start;
			timeStamp dwFreq;
			QueryPerformanceFrequency(&dwFreq);
			QueryPerformanceCounter(&start);
			return double(start.QuadPart) / double(dwFreq.QuadPart);
	}
#endif 

double get_current_time()
{
	#if _WIN32 
		return getTimeMili();
	#else
		static int start = 0, startu = 0;
		struct timeval tval;
		double result;

		if (gettimeofday(&tval, NULL) == -1)
			result = -1.0;
		else if(!start) {
			start = tval.tv_sec;
			startu = tval.tv_usec;
			result = 0.0;
		}
		else
			result = (double) (tval.tv_sec - start) + 1.0e-6*(tval.tv_usec - startu);
		return result;
	#endif
}

/* Función para comprobar errores CUDA */
void checkCUDAError(const char *msg)
{
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString( err) );
        exit(EXIT_FAILURE);
    }                         
}

