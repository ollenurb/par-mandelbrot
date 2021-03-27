#include "bitmap.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Bitmap::Bitmap(unsigned width, unsigned height) {
    this->width = width;
    this->height = height;
    this->pixels_data.resize(width * height);
}

void Bitmap::write_to_file(string path) {
    ofstream img(path, ios_base::out | ios_base::binary);
    struct file_header h_file;
    struct info_header h_info;


    uint32_t padding = 0;
    uint8_t pad_sz = (4 - (width * (h_info.bpp / 8)) % 4) % 4;

    /* Fill headers infos */
    h_file.size += ((h_info.bpp / 8) * width * height) + height * pad_sz;
    h_info.width = width;
    h_info.height = height;

    if(img.is_open()) {
        img.write((const char*) &h_file, H_FILE_SIZE);
        img.write((const char*) &h_info, H_INFO_SIZE);
        for(uint32_t j = 0; j < height; j++) {
            for(uint32_t i = 0; i < width; i++) {
                img.write((const char*) &pixels_data[(width * j) + i], sizeof(rgb));
            }
            img.write((const char*) &padding, pad_sz);

        }
        img.close();
    }
    else {
        cout << "There was an error while creating the file" << endl;
    }
}

void Bitmap::set_pixel(unsigned x, unsigned y, rgb* color) {
    pixels_data[(width * y) + x] = *color;
}

void Bitmap::set_pixel(unsigned pos, rgb* color) {
    pixels_data[pos] = *color;
}

