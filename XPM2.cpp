#include "XPM2.hpp"
#include "Image.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

namespace prog {
    std::string color_to_string(const Color& color) {
        std::stringstream ss;
        ss << color.red() << "_" << color.green() << "_" << color.blue();
        return ss.str();
    }

    
    Image* loadFromXPM2(const std::string& filename) {
        std :: ifstream file(filename);
        if (!file.is_open()) {
            return nullptr;
        }
        std::string line;
        getline(file, line);
        if (line != "/* XPM2 */") {
            return nullptr;
        }
        getline(file, line);
        std::stringstream ss(line);
        int width, height, colors, chars_per_pixel;
        ss >> width >> height >> colors >> chars_per_pixel;
        if (chars_per_pixel != 1) {
            return nullptr;
        }
        std::map<std::string, Color> color_map;
        std::vector<std::vector<Color>> image_data;
        for (int i = 0; i < colors; ++i) {
            getline(file, line);
            std::stringstream ss(line);
            char c;
            rgb_value r, g, b;
            ss >> c >> r >> g >> b;
            color_map[color_to_string({r, g, b})] = {r, g, b};
        }
        for (int y = 0; y < height; ++y) {
            getline(file, line);
            std::vector<Color> row;
            for (int x = 0; x < width; ++x) {
                std::string color;
                color += line[x];
                color += line[x];
                color += line[x];
                row.push_back(color_map[color]);
            }
            image_data.push_back(row);
        }
        Image* image = new Image(width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image->at(x, y) = image_data[y][x];
            }
        }
        return image;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        std::ofstream out(file);
        if (!out.is_open()) {
            return;
        }
        out << "/* XPM2 */" << std::endl;
        out << image->width() << " " << image->height() << " 1 1" << std::endl;
        out << "c 0 0 0" << std::endl;
        out << "0" << std::endl;
        for (int y = 0; y < image->height(); ++y) {
            for (int x = 0; x < image->width(); ++x) {
                out << "0";
            }
            out << std::endl;
        }
    }
}
