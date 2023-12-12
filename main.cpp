//21012737
//Zheng Wang

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


class pixel_class {
private:
    int red, green, blue;
    bool exclude;  // if true, do not check this pixel
public:
    void loaddata(int v1, int v2, int v3);
    void datatofile(fstream & ppmfile);
    int getR() { return red; }
    int getG() { return green; }
    int getB() { return blue; }
    void setexclude(bool ex) { exclude = ex; }
    bool getexclude() { return exclude; }
};

void loadButtons();
void boxDraw(int xin, int xax, int yin, int yax, int R, int G, int B);


int total = 0, xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;  // MUST be global if used
int screenx, screeny, maxcolours;   // you must use these
pixel_class picture[600][600];      // you must use this

void findConnectedPixels(int x, int y) {

    if (picture[x][y].getexclude() == true) { return; } // base case one
    if (picture[x][y].getR() <= 128) { return; } // base case two
    // use x and y to assist in calculating xmax, xmin, ymax, ymin

    ++total;
    if(x > xmax)
        xmax = x;
    if(x < xmin)
        xmin = x;
    if(y > ymax)
        ymax = y;
    if(y < ymin)
        ymin = y;

    picture[x][y].setexclude(true); // do not look at this pixel again

    findConnectedPixels (x - 1, y);
    findConnectedPixels (x, y - 1);
    findConnectedPixels (x + 1, y);
    findConnectedPixels (x, y + 1);
}

int main() {

    // Step 1 : read in the image from Buttons.ppm
    loadButtons();

    // Step 2 : identify buttons and draw boxes

    for(int y = 0; y < screeny; ++y)
        for(int x = 0; x < screenx; ++x){
            findConnectedPixels(x, y);
            if(total){
                //Judge broken and Draw the box
                if(total > 7650 && total < 8300)
                    boxDraw(xmin, xmax, ymin, ymax, 0, 255, 0);
                else
                    boxDraw(xmin, xmax, ymin, ymax, 255, 0, 0);
            }
            total = 0;
            xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;
        }

    // Step 3 : output the final .ppm file
    fstream reDraw;
    reDraw.open("a.ppm", fstream::out);
    reDraw << "P3\n";
    reDraw << "# a.bbm" << endl;
    reDraw << 600 << " " << 600 << endl;
    reDraw << 256 << endl;

    for(int y = 0; y < screeny ; ++y){
        for(int x = 0; x < screenx; ++x){
            picture[x][y].datatofile(reDraw);
        }
        reDraw << endl;
    }
}

void boxDraw(int xin, int xax, int yin, int yax, int R, int G, int B){
    for(int x = xin; x <= xax; ++x){
        picture[x][yin].loaddata(R, G, B);
        picture[x][yin].setexclude(true);
        picture[x][yax].loaddata(R, G, B);
        picture[x][yax].setexclude(true);
    }

    for(int y = yin; y <= yax; ++y){
        picture[xin][y].loaddata(R, G, B);
        picture[xin][y].setexclude(true);
        picture[xax][y].loaddata(R, G, B);
        picture[xax][y].setexclude(true);
    }

}

void loadButtons() {
    // load the picture from Buttons.ppm
    int x, y, R, G, B;
    fstream infile;
    string infilename, line;
    infilename = "Buttons.ppm";
    infile.open(infilename.c_str(), fstream::in);
    if (infile.is_open() == false) {
        cout << "ERROR: not able to open " << infilename << endl;
        exit(2);
    }
    getline(infile, line);  // this line is "P3"
    getline(infile, line);  // this line is "# filename"
    infile >> screenx >> screeny;  // this line is the size
    infile >> maxcolours;  // this line is 256
    for (y = 0; y < screeny; y++) {
        for (x = 0; x < screenx; x++) {
            infile >> R >> G >> B;
            picture[x][y].loaddata(R, G, B);
            picture[x][y].setexclude(false);
        }
    }
    infile.close();
}

//--------------- methods for the pixel_class ------------
void pixel_class::loaddata(int v1, int v2, int v3) {
    red = v1;
    green = v2;
    blue = v3;
}

void pixel_class::datatofile(fstream & ppmfile) {
    // write the data for one pixel to the ppm file
    ppmfile << red << " " << green;
    ppmfile << " " << blue << "  ";
}