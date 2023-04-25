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
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    std::getline(file, line); // Read the first line, which should be the header: /* XPM2 */
    if (line != "/* XPM2 */") {
        std::cerr << "Invalid XPM2 header: " << line << std::endl;
        return nullptr;
    }

    int width, height, colors, charsPerPixel;
    file >> width >> height >> colors >> charsPerPixel;

    if (charsPerPixel != 1) {
        std::cerr << "Unsupported number of characters per pixel: " << charsPerPixel << std::endl;
        return nullptr;
    }

    std::map<char, Color> colorMap;
    for (int i = 0; i < colors; ++i) {
        char ch;
        int r, g, b;
        file >> ch >> r >> g >> b;
        colorMap[ch] = Color(r, g, b);
    }

    file.get(); // Read the newline character after the color definitions

    Image* img = new Image(width, height, Color(0, 0, 0));

    for (int y = 0; y < height; ++y) {
        std::getline(file, line);
        for (int x = 0; x < width; ++x) {
            char ch = line[x];
            if (colorMap.find(ch) != colorMap.end()) {
                img->at(x, y) = colorMap[ch];
            } else {
                std::cerr << "Unknown color character: " << ch << std::endl;
            }
        }
    }

    file.close();
    return img;
}

    void saveToXPM2(const std::string& file, const Image* image) {
        if (!image) {
            return;
        }

        std::ofstream filename(file);
        if (!filename.is_open()) {
            return;
        }

        int width = image->width();
        int height = image->height();

        filename << "/* XPM2 */" << std::endl;
        filename << width << " " << height << " 256 1" << std::endl;

        // Generate the color definitions
        for (int i = 0; i < 256; ++i) {
            int r = i, g = i, b = i;
            filename << static_cast<char>(i) << " " << r << " " << g << " " << b << std::endl;
        }

        // Write the image data
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Color c = image->at(x, y);
                int r = c.red();
                int g = c.green();
                int b = c.blue();

                // Find the closest grayscale color to the current pixel color
                int minDist = 256 * 256 * 3;
                char bestChar = 0;

                for (int i = 0; i < 256; ++i) {
                    int dr = r - i;
                    int dg = g - i;
                    int db = b - i;

                    int dist = dr * dr + dg * dg + db * db;
                    if (dist < minDist) {
                        minDist = dist;
                        bestChar = static_cast<char>(i);
                    }
                }

                filename << bestChar;
            }
            filename << std::endl;
        }

        filename.close();
    }
}
