#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class LinearBlend {
    private:
        static const int MAXALPHA = 100;
        Mat *m1, *m2; // Punteros por eficiencia

    public:
        LinearBlend(Mat &img1, Mat &img2, int initialSliderValue) {
            // Inicializar valores
            m1 = &img1;
            m2 = &img2;

            // Crear slider. Importante: el ultimo parametro es this. Se recibe desde onChange.
            createTrackbar("Blend slider", "Window", &initialSliderValue, MAXALPHA, onChange, this);

            // Llamar a la funcion para mostrar algo antes de llegar a modificar el slider
            process(initialSliderValue);
        }

        static void onChange(int v, void *ptr) {
            // Conversion de tipo y llamada a la funcion que procesa
            LinearBlend *lb = (LinearBlend*)ptr;
            lb->process(v);
        }

        void process(int v) {  // Funcion que procesa el resultado
            double alpha = (double)v/MAXALPHA;
            double beta  = 1 - alpha;

            // Guardamos en dst la imagen procesada
            Mat dst = *m1 * alpha + *m2 * beta;

            // Mostramos la imagen
            imshow("Window",dst);
        }
};


int main() {
    // Importante: Las imagenes deben ser del mismo tipo y dimensiones
    Mat img1 = imread("LinuxLogo.jpg");
    Mat img2 = imread("WindowsLogo.jpg");

    if (!img1.data || ! img2.data || img1.channels()!=img2.channels() || img1.cols!=img2.cols || img1.rows!=img2.rows) {
        cout << "Error cargando imagenes" << endl;
        return -1;
    }

    namedWindow("Window", CV_WINDOW_AUTOSIZE); // Necesario cuando usamos trackbar
    LinearBlend(img1, img2, 20);

    waitKey(0);

    return 0;
}

