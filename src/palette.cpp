#include "palette.h"
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

Palette::Palette() {
    size = 0;
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

rgb& Palette::operator[](unsigned index) {
    if(index < size) {
        return color_palette[index];
    } else {
        throw out_of_range("Index out of range error");
    }
}
