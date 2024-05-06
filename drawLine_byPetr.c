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
