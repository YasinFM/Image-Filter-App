#include "stdio.h"

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct {
    int width;
    int height;
    RGB *data;
} BMPImage;

BMPImage *read_bmp(const char *address) {
    FILE *file = fopen(address, "rb");
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);
    int width = *(int *) &header[18];
    int height = *(int *) &header[22];
    int data_offset = *(int *) &header[10];

    RGB *data = malloc(sizeof(RGB) * width * height);
    fseek(file, data_offset, SEEK_SET);
    int padding = (4 - (width * 3) % 4) % 4;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGB pixel;
            fread(&(pixel.r), sizeof(unsigned char), 1, file);
            fread(&(pixel.g), sizeof(unsigned char), 1, file);
            fread(&(pixel.b), sizeof(unsigned char), 1, file);
            data[y * width + x] = pixel;
        }
        fseek(file, padding, SEEK_CUR);
    }

    BMPImage *image = malloc(sizeof(BMPImage));
    image->width = width;
    image->height = height;
    image->data = data;
    return image;
}

void save_bmp(BMPImage *image, const char *out_address) {
    FILE *out_file = fopen(out_address, "wb");
    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(int *) &header[2] = 54 + 3 * image->width * image->height; // file size
    *(int *) &header[10] = 54; // offset to pixel data
    *(int *) &header[14] = 40; // size of info header
    *(int *) &header[18] = image->width;
    *(int *) &header[22] = image->height;
    *(short *) &header[26] = 1; // number of color planes
    *(short *) &header[28] = 24;
    fwrite(header, sizeof(unsigned char), 54, out_file);
    int padding = (4 - (image->width * 3) % 4) % 4;
    unsigned char padByte = 0;
    fwrite(&padByte, sizeof(unsigned char), padding, out_file);
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGB pixel = image->data[y * image->width + x];
            fwrite(&(pixel.r), sizeof(unsigned char), 1, out_file);
            fwrite(&(pixel.g), sizeof(unsigned char), 1, out_file);
            fwrite(&(pixel.b), sizeof(unsigned char), 1, out_file);
        }
        for (int i = 0; i < padding; i++) {
            fputc(0x00, out_file);
        }
    }
}

void grayscale_bmp(BMPImage *image) {
    int size = image->width * image->height;
    for (int i = 0; i < size; ++i) {
        int avg = (image->data[i].r + image->data[i].g + image->data[i].b) / 3;
        image->data[i].b = avg;
        image->data[i].g = avg;
        image->data[i].r = avg;
    }
}

void red_bmp(BMPImage *image) {
    int size = image->width * image->height;
    for (int i = 0; i < size; ++i) {
        image->data[i].b = 0;
        image->data[i].g = 0;
    }
}

void blue_bmp(BMPImage *image) {
    int size = image->width * image->height;
    for (int i = 0; i < size; ++i) {
        image->data[i].r = 0;
        image->data[i].g = 0;
    }
}

void green_bmp(BMPImage *image) {
    int size = image->width * image->height;
    for (int i = 0; i < size; ++i) {
        image->data[i].r = 0;
        image->data[i].b = 0;
    }
}

void invert_bmp(BMPImage *image) {
    int size = image->width * image->height;
    for (int i = 0; i < size; ++i) {
        image->data[i].r -= 255;
        image->data[i].g -= 255;
        image->data[i].b -= 255;
    }
}

void negative_bmp(BMPImage *image) {
    grayscale_bmp(image);
    invert_bmp(image);
}

void sepia_bmp(BMPImage *image) {
    int size = image->width * image->height, r, g, b;
    for (int i = 0; i < size; ++i) {
        r = (image->data[i].r * 0.393) + (image->data[i].g * 0.769) + (image->data[i].b * 0.189);
        g = (image->data[i].r * 0.349) + (image->data[i].g * 0.686) + (image->data[i].b * 0.168);
        b = (image->data[i].r * 0.272) + (image->data[i].g * 0.534) + (image->data[i].b * 0.131);
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        image->data[i].r = (unsigned char) r;
        image->data[i].g = (unsigned char) g;
        image->data[i].b = (unsigned char) b;
    }
}

void brightness_bmp(BMPImage *image, int bright) {
    int r, g, b, w, h;
    w = image->width;
    h = image->height;
    for (int i = 0; i < w * h; i++) {
        int valuer = image->data[i].r;
        int valueg = image->data[i].g;
        int valueb = image->data[i].b;
        r = valuer + bright;
        g = valueg + bright;
        b = valueb + bright;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        image->data[i].r = (unsigned char) r;
        image->data[i].g = (unsigned char) g;
        image->data[i].b = (unsigned char) b;
    }

}

void darkness_bmp(BMPImage *image, int dark) {
    int r, g, b, w, h;
    w = image->width;
    h = image->height;
    for (int i = 0; i < w * h; i++) {
        int valuer = image->data[i].r;
        int valueg = image->data[i].g;
        int valueb = image->data[i].b;
        r = valuer - dark;
        g = valueg - dark;
        b = valueb - dark;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        image->data[i].r = (unsigned char) r;
        image->data[i].g = (unsigned char) g;
        image->data[i].b = (unsigned char) b;
    }
}

void contrast_bmp(BMPImage *image, float cst) {
    int r, g, b, w, h;
    w = image->width;
    h = image->height;
    for (int i = 0; i < w * h; i++) {
        int valuer = image->data[i].r;
        int valueg = image->data[i].g;
        int valueb = image->data[i].b;
        r = (cst * (valuer - 128)) + 128;
        g = (cst * (valueg - 128)) + 128;
        b = (cst * (valueb - 128)) + 128;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        image->data[i].r = (unsigned char) r;
        image->data[i].g = (unsigned char) g;
        image->data[i].b = (unsigned char) b;
    }
}

float kernelBlur[3][3] = {
        {1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
        {1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
        {1 / 9.0f, 1 / 9.0f, 1 / 9.0f}
};

void blur_bmp(BMPImage *image, BMPImage *out) {
    int width = image->width;
    int height = image->height;
    int sumr, sumb, sumg;
    for (int i = 0; i < width * height; ++i) {
        if (i >= width + 1 && i < width * height - width) {
            sumr += image->data[i].r + image->data[i + 1].r + image->data[i - 1].r +
                    image->data[i + width].r + image->data[i - width].r +
                    image->data[i + 1 + width].r + image->data[i + 1 - width].r +
                    image->data[i - 1 + width].r + image->data[i - 1 - width].r;

            sumg += image->data[i].g + image->data[i + 1].g + image->data[i - 1].g +
                    image->data[i + width].g + image->data[i - width].g +
                    image->data[i + 1 + width].g + image->data[i + 1 - width].g +
                    image->data[i - 1 + width].g + image->data[i - 1 - width].g;

            sumb += image->data[i].b + image->data[i + 1].b + image->data[i - 1].b +
                    image->data[i + width].b + image->data[i - width].b +
                    image->data[i + 1 + width].b + image->data[i + 1 - width].b +
                    image->data[i - 1 + width].b + image->data[i - 1 - width].b;
        } else {
            out->data[i].r = image->data[i].r;
            out->data[i].g = image->data[i].g;
            out->data[i].b = image->data[i].b;
        }
    }
}

void reflect_vertical_bmp(BMPImage *image, BMPImage *out) {
    out->width = image->width;
    out->height = image->height;
    out->data = image->data;
    int x = out->width, y = out->height;
    int temp = 0;
    for (int j = 0; j < y / 2; j++) {
        for (int i = 0; i < x; i++) {

            temp = out->data[j * x + i].r;
            out->data[j * x + i].r = out->data[((y - j - 1) * x + i)].r;
            out->data[((y - j - 1) * x + i)].r = temp;

            temp = out->data[j * x + i].g;
            out->data[j * x + i].g = out->data[((y - j - 1) * x + i)].g;
            out->data[((y - j - 1) * x + i)].g = temp;

            temp = out->data[j * x + i].b;
            out->data[j * x + i].b = out->data[((y - j - 1) * x + i)].b;
            out->data[((y - j - 1) * x + i)].b = temp;
        }
    }
}

void reflect_horizontal_bmp(BMPImage *image, BMPImage *out) {
    out->width = image->width;
    out->height = image->height;
    out->data = image->data;
    int x = out->width, y = out->height;
    int temp = 0;
    for (int i = 0; i < x / 2; i++) {
        for (int j = 0; j < y; j++) {

            temp = out->data[j * x + i].r;
            out->data[j * x + i].r = out->data[(j * x + (x - i - 1))].r;
            out->data[((y - j - 1) * x + i)].r = temp;

            temp = out->data[j * x + i].g;
            out->data[j * x + i].g = out->data[(j * x + (x - i - 1))].g;
            out->data[((y - j - 1) * x + i)].g = temp;

            temp = out->data[j * x + i].b;
            out->data[j * x + i].b = out->data[(j * x + (x - i - 1))].b;
            out->data[((y - j - 1) * x + i)].b = temp;
        }
    }
}

void rotate_bmp(BMPImage *image, BMPImage *out) {
    int height = image->height;
    int width = image->width;
    out->data = malloc(sizeof(RGB) * width * height);
    out->width = height;
    out->height = width;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            out->data[(width - i - 1) * height + j].r = image->data[j * width + i].r;
            out->data[(width - i - 1) * height + j].g = image->data[j * width + i].g;
            out->data[(width - i - 1) * height + j].b = image->data[j * width + i].b;
        }
    }
}

int bmp(const char *path_in, const char *path_out, int filter) {
    BMPImage *image, *out;
    image = read_bmp(path_in);
    out = read_bmp(path_out);
    if (filter == 1) {
        grayscale_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 2) {
        invert_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 3) {
        sepia_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 4) {
        red_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 5) {
        green_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 6) {
        blue_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 7) {
        negative_bmp(image);
        save_bmp(image, path_out);
    } else if (filter == 8) {
        int bright;
        printf("Enter Bright Degree: ");
        scanf("%d", &bright);
        brightness_bmp(image, bright);
        save_bmp(image, path_out);
    } else if (filter == 9) {
        float cst;
        printf("Enter Contrast Degree: ");
        scanf("%f", &cst);
        contrast_bmp(image, cst);
        save_bmp(image, path_out);
    } else if (filter == 10) {
        int dark;
        printf("Enter Dark Degree: ");
        scanf("%d", &dark);
        darkness_bmp(image, dark);
        save_bmp(image, path_out);
    } else if (filter == 11) {
        reflect_vertical_bmp(image, out);
        save_bmp(out, path_out);
    } else if (filter == 12) {
        reflect_horizontal_bmp(image, out);
        save_bmp(out, path_out);
    } else if (filter == 13) {
        rotate_bmp(image, out);
        save_bmp(out, path_out);
    }
}