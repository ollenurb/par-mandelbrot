#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>

#define H_FILE_SIZE 14
#define H_INFO_SIZE 40

#pragma pack(push, 1)
struct file_header {
    uint16_t signature {0x4d42};
    uint32_t size;
    uint32_t reserved {0};
    uint32_t offset;
};
#pragma pack(pop)

struct info_header {
    uint32_t size {H_INFO_SIZE};
    uint32_t width {0};
    uint32_t height {0};
    uint16_t color_planes {1};
    uint16_t bpp {24};
    uint32_t compression {0};
    uint32_t image_sz {0};
    uint32_t x_ppm {0};
    uint32_t y_ppm {0};
    uint32_t tot_colors {0};
    uint32_t imp_colors {0};
};

struct pixel {
    uint8_t blue = 255;
    uint8_t green = 0;
    uint8_t red = 255;
};

class Bitmap {
    public:
        Bitmap(unsigned width, unsigned height);
        void write_to_file(std::string path);
        void set_pixel(unsigned x, unsigned y, struct pixel* px_value);

    private:
        std::vector<struct pixel> pixels_data;
        unsigned width, height;
};

#endif
