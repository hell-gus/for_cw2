#include "structs.h"
#include "work_with_bmp.h"
#include "functions.h"

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


struct {
    int rect,
    hexagon,
    copy,
    info,
    input,
    output;
} flags;

typedef struct copy{
    coords start, end, dest_start;
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
    {"fill", no_argument, 0, 'f'},
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

    BMPFile* img = NULL;

    rectangle rectangle = {0};
    hexagon hexagon = {0};
    copy cope = {0};


    coords left_up;
    coords rigth_down;
    rgb color;

    int fill = 0;
    rgb fill_color;
    int thickness;

    flags.info=0;
    char *input_name, *output_name;
    char *short_options="kpxhIi:u:d:t:c:f:e:r:s:o:z:";
    int opt, option_index;
    while ((opt=getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1){
        //printf("%c\n", opt);
        //printf("%d\n", option_index);
        switch (opt){
            case 'i':
                flags.input = 1;
                input_name = optarg;
                break;
            case 'o':
                flags.output = 1;
                output_name = optarg;
                break;
            case 'h':
                printf("Course work for option 4.9, created by Elena Guseva.\n");
                printf("place for description\n");
                exit(0);
            case 'I':
                flags.info = 1;
                break;
            case 'p':
                flags.rect = 1;
                break;
            case 'u':
                sscanf(optarg, "%d.%d", &left_up.x, &left_up.y);
                break;
            case 'd':
                sscanf(optarg, "%d.%d", &rigth_down.x, &rigth_down.y);
                break;
            case 't':
                sscanf(optarg, "%d", &thickness);
                break;
            case 'c':
                sscanf(optarg, "%hhd.%hhd.%hhd", &color.red, &color.green, &color.blue);
                break;
            case 'f':
                fill = 1;    
                break;
            case 'z':
                sscanf(optarg, "%hhd.%hhd.%hhd", &fill_color.red, &fill_color.green, &fill_color.blue);
                break;
            case 'x':
                flags.hexagon = 1;
                break;
            case 'e':
                sscanf(optarg, "%d.%d", &hexagon.centre.x, &hexagon.centre.y);
                break;
            case 'r':
                sscanf(optarg, "%d", &hexagon.radius);
                break;
            case 'k':
                flags.copy = 1;
                break;
            case 's':
                sscanf(optarg, "%d.%d", &cope.dest_start.x, &cope.dest_start.y);
                break;
        }

    }


    if(flags.input){
        img = readBMPFile(input_name);
    } else{
        input_name = argv[argc - 1];
        img = readBMPFile(input_name);
    }

    if(flags.info == 1){
        if(input_name){
        print_file_header(img->bmfh);
        print_info_header(img->bmih);
        } else {
        printf("Error: BMPii file not loaded.\n");
        }
    }
    

    if(flags.rect){
        rectangle.start = left_up;
        rectangle.end = rigth_down;
        rectangle.thickness = thickness;
        rectangle.color = color;
        rectangle.fill_color = fill_color;
        rectangle.filled = fill;
        drawingRectangle(rectangle, img);
    }

    if(flags.hexagon){
        hexagon.color = color;
        hexagon.fill_color = fill_color;
        hexagon.filled = fill;
        drawingHexagon(hexagon, img);
    }

    if(flags.copy){
        cope.start = left_up;
        cope.end = rigth_down;
        copy_and_paste_area(img, cope.start, cope.end, cope.dest_start);
    }
    
    if(flags.output){
        writeBMPFile(output_name, img);
    } else{
        writeBMPFile("out.bmp", img);
    }


    return 0;

}

