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

