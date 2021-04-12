#ifndef BITMAP_H
#define BITMAP_H

#include "types.h"
#include <string>
#include <vector>

#define H_FILE_SIZE 14
#define H_INFO_SIZE 40

#pragma pack(push, 1)
struct file_header {
    uint16_t signature {0x4d42};
    uint32_t size {H_FILE_SIZE + H_INFO_SIZE};
    uint32_t reserved {0};
    uint32_t offset {H_FILE_SIZE + H_INFO_SIZE};
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

class Bitmap {
    public:
        Bitmap(unsigned width, unsigned height);
        void write_to_file(std::string path);
        void set_pixel(unsigned x, unsigned y, rgb* color);
        void set_pixel(unsigned pos, rgb* color);

    private:
        std::vector<rgb> pixels_data;
        unsigned width, height;
};

#endif
