
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265

#pragma pack(push, 1)

typedef struct BitmapFileHeader{
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct BitmapInfoHeader{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader; 

typedef struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} rgb;

typedef struct BMPFile {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    rgb** data;
} BMPFile;

typedef struct coordinates{
    int x, y;
} coords;

typedef struct rect{
    coords start;
    coords end;
    unsigned int thickness;
    rgb color;
    unsigned char filled;
    rgb fill_color;
} rectangle;

typedef struct hex{
    //coords start, sec, thrd, fourth, fifth, end;
    unsigned int thickness;
    rgb color;
    unsigned char filled;
    rgb fill_color;
    coords centre;
    unsigned int radius;
} hexagon;

#pragma pack(pop)

BMPFile* openBMPFile(char* filename) {
    FILE *filepath = fopen(filename, "rb");

    if (!filepath) {
        wprintf(L"Ошибка чтения файла \'%s\'\n", filename);
        exit(0);
    }

    BMPFile *img = (BMPFile *) malloc(sizeof(BMPFile));
    fread(&img->bmfh, sizeof(BitmapFileHeader), 1, filepath);
    fread(&img->bmih, sizeof(BitmapInfoHeader), 1, filepath);

    fclose(filepath);
    return img;
}

void readRowsBMPFile(FILE* filepath, BMPFile* img) {
    int written = 0;
    unsigned int row_size = img->bmih.width * 3;
    unsigned char row_padding = (4 - (row_size % 4)) % 4;

    unsigned char* row = (unsigned char*) malloc(row_size + row_padding);
    fseek(filepath, img->bmfh.pixelArrOffset, SEEK_SET);

    for(int y = img->bmih.height - 1; y > -1; y--){
        fread(row, row_size + row_padding, 1, filepath);
        for(int x = 0; x < img->bmih.width; x++){
            img->data[y][x].blue = row[x*3];
            img->data[y][x].green = row[x*3 + 1];
            img->data[y][x].red = row[x*3 + 2];
        }
    }
    free(row);
}

BMPFile* readBMPFile(char* filename) {
    BMPFile *img = openBMPFile(filename);

    /*if(img->bmih.compression != 0 || img->bmih.colorsInColorTable != 24 || img->bmfh.signature != 0x4D42){
        wprintf(L"mypaint: неподдерживаемая версия BMP-файла.\nПо команде «--help» можно получить дополнительную информацию.\n");
        exit(0);
    }*/

    FILE *filepath = fopen(filename, "rb");
    img->data = (rgb**) malloc(img->bmih.height * sizeof(rgb*));
    for (int i = 0; i < img->bmih.height; i++)
        img->data[i] = (rgb*) malloc(img->bmih.width * sizeof(rgb));
    readRowsBMPFile(filepath, img);

    fclose(filepath);
    return img;
}

void writeBMPFile(char* filename, BMPFile* img) {
    FILE* filepath = fopen(filename, "wb");

    if (!filepath){
        wprintf(L"Ошибка чтения файла \'%s\'\n", filename);
        exit(0);
    }

    fwrite(&img->bmfh, sizeof(BitmapFileHeader), 1, filepath);
    fwrite(&img->bmih, sizeof(BitmapInfoHeader), 1, filepath);

    unsigned int row_size = img->bmih.width * 3;
    unsigned char row_padding = (4 - (row_size % 4)) % 4;

    unsigned char* row = (unsigned char*) malloc(row_size + row_padding);
    fseek(filepath, img->bmfh.pixelArrOffset, SEEK_SET);
    for (int y = img->bmih.height - 1; y > -1; y--){
        for(int x = 0; x < img->bmih.width; x++){
            row[x*3] = img->data[y][x].blue;
            row[x*3 + 1] = img->data[y][x].green;
            row[x*3 + 2] = img->data[y][x].red;
        }
        fwrite(row, row_size + row_padding, 1, filepath);
    }
    free(row);
}

void print_file_header(BitmapFileHeader header){
    printf("signature:\t%x (%hu)\n", header.signature, header.signature);
    printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
    printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
    printf("reserved2:s\t%x (%hu)\n", header.reserved2, header.reserved2);
    printf("pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
} 

void print_info_header(BitmapInfoHeader header){
    printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
    printf("width: \t%x (%u)\n", header.width, header.width); 
    printf("height: \t%x (%u)\n", header.height, header.height);
    printf("planes: \t%x (%hu)\n", header.planes,header.planes);
    printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
    printf("compression:\t%x (%u)\n", header.compression, header.compression);
    printf("imageSize:\t%x (%u)\n", header.imageSize,header.imageSize);
    printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
    printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
    printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
    printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
} 
void freeBMPFile(BMPFile* img) {
    if(img){
        if (img->data){
            for (int y = 0; y < img->bmih.height; y++)
                free(img->data[y]);
            free(img->data);
        }
        free(img);
    }
}

int is_bmp(char* arg){
    int length = strlen(arg);
    if (length < 4) return 0;
    char* extension = arg + length - 4;
    return strcmp(extension, ".bmp") == 0;
}
//1 func
void drawingLine(coords start, coords end, rgb color, unsigned int thickness, BMPFile* img) {
    int dx = end.x - start.x;
    int absDx = abs(dx);
    int dy = end.y - start.y;
    int absDy = abs(dy);

    int x, y;
    int err = 0;

    int direction;

    if (abs(dx) > abs(dy)){
        y = start.y;
        direction = (dy > 0) ? 1 : -1;
        for (x = start.x; (dx > 0) ? x <= end.x : x >= end.x; (dx > 0) ? x++ : x--){

            for (int j = 0; j < thickness; j++) {
                for (int i = 0; i < thickness; i++) {
                    if (y+j >= 0 && x+i >= 0 && x + i < img->bmih.width && y + j < img->bmih.height) {
                        img->data[y+j][x+i] = color;
                    }
                }
            }

            err += absDy;

            if (err >= absDx) {
                err -= absDx;
                y += direction;
            }
        }
    } else {
        x = start.x;
        direction = (dx > 0) ? 1 : -1;
        for (y = start.y; (dy > 0) ? y <= end.y : y >= end.x; (dy > 0) ? y++ : y--){

            for (int j = 0; j < thickness; j++) 
                for (int i = 0; i < thickness; i++) {
                    if (y+j >= 0 && x+i >= 0 && x + i < img->bmih.width && y + j < img->bmih.height) {
                        img->data[y+j][x+i] = color;
                    }
                }
            }

            err += absDx;

            if (err >= absDy) {
                err -= absDy;
                x += direction;
            }
        }
    }



void drawingRectangle(rectangle data, BMPFile* img) {
    /*if (data.start.x + data.length < 0 || data.start.y + data.length < 0 || data.start.x > img->bmih.width || data.start.y > img->bmih.height) {
        wprintf(L"mypaint: некорректные координаты левого верхнего угла квадрата.\nПо команде «--help» можно получить дополнительную информацию.\n");
        return;
    }img*/

    coords left_top = data.start;
    coords right_top =  {data.start.x, data.end.y};
    coords left_bottom = {data.end.x, data.start.y};
    coords right_bottom = data.end;

    drawingLine(left_top, right_top, data.color, data.thickness, img);
    drawingLine(left_top, left_bottom, data.color, data.thickness, img);
    drawingLine(right_top, right_bottom, data.color, data.thickness, img);
    drawingLine(left_bottom, right_bottom, data.color, data.thickness, img);

    for(int i = data.start.y+data.thickness; i < data.end.y; i++){
        for(int j = data.start.x+data.thickness; j < data.end.x; j++){
            img->data[i][j].blue = data.fill_color.blue;
            img->data[i][j].green = data.fill_color.green;
            img->data[i][j].red = data.fill_color.red;
            }
    }
}

void drawingHexagon(hexagon data, BMPFile* img){
    
    coords l1={data.centre.x-(data.radius/2), data.centre.y+data.radius*pow(3, 0.5)/2};

    coords l2={data.centre.x+(data.radius/2), data.centre.y+data.radius*pow(3, 0.5)/2};

    coords l3={data.centre.x+data.radius, data.centre.y};

    coords l4={data.centre.x+(data.radius/2), data.centre.y-data.radius*pow(3, 0.5)/2};

    coords l5={data.centre.x-(data.radius/2), data.centre.y-data.radius*pow(3, 0.5)/2};

    coords l6={data.centre.x-data.radius, data.centre.y};

    drawingLine(l1, l2, data.color, data.thickness, img);
    drawingLine(l2, l3, data.color, data.thickness, img);
    drawingLine(l3, l4, data.color, data.thickness, img);
    drawingLine(l4, l5, data.color, data.thickness, img);
    drawingLine(l5, l6, data.color, data.thickness, img);
    drawingLine(l6, l1, data.color, data.thickness, img);

}
int main()
{
    rectangle rect;
    rect.color.red=255;
    rect.color.blue=0;
    rect.color.green=0;
    rect.fill_color.blue=255;
    rect.fill_color.green=0;
    rect.fill_color.red=0;
    rect.filled=0;
    rect.thickness=5;
    rect.start.x=1;
    rect.start.y=1;
    rect.end.x=200;
    rect.end.y=100;

    hexagon hex;
    hex.centre.x=250;
    hex.centre.y=250;
    hex.color.red=255;
    hex.color.blue=0;
    hex.color.green=0;
    hex.filled=0;
    hex.thickness=5;
    hex.radius=200;
    hex.fill_color.red=0;
    hex.fill_color.blue=255;
    hex.fill_color.green=0;
    BMPFile* img = readBMPFile("Airplane.bmp");
    //drawingRectangle(rect, img);
    drawingHexagon(hex, img);
    writeBMPFile("fill.bmp", img);
}
/*    coords l1={data.centre.x-(data.radius/2), data.centre.y+pow(data.radius, 0.5)/2};

    coords l2={data.centre.x+(data.radius/2), data.centre.y+pow(data.radius, 0.5)/2};

    coords l3={data.centre.x+data.radius, data.centre.y};

    coords l4={data.centre.x+(data.radius/2), data.centre.y-pow(data.radius, 0.5)/2};

    coords l5={data.centre.x-(data.radius/2), data.centre.y-pow(data.radius, 0.5)/2};

    coords l6={data.centre.x-data.radius, data.centre.y};
    
    
    
    
     coords l1={data.centre.x+data.radius, data.centre.y};

    coords l2={data.centre.x+data.radius*cos(PI/3), data.centre.y+data.radius*sin(PI/3)};

    coords l3={data.centre.x+data.radius*cos(2*PI/3), data.centre.y+data.radius*sin(2*PI/3)};

    coords l4={data.centre.x-data.radius, data.centre.y};

    coords l5={data.centre.x+data.radius*cos(4*PI/3), data.centre.y+data.radius*sin(4*PI/3)};

    coords l6={data.centre.x+data.radius*cos(5*PI/3), data.centre.y+data.radius*sin(5*PI/3)};
    */
