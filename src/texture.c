#include "texture.h"
#include "int_types.h"
#include <stdlib.h>
#include <png.h>

error_t load_png_textures(size_t num_textures, FILE* const files[], GLuint textures[]) {
    for (size_t i = 0; i < num_textures; i++) {
        png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) { return -1; }

        png_info* info = png_create_info_struct(png);
        if (!info) { return -1; }

        if (setjmp(png_jmpbuf(png))) { return -1; }

        png_init_io(png, files[i]);

        png_read_info(png, info);

        u32 width = png_get_image_width(png, info);
        u32 height = png_get_image_height(png, info);
        u32 color_type = png_get_color_type(png, info);
        u32 bit_depth = png_get_bit_depth(png, info);

        if (bit_depth == 16) {
            png_set_strip_16(png);
        }

        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png);
        }
        
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png);
        }

        if (png_get_valid(png, info, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png);
        }

        if (
            color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE
        ) {
            png_set_filler(png, 0xff, PNG_FILLER_AFTER);
        }

        if (
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA
        ) {
            png_set_gray_to_rgb(png);
        }

        png_read_update_info(png, info);

        
        for (size_t i = 0; i < height; i++) {

        }

        png_destroy_read_struct(&png, &info, NULL);
    }

    return 0;
}