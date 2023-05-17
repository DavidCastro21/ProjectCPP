#include "XPM2.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>

#include <vector>
#include <iomanip>

namespace prog {
    std::string color_to_string(const Color& color) {
        std::stringstream ss;
        ss << color.red() << "_" << color.green() << "_" << color.blue();
        return ss.str();
    }

    
    Image* loadFromXPM2(const std::string& file) {
        std::ifstream filename(file);
        std::string _;
        filename >> _ >> _;
        int width, height, colors, chars_per_pixel;
        filename >> width >> height >> colors >> chars_per_pixel;
        Image *image = new Image(width, height);
        std::map<std::string, Color> color_map;
        std::string key, color;
        for (int i = 0; i < colors; ++i) {
            rgb_value red, green, blue;
            filename >> key >> _ >> color;
            red = std::stoi(color.substr(1, 2), nullptr, 16);
            green = std::stoi(color.substr(3, 2), nullptr, 16);
            blue = std::stoi(color.substr(5, 2), nullptr, 16);
            std :: cout << red << " " << green << " " << blue << std::endl;
            Color c(red, green, blue);
            color_map[key] = c;
        }
        int y = 0;
        std::string line;
        while(filename >> line){
            for (int x = 0; x < width; x+=chars_per_pixel) {
                image->at(x, y) = color_map[line.substr(x, chars_per_pixel)];
            }
            y++;
        }
        return image;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        /*
        std::ofstream out(file);
        if (!out.is_open()) {
            return;
        }
        int height = image->height();
        int width = image->width();
        std:: string _ = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        auto it = _.begin();
        std::map<Color, char> color_map;
        std::map<char, Color> color_map2;
        int color_count = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width ; ++x) {
                Color color = image->at(x, y);
                color_map[color] = *it;
                color_map2[*it] = color;
                color_count++;
            }
        }
        auto it2 = color_map.begin();
        out << "! XPM2" << std::endl;
        out << image->width() << " " << image->height() << " "<< color_count <<" 1" << std::endl;
        for(int i = 0; i < color_count; i++){
            out << it2->second << " c " << color_to_string(it2->first) << std::endl;
        }
        for (int y = 0; y < image->height(); ++y) {
            for (int x = 0; x < image->width(); ++x) {
                out << color_map[image->at(x, y)];
            }
            out << std::endl;
        }
        */
    }
}
