#ifndef EXR_HPP
#define EXR_HPP

#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <iostream>

int write_exr(Imf::Array2D<Imf::Rgba>& pixels, int width, int height) {
    try {
        Imf::RgbaOutputFile file("hello.exr", width, height, Imf::WRITE_RGBA);
        file.setFrameBuffer(&pixels[0][0], 1, width);
        file.writePixels(height);
    } catch(const std::exception &e) {
        std::cerr << "error writing EXR file hello.exr:" << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#endif
