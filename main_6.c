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

    int i, j, k, h, g, z;

    char *input_name, *output_name;
    char *short_options="kpxhIi:u:d:t:c:f:e:r:d:o:z:";
    int opt, option_index;
    while ((opt=getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1){
        printf("%c\n", opt);
        printf("%d\n", option_index);
        switch (opt){
            case 'h':
                printf("place for description");
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
                    rectangle.place_"%hhd.%hhd.%hhd"1 = 1;
                } else{
                    rectangle.place_1 = 2;
                } 
                if(flags.copy == 0){
                    rectangle.place_2 == 1;
                } else{
                    rectangle.place_2 == 2;
                }
                break;
            case 'u':
                sscanf(optarg, "%d.%d", &left_up[k].x, &left_up[k].y);
                k++;
                break;
            case 'd':
                sscanf(optarg, "%d.%d", &rigth_down[i].x, &rigth_down[].y);
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
                sscanf(optarg, "%d", &fill[g]);
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
                break;
            case 'k':
                flags.copy = 1;
                if(flags.rect ==0){
                    copy.place = 1;
                } else{
                    copy.place = 2;
                }
                break;
            case 'd':
                sscanf(optarg, "%d.%d", &copy.dest_start.x, &copy.dest_start.y);
                break;
        }

    }
    
    if(rectangle.place_1 == 1){
        rectangle.start = left_up[0];
        rectangle.end = rigth_down[0];
    } else if (rectangle.place_1 == 2){
        rectangle.start = left_up[1];
        rectangle.end = rigth_down[1];
    }

    if(rectangle.place_2 == 1){
        rectangle.color = color[0];
        rectangle.filled = fill[0];
        rectangle.fill_color = fill_color[0];
    } else if (rectangle.place_2 == 2){
        rectangle.color=color[1];
        rectangle.filled=fill[1];
        rectangle.fill_color=fill_color[1];
    }

    if(hexagon.place == 1){
        hexagon.color = color[0];
        hexagon.filled = fill[0];
        hexagon.fill_color = fill_color[0];
    } else if (hexagon.place == 2){
        hexagon.color = color[1];
        hexagon.filled = fill[1];
        hexagon.fill_color = fill_color[1];
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
        writeBMPFile(output_name.bmp, img);
    } else{
        writeBMPFile(out.bmp, img);
    }
    return 0;
}
