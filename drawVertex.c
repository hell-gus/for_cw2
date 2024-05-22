void drawVertex(BMPFile* img, coords start, int size, rgb fill_color){
    int x, y;
    x=start.x;
    y=start.y;
    int kat = size/sqrt(2);
    int x0 = x;
    int y0=y+kat;

    img->data[y][x].red = fill_color.red;
    img->data[y][x].green = fill_color.green;
    img->data[y][x].blue = fill_color.blue;

    img->data[y+2*kat][x].red = fill_color.red;
    img->data[y+2*kat][x].green = fill_color.green;
    img->data[y+2*kat][x].blue = fill_color.blue;

    img->data[y+kat][x+kat].red = fill_color.red;
    img->data[y+kat][x+kat].green = fill_color.green;
    img->data[y+kat][x+kat].blue = fill_color.blue;

    img->data[y+kat][x-kat].red = fill_color.red;
    img->data[y+kat][x-kat].green = fill_color.green;
    img->data[y+kat][x-kat].blue = fill_color.blue;
    coords center;
    coords l3;
    center.x = start.x;
    center.y = start.y + size;
    l3.x = center.x;
    l3.y = center.y + size; 

    coords a1={x, y};
    coords a2={x+kat, y+kat};
    coords a3={x-kat, y+kat};
    coords a4={x, y+2*kat};
    
    drawingLine(a1, a2, fill_color, 1, img);
    drawingLine(a1, a3, fill_color, 1, img);
    drawingLine(a2, a4, fill_color, 1, img);
    drawingLine(a3, a4, fill_color, 1, img);

    for(int i=0; i<=kat; i++){
        coords b1={x, y+i};
        coords b2={x+kat-i, y+kat};
        coords b3={x-kat+i, y+kat};
        coords b4={x, y+2*kat-i};
        drawingLine(b1, b2, fill_color, 1, img);
        drawingLine(b1, b3, fill_color, 1, img);
        drawingLine(b2, b4, fill_color, 1, img);
        drawingLine(b3, b4, fill_color, 1, img);
    }
    drawingLine(a4, a1, fill_color, 1, img);
}
