// photoeditorcproject@gmail.com
// xtgcggxpnrmtuulf

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "C:\project\stb\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\project\stb\stb_image_write.h"
#include <string.h>
#include "curl/curl.h"
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <conio.h>
//#include "bmp.h"

#define namelen 20

int ui(){
    char c;
    int uid = 0;
    FILE *file = fopen("C:\\project\\fetch\\uid.txt","r");
    while (1){
        if (feof(file)){
            break;
        }
        c = fgetc(file);
        if ( c>='0' && c<='9' ){
            uid = (uid*10) + (c-'0');
        }
    }
    rewind(file);
    fclose(file);
    file = fopen("C:\\project\\fetch\\uid.txt","w");
    int i = uid + 1;
    fprintf(file,"%d",i);
    fclose(file);
    return uid;
}

int uim(){
    char c;
    int uid = 0;
    FILE *file = fopen("C:\\project\\fetch\\uid.txt","r");
    while (1){
        if (feof(file)){
            break;
        }
        c = fgetc(file);
        if ( c>='0' && c<='9' ){
            uid = (uid*10) + (c-'0');
        }
    }
    rewind(file);
    fclose(file);
    file = fopen("C:\\project\\fetch\\uid.txt","w");
    int i = uid - 1;
    fprintf(file,"%d",i);
    fclose(file);
    return 0;
}

int b64decode(char input[250],char name[10]) {
    int i = 0;
    char c;
    char ch[15];
    FILE *fs;
    fs = fopen(input, "r");
    for (;i < 10;) {
        c = fgetc(fs);
        ch[i++]=c;
    }
    ch[i++]='\0';
    char command[200],e[10];
    char *found_string1 = strstr(ch, "Qk");
    char *found_string2 = strstr(ch, "/9j");
    if(found_string1 != NULL){
        char e[namelen] = "emailoutput.bmp\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //bmp
    }else if(found_string2 != NULL){
        char e[namelen] = "emailoutput.jpg\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //jpg
    }else{
        char e[namelen] = "emailoutput.png\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //png
    }
    system(command);
    return 0;
}

int fetch(char oldt[200],char newt[200]) {
    FILE *fs, *fn;
    fs = fopen(oldt, "r");
    if(fs == NULL)
    {
        printf("\nError in Opening the first file, %s", oldt);
        getch();
        return 0;
    }
    fn = fopen(newt, "w");
    if(fn == NULL)
    {
        printf("\nError in Opening the second file, %s", newt);
        getch();
        return 0;
    }
    char c;
    char cf[200];
    int flag = 0;
    int i=0;
    while(1){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        if (c != '\n') {
            cf[i++] = c;
        } else {
            cf[i++] = c;
            i = 0;
            char *found_string = strstr(cf, "X-Attachment-Id: ");
            if (found_string == NULL) {
                continue;
            } else {
                flag = 1;
                break;
            }
        }
    }
    while(flag){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        if(c == '-'){
            break;
        }
        else{
            fputc(c,fn);
        }
    }
    fclose(fs);
    fclose(fn);
    printf("fetching file finished\n");
    return 0;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* file = (FILE*)userp;
    size_t written = fwrite(contents, size, nmemb, file);
    return written;
}

int email() {
    CURL* curl;
    CURLcode res;
    FILE* file;
    // Set up libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        int flag = 0;
        // Set IMAP server and account details
        char email[] = "photoeditorcproject@gmail.com"; // Replace with your Gmail address
        char password[] = "xtgcggxpnrmtuulf"; // Replace with your Gmail password
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\project\\cacert-2023-05-30.pem");

        // Set the mailbox name and UID
        char mailbox[] = "INBOX";
        int uid = ui();

        // Create the URL with the UID
        char url[100];
        sprintf(url, "imaps://imap.gmail.com/%s;UID=%d", mailbox, uid);

        // Connect to the IMAP server and log in
        curl_easy_setopt(curl, CURLOPT_USERNAME, email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the fetch request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            flag = 1;
        }

        // Open a file to save the fetched email
        file = fopen("C:\\project\\fetch\\image.txt", "wb");
        if (!file) {
            printf("Error opening file for writing.");
            return 1;
        }

        // Set the write callback function to write data into the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the fetch request and save the email content to the file
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            flag = 1;
        }

        if(flag){
            uim();
            return 1;
        }

        // Clean up and close the file
        fclose(file);

        // Clean up and close the IMAP session
        curl_easy_cleanup(curl);
    } else {
        printf("Error initializing libcurl.\n");
        return 1;
    }

    // Clean up libcurl
    curl_global_cleanup();
    fetch("C:\\project\\fetch\\image.txt","C:\\project\\fetch\\fetch.txt");
    b64decode("C:\\project\\fetch\\fetch.txt","C:\\project\\fetch\\fetch.txt");

    return 0;
}

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

void grayscale(int w, int h, int ch, unsigned char *inp) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = inp[pixel_index + 0];
            int valueg = inp[pixel_index + 1];
            int valueb = inp[pixel_index + 2];
            r = (valuer + valueg + valueb) / 3;
            g = (valuer + valueg + valueb) / 3;
            b = (valuer + valueg + valueb) / 3;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            inp[pixel_index + 0] = (unsigned char) r;
            inp[pixel_index + 1] = (unsigned char) g;
            inp[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void invert(int w, int h, int ch, unsigned char *inp) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = inp[pixel_index + 0];
            int valueg = inp[pixel_index + 1];
            int valueb = inp[pixel_index + 2];
            r = 255 - valuer;
            g = 255 - valueg;
            b = 255 - valueb;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            inp[pixel_index + 0] = (unsigned char) r;
            inp[pixel_index + 1] = (unsigned char) g;
            inp[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void blur(int w, int h, int ch, const unsigned char *inp, unsigned char *outp) {
    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            float r = 0.0f, g = 0.0f, b = 0.0f;
            int k_i = 0;
            for (int j = -1; j <= 1; j++) {
                int k_j = 0;
                for (int i = -1; i <= 1; i++) {
                    int idx = (y + j) * w * ch + (x + i) * ch;
                    r += inp[idx] * kernelBlur[k_j][k_i];
                    g += inp[idx + 1] * kernelBlur[k_j][k_i];
                    b += inp[idx + 2] * kernelBlur[k_j][k_i];
                    k_j++;
                }
                k_i++;
            }
            int idx = y * w * ch + x * ch;
            outp[idx] = (unsigned char) r;
            outp[idx + 1] = (unsigned char) g;
            outp[idx + 2] = (unsigned char) b;
        }
    }
}

void sepia(int width, int height, int channels, unsigned char *image_data) {
    int r, g, b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate pixel index
            int pixel_index = (y * width + x) * channels;
            // Add random noise to each channel
            int valuer = image_data[pixel_index + 0];
            int valueg = image_data[pixel_index + 1];
            int valueb = image_data[pixel_index + 2];
            r = (valuer * 0.393) + (valueg * 0.769) + (valueb * 0.189);
            g = (valuer * 0.349) + (valueg * 0.686) + (valueb * 0.168);
            b = (valuer * 0.272) + (valueg * 0.534) + (valueb * 0.131);
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            image_data[pixel_index + 0] = (unsigned char) r;
            image_data[pixel_index + 1] = (unsigned char) g;
            image_data[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void red(unsigned char *image, int w, int h, int ch) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = image[pixel_index + 0];
            image[pixel_index + 0] = (unsigned char) valuer;
            image[pixel_index + 1] = (unsigned char) 0;
            image[pixel_index + 2] = (unsigned char) 0;
        }
    }
}

void green(unsigned char *image, int w, int h, int ch) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valueg = image[pixel_index + 1];
            image[pixel_index + 0] = (unsigned char) 0;
            image[pixel_index + 1] = (unsigned char) valueg;
            image[pixel_index + 2] = (unsigned char) 0;
        }
    }
}

void blue(unsigned char *image, int w, int h, int ch) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valueb = image[pixel_index + 2];
            image[pixel_index + 0] = (unsigned char) 0;
            image[pixel_index + 1] = (unsigned char) 0;
            image[pixel_index + 2] = (unsigned char) valueb;
        }
    }
}

void negative(unsigned char *image, int w, int h, int ch) {
    grayscale(w, h, ch, image);
    invert(w, h, ch, image);
}

void brightness(unsigned char *image, int w, int h, int ch, int bright) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = image[pixel_index + 0];
            int valueg = image[pixel_index + 1];
            int valueb = image[pixel_index + 2];
            r = valuer + bright;
            g = valueg + bright;
            b = valueb + bright;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            image[pixel_index + 0] = (unsigned char) r;
            image[pixel_index + 1] = (unsigned char) g;
            image[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void contrast(unsigned char *image, int w, int h, int ch, float cst) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = image[pixel_index + 0];
            int valueg = image[pixel_index + 1];
            int valueb = image[pixel_index + 2];
            r = (cst * (valuer - 128)) + 128;
            g = (cst * (valueg - 128)) + 128;
            b = (cst * (valueb - 128)) + 128;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            image[pixel_index + 0] = (unsigned char) r;
            image[pixel_index + 1] = (unsigned char) g;
            image[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void darkness(unsigned char *image, int w, int h, int ch, int dark) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = image[pixel_index + 0];
            int valueg = image[pixel_index + 1];
            int valueb = image[pixel_index + 2];
            r = valuer - dark;
            g = valueg - dark;
            b = valueb - dark;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            image[pixel_index + 0] = (unsigned char) r;
            image[pixel_index + 1] = (unsigned char) g;
            image[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void grain(unsigned char *image, int width, int height) {
    for (int i = 0; i < width * height * 3; i += 3) {
        // Generate random noise for each color channel
        unsigned char noiseR = rand() % 100;
        unsigned char noiseG = rand() % 100;
        unsigned char noiseB = rand() % 100;

        // Add the noise to the pixel
        image[i] = (unsigned char) (image[i] + noiseR) / 2;
        image[i + 1] = (unsigned char) (image[i + 1] + noiseG) / 2;
        image[i + 2] = (unsigned char) (image[i + 2] + noiseB) / 2;
    }
}

int str_len(const char *str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) len++;
    return len;
}

int isimage(const char *path) {
    int len = str_len(path), f = 0;
    char format[4];
    for (int i = len - 1; path[i] != '.'; --i) format[f++] = path[i];
    if (strcmp(format, "gpj") != 0 || strcmp(format, "gnp") != 0 || strcmp(format, "pmb") != 0 ||
        strcmp(format, "fifj") != 0)
        return 1;
    return 0;
}

void copy(const unsigned char *image, int w, int h, int ch, unsigned char *out) {
    int r, g, b;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Calculate pixel index
            int pixel_index = (y * w + x) * ch;
            // Add random noise to each channel
            int valuer = image[pixel_index + 0];
            int valueg = image[pixel_index + 1];
            int valueb = image[pixel_index + 2];
            r = valuer;
            g = valueg;
            b = valueb;
            out[pixel_index + 0] = (unsigned char) r;
            out[pixel_index + 1] = (unsigned char) g;
            out[pixel_index + 2] = (unsigned char) b;
        }
    }
}

void reflect(const unsigned char *image, unsigned char *out, int width, int height) {
    int x = width, y = height;
    copy(image, width, height, 3, out);
    int temp = 0;
    for (int j = 0; j < y / 2; j++) {
        for (int i = 0; i < x; i++) {
            temp = out[j * x + i];
            out[j * x + i] = out[((y - j - 1) * x + i)];
            out[((y - j - 1) * x + i)] = temp;

            temp = out[j * x + i + 1];
            out[j * x + i + 1] = out[((y - j - 1) * x + i) + 1];
            out[((y - j - 1) * x + i) + 1] = temp;

            temp = out[j * x + i + 2];
            out[j * x + i + 2] = out[((y - j - 1) * x + i) + 2];
            out[((y - j - 1) * x + i) + 2] = temp;
        }
    }
}

char *find_name(const char *url) {
    int len = str_len(url), n = 1, start;
    char *name = malloc(100);
    name[0] = '\\';
    for (int i = len - 1; i > 0; --i) {
        if (url[i] == '/') {
            start = i + 1;
            break;
        }
    }

    int j;
    for (j = start; j < len; ++j) name[n++] = url[j];
    name[n] = '\0';

    return name;
}

void reflect_horizontal(const unsigned char *image, unsigned char *out, int width, int height, int channels) {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            out[(j * width + i) * channels] = image[(j*width+(width-1-i))*channels];
            out[(j * width + i) * channels + 1] = image[(j*width+(width-1-i))*channels + 1];
            out[(j * width + i) * channels + 2] = image[(j*width+(width-1-i))*channels + 2];
        }
    }
}

void reflect_vertical(const unsigned char *image, unsigned char *out, int width, int height, int channels) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            out[(i * width + j) * channels] = image[((height - i - 1) * width + j) * channels];
            out[(i * width + j) * channels + 1] = image[((height - i - 1) * width + j) * channels + 1];
            out[(i * width + j) * channels + 2] = image[((height - i - 1) * width + j) * channels + 2];
        }
    }
}

// Main program
int main() {
    unsigned char *image, *out;
    char *filename_in = calloc(100, sizeof(char)), filename_out[100];
    int width, height, channels;

    int way = 0;
    printf("1. Path\n");
    printf("2. URL\n");
    printf("3. Email\n");
    while (way != 1 && way != 2 && way != 3) {
        scanf("%d", &way);
        if (way < 1 || way > 3) printf("Choice a way: ");
    }

    printf("1. GrayScale\n");
    printf("2. Invert\n");
    printf("3. Sepia\n");
    printf("4. Red\n");
    printf("5. Green\n");
    printf("6. Blue\n");
    printf("7. Negative\n");
    printf("8. Brightness\n");
    printf("9. Contrast\n");
    printf("10. Darkness\n");
    printf("11. Reflect Horizontal\n");
    printf("12. Reflect Vertical\n");

    int filter = 0;
    while (!(filter > 0 && filter < 14)) {
        scanf("%d", &filter);
        if (!(filter > 0 && filter < 14)) printf("Enter a filter: ");
    }

    printf("\n1. jpg\n");
    printf("2. png\n");
    printf("3. bmp\n");
    printf("4. jfif\n");
    int format = 0;
    while (!(format > 0 && format < 5)) {
        scanf("%d", &format);
        if (!(format > 0 && format < 5)) printf("Enter a format");
    }

    if (way == 1) {
        printf("Enter your image path: ");
        scanf("%s", filename_in);
        if (isimage(filename_in)) {
            printf("\nEnter your final image path: ");
            scanf("%s", filename_out);
        } else {
            while (!(isimage(filename_in))) {
                scanf("%s", filename_in);
                if (!(isimage(filename_in)))
                    printf("Enter image path: ");
            }
        }
        if (format != 3) {
            image = stbi_load(filename_in, &width, &height, &channels, 0);
            out = malloc(width * height * channels);
        } else {
            bmp(filename_in, filename_out, filter);
            return 0;
        }
    } else if (way == 2) {
        char *url = calloc(1000, sizeof(char));
        char *path = calloc(1000, sizeof(char));
        char *type = calloc(1000, sizeof(char));
        char *filename = calloc(1000, sizeof(char));
        // https://sample-videos.com/img/Sample-png-image-5mb.png
        // C:\Users\acer\Desktop\name
        printf("Enter your image url: ");
        scanf("%s", url);
        printf("Enter your final image path: ");
        scanf("%s", path);
        filename = find_name(url);

        if (format == 1) type = "jpg";
        else if (format == 2) type = "png";
        else if (format == 3) type = "bmp";
        else if (format == 4) type = "jfif";
        // wget -nd -r -P Desktop -A png https://sample-videos.com/img/Sample-png-image-200kb.png
        char command[1000] = "wget -nd -r -P ";
        strcat(command, path);
        strcat(command, " -A ");
        strcat(command, type);
        strcat(command, " ");
        strcat(command, url);
        printf("%s", command);
        system(command);
        strcat(path, filename);
        printf("Filtered image: ");
        scanf("%s", filename_out);
        if (strcmp(type, "png") != 0 || strcmp(type, "jpg") != 0)
            image = stbi_load(path, &width, &height, &channels, 0);
        else if (strcmp(type, "bmp") != 0) bmp(filename_in, filename_out, filter);
    } else if (way == 3) {
        int em = email();
        if(em == 0) {
            if (format == 3) {
                BMPImage *in;
                in = read_bmp("C:\\project\\cmake-build-debug\\emailoutput.bmp");
            } else if (format == 2) {
                image = stbi_load("C:\\project\\cmake-build-debug\\emailoutput.png", &width, &height, &channels, 0);
            } else {
                image = stbi_load("C:\\project\\cmake-build-debug\\emailoutput.jpg", &width, &height, &channels, 0);
            }
            printf("Filtered image: ");
            scanf("%s", filename_out);
        } else{
            printf("Error!");
        }
    }


    if (filter == 1) {
        grayscale(width, height, channels, image);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 2) {
        invert(width, height, channels, image);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 3) {
        sepia(width, height, channels, image);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 4) {
        red(image, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 5) {
        green(image, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 6) {
        blue(image, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 7) {
        negative(image, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 8) {
        int bright;
        printf("Enter Brightness Degree: ");
        scanf("%d", &bright);
        brightness(image, width, height, channels, bright);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 9) {
        float cst;
        printf("Enter Contrast Degree: ");
        scanf("%f", &cst);
        contrast(image, width, height, channels, cst);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 10) {
        int dark;
        printf("Enter Brightness Degree: ");
        scanf("%d", &dark);
        darkness(image, width, height, channels, dark);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, image, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, image, 0);
    } else if (filter == 11) {
        reflect_horizontal(image, out, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, out, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, out, 0);
    } else if (filter == 12) {
        reflect_vertical(image, out, width, height, channels);
        if (format == 1 || format == 4)
            stbi_write_jpg(filename_out, width, height, channels, out, 100);
        else if (format == 2)
            stbi_write_png(filename_out, width, height, channels, out, 0);
    }

    return 0;
}