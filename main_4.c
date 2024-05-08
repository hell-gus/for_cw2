//рисование линий и вспомогательные функции для заливки от данилы пачева
//напрягают закругленные края у прямоугольника и шестиугольника
//и код надо чутка переделать чтобы не повторять данилин точь в точь
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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
void draw_pixel(BMPFile *img, int x, int y, rgb color) {
    if (!(x < 0 || y < 0 || x >= img->bmih.width || y >= img->bmih.height)) {
        img->data[y][x] = color;
    }
}

void fill_circle(BMPFile *img, int x0, int y0, int r, rgb color) {
    int x = 0;
    int y = r;
    int delta = 3 - 2 * y;
    while (y >= x) {
        draw_pixel(img, x0 + x, y0 + y, color);
        draw_pixel(img, x0 + x, y0 - y, color);
        draw_pixel(img, x0 - x, y0 + y, color);
        draw_pixel(img, x0 - x, y0 - y, color);
        draw_pixel(img, x0 + y, y0 + x, color);
        draw_pixel(img, x0 + y, y0 - x, color);
        draw_pixel(img, x0 - y, y0 + x, color);
        draw_pixel(img, x0 - y, y0 - x, color);
        delta += delta < 0 ? 4 * x + 6 : 4 * (x - y--) + 10;
        ++x;
    }
    for (int y = -r; y <= r; y++) {
        if ((y0+y)<0 || (y0+y)>=img->bmih.height){
            continue;
        }
        for (int x = -r; x <= r; x++) {
            if (((x0+x)>=0) && ((x0+x)<img->bmih.width)&& (x * x + y * y <= r * r)) {
                draw_pixel(img, x0 + x, y0 + y, color);
            }
        }
    }
}

void drawingLine(coords start, coords end, rgb color, unsigned int thickness, BMPFile* img) {
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;
    int x = x1;
    int y = y1;
    while (x != x2 || y != y2) {
        draw_pixel(img,x,y,color);
        if (thickness % 2 == 0) {
            fill_circle(img, x, y, thickness / 2, color);
        } else if (thickness == 1) {
            fill_circle(img, x, y, 0, color);
        } else {
            fill_circle(img, x, y, (thickness + 1) / 2, color);
        }
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}
bool checkColor(rgb a1, rgb a2){
    if(a1.blue == a2.blue && a1.green == a2.green && a1.red == a2.red){
        return true;
    } else{
        return false;
    }
}

bool check_coords(int y, int x, BitmapInfoHeader data){
    return !(y < 0 || y >= data.height || x < 0 || x >= data.width);
}
void fill_zone(int i, int j, rgb color, rgb fill_color, BMPFile* img) {//img->data массив пикселей
    if (!check_coords(i, j, img->bmih) || checkColor(img->data[i][j], color) || checkColor(img->data[i][j], fill_color)) { return; }
    img->data[i][j] = fill_color;
    fill_zone(i + 1, j, color, fill_color, img);
    fill_zone(i - 1, j, color, fill_color, img);
    fill_zone(i, j + 1, color, fill_color, img);
    fill_zone(i, j - 1, color, fill_color, img);
}


void drawingRectangle(rectangle data, BMPFile* img) {
    /*if (data.start.x + data.length < 0 || data.start.y + data.length < 0 || data.start.x > img->bmih.width || data.start.y > img->bmih.height) {
        wprintf(L"mypaint: некорректные координаты левого верхнего угла квадрата.\nПо команде «--help» можно получить дополнительную информацию.\n");
        return;
    }img*/

    coords left_top = data.start;
    coords right_top = {data.start.x, data.end.y};
    coords left_bottom = {data.end.x, data.start.y};
    coords right_bottom = data.end;

    drawingLine(left_top, right_top, data.color, data.thickness, img);
    drawingLine(left_top, left_bottom, data.color, data.thickness, img);
    drawingLine(right_top, right_bottom, data.color, data.thickness, img);
    drawingLine(left_bottom, right_bottom, data.color, data.thickness, img);

    if (data.filled == 1) {
    rgb fill_colors;
    fill_colors.red = data.fill_color.red;
    fill_colors.green = data.fill_color.green;
    fill_colors.blue = data.fill_color.blue;
    for (int y = data.start.y; y < data.end.y + 1; y++) {
        for (int x = data.start.x; x < data.end.x + 1; x++) {
            draw_pixel(img, x, y, fill_colors);
        }
    }
}
}
int is_inside_hexagon(coords polygon[], int n, coords p) {
    int i, j, c = 0;
    for (i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
            c = !c;
    }
    return c;
}

void drawingHexagon(hexagon data, BMPFile* img) {
    coords l1 = {data.centre.x - (data.radius / 2), data.centre.y + data.radius * sqrt(3.0) / 2};
    coords l2 = {data.centre.x + (data.radius / 2), data.centre.y + data.radius * sqrt(3.0) / 2};
    coords l3 = {data.centre.x + data.radius, data.centre.y};
    coords l4 = {data.centre.x + (data.radius / 2), data.centre.y - data.radius * sqrt(3.0) / 2};
    coords l5 = {data.centre.x - (data.radius / 2), data.centre.y - data.radius * sqrt(3.0) / 2};
    coords l6 = {data.centre.x - data.radius, data.centre.y};

    drawingLine(l1, l2, data.color, data.thickness, img);
    drawingLine(l2, l3, data.color, data.thickness, img);
    drawingLine(l3, l4, data.color, data.thickness, img);
    drawingLine(l4, l5, data.color, data.thickness, img);
    drawingLine(l5, l6, data.color, data.thickness, img);
    drawingLine(l6, l1, data.color, data.thickness, img);
    coords poligon[6]={l1, l2, l3, l4, l5, l6};

    if (data.filled == 1) {
        rgb fill_colors;
        fill_colors.red = data.fill_color.red;
        fill_colors.green = data.fill_color.green;
        fill_colors.blue = data.fill_color.blue;
        for (int y = l5.y; y < l2.y + 1; y++) {
            for (int x = l6.x; x < l3.x + 1; x++) {
                coords point = {x, y};
                if (is_inside_hexagon(poligon, 6,point)) {
                    draw_pixel(img, x, y, fill_colors);
                }
            }
        }
    }
}


int main()
{
    BMPFile* img = readBMPFile("Airplane.bmp");
    
    rectangle rect;
    rect.color.red=255;
    rect.color.blue=0;
    rect.color.green=0;
    rect.fill_color.blue=250;
    rect.fill_color.green=0;
    rect.fill_color.red=200;
    rect.filled=1;
    rect.thickness=5;
    rect.start.x=5;
    rect.start.y=5;
    rect.end.x=150;
    rect.end.y=100;
    drawingRectangle(rect, img);
    

    
    hexagon hex;
    hex.centre.x=250;
    hex.centre.y=250;
    hex.color.red=200;
    hex.color.blue=0;
    hex.color.green=130;
    hex.filled=1;
    hex.thickness=5;
    hex.radius=100;
    hex.fill_color.red=170;
    hex.fill_color.blue=200;
    hex.fill_color.green=0;
    drawingHexagon(hex, img);
    
    coords t1, t2;
    t1.x = 100;
    t1.y = 100;
    t2.x = 200;
    t2.y = 200;
    
        
    writeBMPFile("fill.bmp", img);
}
