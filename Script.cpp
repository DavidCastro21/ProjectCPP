#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if (command == "replace") {
                replace();
                continue;
            }
            if (command == "fill") {
                fill();
                continue;
            }
            if (command == "h_mirror") {
                h_mirror();
                continue;
            }
            if (command == "v_mirror") {
                v_mirror();
                continue;
            }
            if (command == "add") {
                add();
                continue;
            }
            if (command == "crop") {
                crop();
                continue;
            }
            if (command == "rotate_left") {
                rotate_left();
                continue;
            }
            if (command == "rotate_right") {
                rotate_right();    
                continue;
            }
            if (command == "median_filter") {
                median_filter();
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
        int w = image->width();
        int h = image->height();
        for(int i = 0; i<h; i++){
            for(int j = 0; j<w; j++){
                Color c = image->at(j,i);
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
                image->at(j,i) = c;
            }
        }
    }
    void Script::to_gray_scale() {
        int h = image->height();
        int w = image->width();
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                Color c = image->at(j,i);
                int gray = (c.red() + c.green() + c.blue())/3;
                c.red() = gray;
                c.green() = gray;
                c.blue() = gray;
                image->at(j,i) = c;
            }
        }
    }
    void Script::replace() {
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        Color c1(r1, g1, b1);
        Color c2(r2, g2, b2);
        int h = image->height();
        int w = image->width();
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                Color c = image->at(j,i);
                if (c == c1){
                    image->at(j,i) = c2;
                }
            }
        }
    }
    void Script::fill() {
        int x, y, w, h, r, g, b;
        input >> x >> y >>w>>h>> r >> g >> b;
        for (int i = y; i < y + h; i++){
            for (int j = x; j<x+w ; j++){
                Color& c = image->at(j,i);
                c.red() = r;
                c.green() = g;
                c.blue() = b;
                image->at(j,i) = c;
            }
        }
    }
    void Script::h_mirror() {
        //Mirror image horizontally. Pixels (x, y) and (width() - 1 - x, y) for all 0 <= x < width() / 2 and 0 <= y < height().
        int h = image->height();
        int w = image->width();
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w/2 ; j++){
                Color c1 = image->at(j,i);
                Color c2 = image->at(w-1-j,i);
                image->at(j,i) = c2;
                image->at(w-1-j,i) = c1;
            }
        }
    }
    void Script::v_mirror() {
        //Mirror image vertically. Pixels (x, y) and (x, height() - 1 - y) for all 0 <= x < width() and 0 <= y < height() / 2.
        int h = image->height();
        int w = image->width();
        for (int i = 0; i < h/2; i++){
            for (int j = 0; j<w ; j++){
                Color c1 = image->at(j,i);
                Color c2 = image->at(j,h-1-i);
                image->at(j,i) = c2;
                image->at(j,h-1-i) = c1;
            }
        }
    }
    void Script::add() {
        //Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b), to the rectangle of the current image with top-left corner (x, y) of the current image. The rectangle affected in the current image has the same dimensions as the given image, and you may assume that rectangle is within the current image bounds.
        string filename;
        Color cor;
        int x, y;
        input >> filename >>cor>> x >> y;
        Image* img = loadFromPNG(filename);

        int newH = img->height();
        int newW = img->width();
        int h = image->height();
        int w = image->width();

        for (int i = 0; i < newW; i++){
            for (int j = 0; j<newH ; j++){
                Color& c2 = img->at(i,j);
                if ((c2 != cor) && (i+x < w) && (j+y < h)){
                    image->at(i+x,j+y) = c2;
                }
            }
        }
        delete img;
    }
    void Script::crop() {
        //Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h.You may assume that the rectangle is always within the current image bounds.
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image* img = new Image(w, h);
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                Color& c = image->at(j+x,i+y);
                img->at(j,i) = c;
            }
        }
        delete image;
        image = img;
    }
    void Script::rotate_left() {
        // Rotate image left by 90 degrees.
        int w = image->width();
        int h = image->height();
        Image* img = new Image(h, w, Color(255,255,255));
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                Color c = image->at(j,i);
                img->at(h-i-1,j) = c;
            }
        }
        delete image;
        image = img;
    }
    void Script::rotate_right() {
        // Rotate image right by 90 degrees.
        int w = image->width();
        int h = image->height();
        Image* img = new Image(h, w, Color(0,0,0));
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                img->at(i,w-j-1) = image->at(j,i);
            }
        }
        delete image;
        image = img;
    }
    void Script::median_filter(){
        //The general idea to replace each pixel (x, y) by “a median pixel” of neighboring pixels to (x,y) inside a window size of size ws * ws, where ws >= 3 is always an odd number (3, 5, 7, …). For a generic description of the median filter algorithm, you may check the Wikipedia page for the algorithm.
        int ws;
        input >> ws;
        int w = image->width();
        int h = image->height();
        Image* img = new Image(w, h, Color(0,0,0));
        for (int i = 0; i < h; i++){
            for (int j = 0; j<w ; j++){
                int r = 0;
                int g = 0;
                int b = 0;
                int count = 0;
                for (int k = i-ws/2; k <= i+ws/2; k++){
                    for (int l = j-ws/2; l <= j+ws/2; l++){
                        if ((k >= 0) && (k < h) && (l >= 0) && (l < w)){
                            Color c = image->at(l,k);
                            r += c.red();
                            g += c.green();
                            b += c.blue();
                            count++;
                        }
                    }
                }
                Color c(r/count, g/count, b/count);
                img->at(j,i) = c;
            }
        }
        delete image;
        image = img;
    }
}
