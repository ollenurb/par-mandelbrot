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

    /* Fill headers infos */
    h_file.size = (H_FILE_SIZE + H_INFO_SIZE) + ((h_info.bpp / 8) * width * height);
    h_file.offset = H_FILE_SIZE + H_INFO_SIZE;
    h_info.width = width;
    h_info.height = height;

    if(img.is_open()) {
        img.write((const char*) &h_file, H_FILE_SIZE);
        img.write((const char*) &h_info, H_INFO_SIZE);
        for(unsigned int i = 0; i < pixels_data.size(); i++) {
            img.write((const char*) &pixels_data[i], 3);
        }
        img.close();
    }
    else {
        cout << "There was an error while creating the file" << endl;
    }
}

void Bitmap::set_pixel(unsigned x, unsigned y, struct pixel* px_value) {
    pixels_data[(width * x) + y] = *px_value;
}


