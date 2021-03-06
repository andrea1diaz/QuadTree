#include <iostream>
#include <vector>
#include "quad_tree.h"

using namespace std;

CImg<char> Binarizar(CImg<float> & img, int umbral) {
    CImg<char> R(img.width(),img.height()); 
	for(int i = 0; i < img.width(); i++) {
 	    for(int j = 0; j < img.height(); j++) {
            int r = img(i,j,0);
            int g = img(i,j,1);
            int b = img(i,j,2);
            if ( (r+g+b)/2 > umbral) 
		        R(i,j) = 255;
            else
                R(i,j) = 0;
        }
    }
    
    return R;    
} 


int main() {
    CImg<float> A("tigre.jpg");
    CImg<char> R =  Binarizar(A,40);
//    A.display();
//    R.display();

    int input;
    do {
        cout << "Comprimir (1) o reconstruir (2) o ambos (3): "; cin >> input;
    } while (input != 1 && input != 2 && input != 3);

    if (input == 1) {
        QuadTree qt(R);
    } else if (input == 2) {
        QuadTree qt;
        qt.reconstruir();
    } else {
        QuadTree qt(R);
        qt.reconstruir();
    }
   
    return 0;
}
