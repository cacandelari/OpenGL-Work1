/*******************************************************
* Homework 1: Rasterization                           *
* CS 148 (Summer 2016), Stanford University           *
*-----------------------------------------------------*
* Here you will implement the circle rasterization    *
* method you derived in the written portion of the    *
* homework.                                           *
* To compile this in linux:                           *
*        g++ hw1.cpp                                  *
* Then, run the program as follows:                   *
*        ./a.out 200                                  *
* to generate a 200x200 image containing a circular   *
* arc.  Note that the coordinate system we're using   *
* places pixel centers at integer coordinates and     *
* has the origin at the lower left.                   *
* Your code will generate a .ppm file containing the  *
* final image. These images can be viewed using       *
* "display" in Linux or Irfanview in Mac/Windows.     *
*******************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int theSize; //need to change size to "theSize" or something else because size is already used in one of the #includes.
			//Else I would have to remove using namespace std;
bool **image;

void renderPixel(int x, int y) {
	assert(x >= 0 && y >= 0 && x <= theSize && y <= theSize);
	image[x][y] = 1;

	//Nothing to be done here, this function works on its own
	// TODO:  light up the pixel's symmetric counterpart
}

void rasterizeArc(int radius) {

	/*
	Midpoint algorithm for circles was pulled from the readings:
	“Computer Graphics Principles and Practice - Foley
	et. al” section 3.2 “Scan Converting Lines” and section 3.3 “Scan Converting
	Circles”
	Specifically section 3.3
	*/

	int x = 0;//ints should be used to increase performance and speed
	int y = radius;
	int d = 1 - radius;//5/4 (1.25) when not a double == 1 anyway

	renderPixel(x, y);//Render the two relevant points for the positive X,Y coordinate quadrant
	renderPixel(y, x);

	while (y > x) {
		if (d < 0) {//choose E
			d += 2 * x + 3;//d is changed after each pixel added to choose which E or SE pixel should be added next
		}
		else {//select SE
			d += 2 * (x - y) + 5;
			y--;//Y is only decremented here because we only want the arc to decrease in Y height when SE is chosen
		}
		x++;//however we always want X to be increasing with each pixel placement, as shown in the final picture, the pixels coordinates that make up the arc must have 
			//incrementally increasing X values
			//OR, for the reflected coordinates, increasing Y values and potentially increasing X values
		renderPixel(x, y);
		renderPixel(y, x);
		/*
		This is done because, as shown in the reading, we technically only need 1/8th of the circle (here we are only drawing 1/4th)
		With that being said, we can reflect the points across the positive X,Y quadrant's diagonal midpoint to slowly fill in the circle's arc
		until they meet up at the midpoint, completing the arc
		*/
	}
	// TODO:  rasterize the arc using renderPixel to light up pixels
}

// You shouldn't need to change anything below this point.

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " circleSize\n";
		return 0;
	}

#ifdef _WIN32
	sscanf_s(argv[1], "%d", &theSize);
#else
	sscanf(argv[1], "%d", &theSize);
#endif
	if (theSize <= 0) {
		cout << "Image must be of positive size.\n";
		return 0;
	}

	// reserve image as 2d array
	image = new bool*[theSize + 1];
	for (int i = 0; i <= theSize; i++) image[i] = new bool[theSize + 1];

	rasterizeArc(theSize);

	char filename[50];
#ifdef _WIN32
	sprintf_s(filename, 50, "circle%d.ppm", theSize);
#else
	sprintf(filename, "circle%d.ppm", theSize);
#endif

	ofstream outfile(filename);
	outfile << "P3\n# " << filename << "\n";
	outfile << theSize + 1 << ' ' << theSize + 1 << ' ' << 1 << endl;

	for (int i = 0; i <= theSize; i++)
		for (int j = 0; j <= theSize; j++)
			outfile << image[theSize - i][j] << " 0 0\n";

	// delete image data
	for (int i = 0; i <= theSize; i++) delete[] image[i];
	delete[] image;

	return 0;
}

/*
Tried code: 

/*
int x0 = 0, x1 = radius, y0 = 0, y1 = 100;
int dx = x1 - x0, dy = y1 - y0;
int d = 2 * dy - dx;
int delE = 2 * dy, delNE = 2 * (dy - dx);
int x = x0, y = y0;
renderPixel(x, y);

while (x < x1) {
renderPixel(0,100);
renderPixel(100,0);
if (d <= 0) {
d += delE; x = x + 1;
}
else {
d += delNE; x = x + 1; y = y + 1;
}
renderPixel(x, y);

}
*/


/*
int x0 = 0, y0 = 0;
int x = radius;
int y = 0;
int d = 0;

while (x >= y) {
renderPixel(x0 + x, y0 + y);
renderPixel(x0 + y, y0 + x);

if (d <= 0) {
y += 1;
d += 2 * y + 1;
}
else {
x += 1;
d += 2 * x + 1;
}

}
*/