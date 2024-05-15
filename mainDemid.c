#include "structs.h"
#include "work_with_bmp.h"
#include "functions.h"
#include "checks.h"

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


    coords left_up[2];
    coords rigth_down[2];
    rgb color[2];

    int fill[2] = {0, 0};
    rgb fill_color[2];
    int thickness[2];

    int i = 0;
    int j = 0;
    int k = 0;
    int h = 0;
    int g = 0;
    int z = 0;

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
                fill[g] = 1;    
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
                    cope.place = 1;
                } else{
                    cope.place = 2;
                }
                break;
            case 's':
                sscanf(optarg, "%d.%d", &cope.dest_start.x, &cope.dest_start.y);
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

    if(cope.place == 1){
        cope.start = left_up[0];
        cope.end = rigth_down[0];
    } else if (cope.place == 2){
        cope.start = left_up[1];
        cope.end = rigth_down[1];
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
        drawingRectangle(rectangle, img);
    }

    if(flags.hexagon){
        drawingHexagon(hexagon, img);
    }

    if(flags.copy){
        copy_and_paste_area(img, cope.start, cope.end, cope.dest_start);
    }
    
    if(flags.output){
        writeBMPFile(output_name, img);
    } else{
        writeBMPFile("out.bmp", img);
    }


    return 0;

}
