//добавить флаг(структуру)
//добавить в гетопт
//добавить реализацию
//добаввить в свич
//добавить в мэйн это:
/*
if(flags.frame){
        frm.color = color;
        frm.thickness = thickness;
        drawFrame(img, frm.color, frm.thickness);
    }
*/

void drawFrame(BMPFile* img, rgb color, int thickness){
    img->bmih.height += thickness*2;
    img->bmih.width += thickness*2;
    img->bmih.imageSize += (img->bmih.width) * sizeof(rgb);
    img->bmfh.filesize += (img->bmih.width) * sizeof(rgb);

    rgb** new = (rgb**) malloc(img->bmih.height * sizeof(rgb*));
    for (int y = 0; y < img->bmih.height; y++) {
        new[y] = (rgb*) malloc(img->bmih.width * sizeof(rgb));
    }

    for(int y = 0; y<thickness; y++){
        for(int x = 0; x < img->bmih.width; x++){
            new[y][x].red = color.red;
            new[y][x].green = color.green;
            new[y][x].blue = color.blue;
        }
    }
    for(int y = 0; y < img->bmih.height - thickness*2; y++){
        for(int x = 0; x<thickness; x++){
            new[y+thickness][x].red = color.red;
            new[y+thickness][x].green = color.green;
            new[y+thickness][x].blue = color.blue;
        }
        for(int x = thickness; x<img->bmih.width-thickness; x++){
            new[y+thickness][x] = img->data[y][x-thickness];
        }
        for(int x = img->bmih.width-thickness; x<img->bmih.width; x++){
            new[y+thickness][x].red = color.red;
            new[y+thickness][x].green = color.green;
            new[y+thickness][x].blue = color.blue;
        }
    }
    for(int y = img->bmih.height - thickness; y < img->bmih.height; y++){
        for(int x = 0; x<img->bmih.width; x++){
            new[y][x].red = color.red;
            new[y][x].green = color.green;
            new[y][x].blue = color.blue;
            
        }
    }


    img->data = new;

}
