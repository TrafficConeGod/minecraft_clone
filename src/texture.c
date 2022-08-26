#include "texture.h"
#include "mem.h"
#include <stdlib.h>
#include <png.h>

error_t load_png_images_onto_data_stack(size_t num_images, FILE* const files[], image images[]) {
    for (size_t i = 0; i < num_images; i++) {
        png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) { return -1; }

        png_info* info = png_create_info_struct(png);
        if (!info) { return -1; }

        png_init_io(png, files[i]);

        png_read_info(png, info);

        size_t width = png_get_image_width(png, info);
        size_t height = png_get_image_height(png, info);
        size_t color_type = png_get_color_type(png, info);
        size_t bit_depth = png_get_bit_depth(png, info);

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

        size_t num_row_bytes = png_get_rowbytes(png, info);
        size_t image_size = num_row_bytes * height * sizeof(png_byte);

        mem.data_stack -= sizeof(size_t) + image_size;
        png_byte* image_data = mem.data_stack + sizeof(size_t);
        *(size_t*)mem.data_stack = image_size;


        png_byte* row_pointers[height];
        for (size_t y = 0; y < width; y++) {
            row_pointers[y] = image_data + (y * num_row_bytes);
        }

        png_read_image(png, row_pointers);

        png_destroy_read_struct(&png, &info, NULL);

        image img = {
            .width = width,
            .height = height,
            .data = image_data
        };
        images[i] = img;
    }

    return 0;
}

void load_textures(size_t num_textures, const image images[], GLuint textures[]) {
    glGenTextures(num_textures, textures);
    for (size_t i = 0; i < num_textures; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[i].width, images[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i].data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void free_images_from_data_stack(size_t num_images) {
    for (size_t i = 0; i < num_images; i++) {
        size_t image_size = *(size_t*)mem.data_stack;
        mem.data_stack += sizeof(size_t) + image_size;
    }
}