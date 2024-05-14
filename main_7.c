//заливка у шестиугольника херня
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
    int filled;
    rgb fill_color;
    int place_1, place_2;
} rectangle;

typedef struct hex{
    //coords start, sec, thrd, fourth, fifth, end;
    unsigned int thickness;
    rgb color;
    int filled;
    rgb fill_color;
    coords centre;
    unsigned int radius;
    int place;
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
int isInsideArea(coords polygonPoints[], int nAngle, coords point) {
    int i, j, c = 0;
    for (i = 0, j = nAngle - 1; i < nAngle; j = i++) {
        if (((polygonPoints[i].y > point.y) != (polygonPoints[j].y > point.y)) &&
            (point.x < (polygonPoints[j].x - polygonPoints[i].x) * (point.y - polygonPoints[i].y) / (polygonPoints[j].y - polygonPoints[i].y) + polygonPoints[i].x))
            c = !c;
    }
    return c;
}

void drawingRectangle(rectangle data, BMPFile* img) {
    coords left_top = data.start;
    coords right_top = {data.start.x, data.end.y};
    coords left_bottom = {data.end.x, data.start.y};
    coords right_bottom = data.end;

    drawingLine(left_top, right_top, data.color, data.thickness, img);
    drawingLine(left_top, left_bottom, data.color, data.thickness, img);
    drawingLine(right_top, right_bottom, data.color, data.thickness, img);
    drawingLine(left_bottom, right_bottom, data.color, data.thickness, img);

    if (data.filled == 1) {
        for (int y = data.start.y; y < data.end.y + 1; y++) {
            for (int x = data.start.x; x < data.end.x + 1; x++) {
                draw_pixel(img, x, y, data.fill_color);
                
            }
        }
    }
}

void drawingHexagon(hexagon data, BMPFile* img) {
    coords l1 = {data.centre.x - (data.radius / 2), data.centre.y + data.radius * sqrt(3.0) / 2};
    coords l2 = {data.centre.x + (data.radius / 2), data.centre.y + data.radius * sqrt(3.0) / 2};
    coords l3 = {data.centre.x + data.radius, data.centre.y};
    coords l4 = {data.centre.x + (data.radius / 2), data.centre.y - data.radius * sqrt(3.0) / 2};
    coords l5 = {data.centre.x - (data.radius / 2), data.centre.y - data.radius * sqrt(3.0) / 2};
    coords l6 = {data.centre.x - data.radius, data.centre.y};
    coords poligon[6]={l1, l2, l3, l4, l5, l6}; 
    if (data.filled == 1) {
        for (int y = l5.y; y < l1.y ; y++) {
            for (int x = l6.x; x < l3.x+1 ; x++) {
                coords point = {x, y};
                if (isInsideArea(poligon, 6, point)) {
                        draw_pixel(img, x, y, data.fill_color);
                }
            }
        }
    }
    drawingLine(l1, l2, data.color, data.thickness, img);
    drawingLine(l2, l3, data.color, data.thickness, img);
    drawingLine(l3, l4, data.color, data.thickness, img);
    drawingLine(l4, l5, data.color, data.thickness, img);
    drawingLine(l5, l6, data.color, data.thickness, img);
    drawingLine(l6, l1, data.color, data.thickness, img);
    
    
}
void copy_and_paste_area(BMPFile* img, coords start, coords end, coords dest_start) {
    int width = end.x - start.x;
    int height = end.y - start.y;

    // Создаем временный буфер для хранения скопированной области
    rgb** temp_buffer = (rgb**)malloc(sizeof(rgb*) * height);
    if (!temp_buffer) {
        printf("Memory allocation failed.\n");
        return;
    }
    for (int i = 0; i < height; i++) {
        temp_buffer[i] = (rgb*)malloc(sizeof(rgb) * width);
        if (!temp_buffer[i]) {
            printf("Memory allocation failed.\n");
            // Освобождаем ранее выделенную память
            for (int j = 0; j < i; j++) {
                free(temp_buffer[j]);
            }
            free(temp_buffer);
            return;
        }
    }

    // Копируем область изображения во временный буфер
    for (int y = start.y; y < end.y; y++) {
        for (int x = start.x; x < end.x; x++) {
            temp_buffer[y - start.y][x - start.x] = img->data[y][x];
        }

   }

    // Вставляем скопированную область в указанное место в изображении
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            img->data[dest_start.y + y][dest_start.x + x] = temp_buffer[y][x];
        }
    }

    // Освобождаем временный буфер
    for (int i = 0; i < height; i++) {
        free(temp_buffer[i]);
    }
    free(temp_buffer);
}

struct {
    int rect,
    hexagon,
    copy,
    input,
    output;
} flags;

typedef struct copy{
    coords start, end, dest_start;
    int place;
}copy;
static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"info", no_argument, 0, 'I'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},

    {"rect", no_argument, 0, 'p'},
    {"left_up", required_argument, 0, 'u'},
    {"right_down", required_argument, 0, 'd'},
    {"thickness", required_argument, 0, 't'},
    {"color", required_argument, 0, 'c'},
    {"fill", required_argument, 0, 'f'},
    {"fill_color", required_argument, 0, 'z'},

    {"hexagon", no_argument, 0, 'x'},
    {"center", required_argument, 0, 'e'}, 
    {"radius", required_argument, 0, 'r'},

    {"copy", no_argument, 0, 'k'},
    {"dest_left_up", required_argument, 0, 's'},
    {NULL, 0, NULL, 0}
};
//BMPFile* img = readBMPFile("Airplane.bmp");hell-guss@hellguss-HP:~/Рабочий стол/cw_2$ gcc main.c                             ^
//writeBMPFile("fill.bmp", img);
int main(int argc, char** argv)
{
    BMPFile* img = readBMPFile("Airplane.bmp");

    rectangle rectangle;
    hexagon hexagon;
    copy copy;

    coords left_up[2];
    coords rigth_down[2];
    rgb color[2];

    int fill[2];
    rgb fill_color[2];
    int thickness[2];

    int i = 0;
    int j = 0;
    int k = 0;
    int h = 0;
    int g = 0;
    int z = 0;

    char *input_name, *output_name;
    char *short_options="kpxhIi:u:d:t:c:f:e:r:s:o:z:";
    int opt, option_index;
    while ((opt=getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1){
        printf("%c\n", opt);
        printf("%d\n", option_index);
        switch (opt){
            case 'h':
                printf("place for description\n");
                exit(0);
            case 'I':
                print_file_header(img->bmfh);
                print_info_header(img->bmih);
                break;
            case 'i':
                flags.input = 1;
                input_name = optarg;
                break;
            case 'o':
                flags.output = 1;
                output_name = optarg;
                break;
            case 'p':
                flags.rect = 1;
                if(flags.hexagon == 0){                         
                    rectangle.place_1 = 1;
                } else{
                    rectangle.place_1 = 2;
                } 
                if(flags.copy == 0){
                    rectangle.place_2 = 1;
                } else{
                    rectangle.place_2 = 2;
                }
                break;
            case 'u':
                sscanf(optarg, "%d.%d", &left_up[k].x, &left_up[k].y);
                k++;
                break;
            case 'd':
                sscanf(optarg, "%d.%d", &rigth_down[i].x, &rigth_down[i].y);
                i++;
                break;
            case 't':
                sscanf(optarg, "%d", &thickness[h]);
                h++;
                break;
            case 'c':
                sscanf(optarg, "%hhd.%hhd.%hhd", &color[j].red, &color[j].green, &color[j].blue);
                j++;
                break;
            case 'f':
                if(!strcmp(optarg, "true")){
                    fill[g] = 1;    
                } else if(!strcmp(optarg, "false")){
                    fill[g] = 0;
                }
                g++;
                break;
            case 'z':
                sscanf(optarg, "%hhd.%hhd.%hhd", &fill_color[z].red, &fill_color[z].green, &fill_color[z].blue);
                z++;
                break;
            case 'x':
                flags.hexagon = 1;
                if(flags.rect == 0){
                    hexagon.place = 1;
                } else{
                    hexagon.place = 2;
                }
                break;
            case 'e':
                sscanf(optarg, "%d.%d", &hexagon.centre.x, &hexagon.centre.y);
                
                break;
            case 'r':
                sscanf(optarg, "%d", &hexagon.radius);
                printf("))%d\n", hexagon.radius);
                break;
            case 'k':
                flags.copy = 1;
                if(flags.rect ==0){
                    copy.place = 1;
                } else{
                    copy.place = 2;
                }
                break;
            case 's':
                sscanf(optarg, "%d.%d", &copy.dest_start.x, &copy.dest_start.y);
                break;
        }

    }
    
    if(rectangle.place_2 == 1){
        rectangle.start = left_up[0];
        rectangle.end = rigth_down[0];
    } else if (rectangle.place_2 == 2){
        rectangle.start = left_up[1];
        rectangle.end = rigth_down[1];
    }

    if(rectangle.place_1 == 1){
        rectangle.color = color[0];
        rectangle.filled = fill[0];
        rectangle.fill_color = fill_color[0];
        rectangle.thickness = thickness[0];
    } else if (rectangle.place_1 == 2){
        rectangle.color=color[1];
        rectangle.filled=fill[1];
        rectangle.fill_color=fill_color[1];
        rectangle.thickness = thickness[1];
    }

    if(hexagon.place == 1){
        hexagon.color = color[0];
        hexagon.filled = fill[0];
        hexagon.fill_color = fill_color[0];
        hexagon.thickness = thickness[0];
    } else if (hexagon.place == 2){
        hexagon.color = color[1];
        hexagon.filled = fill[1];
        hexagon.fill_color = fill_color[1];
        hexagon.thickness = thickness[1];
    }

    if(copy.place == 1){
        copy.start = left_up[0];
        copy.end = rigth_down[0];
    } else if (copy.place == 2){
        copy.start = left_up[1];
        copy.end = rigth_down[1];
    }

    if(flags.input){
        BMPFile* img = readBMPFile(input_name);
    } else{
        input_name = argv[argc - 1];
        BMPFile* img = readBMPFile(input_name);
    }

    

    if(flags.rect){
        drawingRectangle(rectangle, img);
    }

    if(flags.hexagon){
        drawingHexagon(hexagon, img);
    }

    if(flags.copy){
        copy_and_paste_area(img, copy.start, copy.end, copy.dest_start);
    }
    
    if(flags.output){
        writeBMPFile(output_name, img);
    } else{
        writeBMPFile("out.bmp", img);
    }
    return 0;


}
