#ifndef PALETTE_H
#define PALETTE_H

#include "types.h"
#include <string>
#include <vector>

/**
 * Defines a color Palette class that supports a useful method for loading
 * different color palettes from a given .map file.
 */
class Palette {
    public:
        void load_from_file(std::string path);
        rgb& operator[](unsigned index);
        Palette();

    private:
        std::vector<rgb> color_palette;
        unsigned size;
};

#endif
