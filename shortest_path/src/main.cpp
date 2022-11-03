#include <iostream>

#include "map/map.h"

void decode_from_png(std::vector<unsigned char>& image, const char* fname,
                     unsigned height, unsigned width) {
  std::vector<unsigned char> png;
  unsigned error = lodepng::load_file(png, fname);
  if (!error) {
    error = lodepng::decode(image, width, height, png);
  }
  if (error) {
    std::cout << "decoder error " << error << ": ";
    std::cout << lodepng_error_text(error) << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 7) {
    std::cerr << "Wrong input arguments" << std::endl;
    return -1;
  }
  int rows = atoi(argv[2]), cols = atoi(argv[3]);
  int x_i = atoi(argv[4]), x_f = atoi(argv[5]);
  char *input_file = argv[1], *output_file = argv[6];

  Map M;
  {
    std::vector<unsigned char> image;
    decode_from_png(image, input_file, rows, cols);  // read png file
    M.from_image(image, rows, cols);
    find_path(M, x_i, x_f);
    draw_path(image, M, output_file, x_i, x_f);
  }
  // {
  //   std::vector<unsigned char> image;
  //   decode_from_png(image, "../black.png", rows, cols);
  //   draw_map(image, M, "../map.png");
  // }
}
