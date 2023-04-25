#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this->width_ = w;
    this->height_ = h;
    this->data_ = new Color*[h];
    for(int i = 0; i<h; i++){
      this->data_[i] = new Color[w];
      for(int j = 0; j<w; j++){
        this->data_[i][j] = fill;
      }
    }
  }
  Image::~Image()
  {
    for(int i = 0; i<this->height_; i++){
      delete[] this->data_[i];
    }
  } 
  int Image::width() const
  {
    return this->width_;
  }
  int Image::height() const
  {
    return this->height_;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  Color DUMMY_color;

  Color& Image::at(int x, int y)
  {
    return data_[y][x];
  }

  const Color& Image::at(int x, int y) const
  {
    return data_[y][x];
  }
}
