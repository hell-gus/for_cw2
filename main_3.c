//здесь функция по рисованию фигуры петина, в ней проблемы с толщиной линии
//а также здесь реализвано 2/3 функций, но с рекурсивной заливкой

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
void drawingLine(coords start, coords end, rgb new_pix, unsigned int thickness, BMPFile* img) {
    thickness = 0;
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    img->data[y2][x2] = new_pix;
    while(x1 != x2 || y1 != y2) 
  {  

      img->data[y1][x1] = new_pix;
      int error2 = error * 2;
      if(error2 > -deltaY) 
      {
        error -= deltaY;
        x1 += signX;
      }
      if(error2 < deltaX) 
      {
        error += deltaX;
        y1 += signY;
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

    /*
    if (data.filled==1){
        for(int i = data.start.y+data.thickness; i < data.end.y; i++){
            for(int j = data.start.x+data.thickness; j < data.end.x; j++){
                img->data[i][j].blue = data.fill_color.blue;
                img->data[i][j].green = data.fill_color.green;
                img->data[i][j].red = data.fill_color.red;
                }
        }
    }*/
    if (data.filled==1){
        fill_zone(data.start.y+1, data.start.x+1, data.color, data.fill_color, img);
    }
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

    if(data.filled==1){
        fill_zone(data.centre.y, data.centre.x, data.color, data.fill_color, img);
    }      
}

rgb** cut_image(rgb **arr, BitmapInfoHeader* bmif, coords left_top, coords right_bottom){
 // выделяем место под обрезанную картинку
    int size;
    size=left_top.x-right_bottom.x;

    rgb ** new_arr = malloc(size * sizeof(rgb *));
    for (int i = 0; i < size; i++){
        new_arr[i] = malloc(size * sizeof(rgb));
    }
    // выбираем определенную часть из исходного изображения
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (i >= 0 && j >= 0 && j < bmif->width && bmif->height - left_top.y - i >= 0 && bmif->height - left_top.y - i < bmif->height && left_top.x + j >= 0 && left_top.x + j < bmif->width){
                new_arr[size - i - 1][j] = arr[bmif->height - left_top.y - i][left_top.x + j];
            }
        }
    }
    // удаляем старую картинку
    for(int i = 0; i < bmif->height; i++){
        free(arr[i]);
    }
    free(arr);
    // меняем размер картинки в информационном заголовке
    bmif->height = size;
    bmif->width = size;
    return new_arr;
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
    rect.start.x=0;
    rect.start.y=0;
    rect.end.x=150;
    rect.end.y=100;
    drawingRectangle(rect, img);
    

    
    hexagon hex;
    hex.centre.x=250;
    hex.centre.y=250;
    hex.color.red=255;
    hex.color.blue=0;
    hex.color.green=0;
    hex.filled=1;
    hex.thickness=1;
    hex.radius=100;
    hex.fill_color.red=0;
    hex.fill_color.blue=255;
    hex.fill_color.green=0;
    drawingHexagon(hex, img);
    
    coords t1, t2;
    t1.x = 100;
    t1.y = 100;
    t2.x = 200;
    t2.y = 200;
    //cut_image(img->data, img->bmih, t1, t2);
        
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
    
    
    coords l1={data.centre.x-(data.radius/2), data.centre.y+data.radius*pow(3, 0.5)/2};

    coords l2={data.centre.x+(data.radius/2), data.centre.y+data.radius*pow(3, 0.5)/2};

    coords l3={data.centre.x+data.radius, data.centre.y};

    coords l4={data.centre.x+(data.radius/2), data.centre.y-data.radius*pow(3, 0.5)/2};

    coords l5={data.centre.x-(data.radius/2), data.centre.y-data.radius*pow(3, 0.5)/2};

    coords l6={data.centre.x-data.radius, data.centre.y};

    printf("%d %d %d %d %d %d\n", l1.x, l2.x, l3.x, l4.x, l5.x, l6.x);
    printf("%d %d %d %d %d %d\n", l1.y, l2.y, l3.y, l4.y, l5.y, l6.y);
    
    заполняет прямоугольник внутри
    if (data.filled == 1) {
        coords k1, k2;
        k1=l1;
        k2=l5;
        k1.y-=1;
        k2.y+=1;
        while (k1.x!=l2.x){
            drawingLine(k1, k2, data.fill_color, data.thickness, img);
            k1.x++;
            k2.x++;
            
            
            
    if (data.filled == 1) {
        coords k1, k2;
        k1=l1;
        k2=l2;
        k1.y=l1.y-data.thickness;
        k2.y=l2.y-data.thickness;

        rgb ppx;
        ppx.green=255;
        ppx.blue=0;
        ppx.red=0;
        drawingLine(k1, k2, data.fill_color, data.thickness, img);
            while(k1.y!= l6.y && k2.y != l3.y){
                k1.y-=data.thickness;
                k2.y-=data.thickness;
                drawingLine(k1, k2, data.fill_color, data.thickness, img);  
                drawingLine(k1, k2, data.fill_color, data.thickness, img);

                
                continue;
            } 
            while(k1.y != l5.y+data.thickness && k2.y != l4.y+data.thickness ){
                k1.y-=data.thickness;
                k2.y-=data.thickness;

                k1.x+=1;
                k2.x-=1;
                drawingLine(k1, k2, ppx, data.thickness, img); 
                drawingLine(k1, k2, ppx, data.thickness, img);
            }
            
    }
    */
