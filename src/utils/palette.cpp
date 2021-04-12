#include "palette.h"
#include <fstream>

using namespace std;

Palette::Palette(unsigned max_iters) {
    size = 0;
    this->max_iters = max_iters;
}

void Palette::load_from_file(string path) {
    ifstream in_file(path);
    int r, g, b;

    while(in_file >> r >> g >> b) {
        rgb color;
        color.red = r;
        color.green = g;
        color.blue = b;
        color_palette.push_back(color);
        size += 1;
    }
}

int Palette::get_size() {
    return size;
}

rgb& Palette::operator[](unsigned index) {
    // Round down the interval
    index = static_cast<unsigned>((size * index) / max_iters);
    return color_palette[index];
}
