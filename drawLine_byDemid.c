int swap_int(int* a, int*b){
    int* swap;
    swap=a;
    a=b;
    b=swap;
}

void drawingLine(coords start, coords end, rgb color, int thickness, BMPFile* img){
    // вертикальная линия
    rgb** arr=img->data;
    int H=img->bmih.height;
    int W=img->bmih.width;
    int x0=start.x;
    int y0=start.y;
    int x1=end.x;
    int y1=end.y;
    int r=color.red;
    int g=color.green;
    int b=color.blue;
    if (x0 == x1){
        if (y0 > y1){
            swap_int(&y0, &y1);
        }
        for (int i = y0; i < y1; i++){
            for (int j = 0; j < thickness; j++){
                arr[i][x0 - j].red = r;
                arr[i][x0 - j].green = g;
                arr[i][x0 - j].blue = b;
            }
        }
    // горизонтальная линия
    } else if (y0 == y1){
        if (x0 > x1){
            swap_int(&x0, &x1);
        }
        for (int i = x0; i < x1; i++){
            for (int j = 0; j < thickness; j++){
                arr[y0 + j][i].r = r;
                arr[y0 + j][i].g = g;
                arr[y0 + j][i].b = b;
            }
        }
    //наклонная линия (алгоритм Брезенхема)
    }else{
        int res = 0;
        // Проверяем рост отрезка по оси икс и по оси игрек
        // Отражаем линию по диагонали, если угол наклона слишком большой
        if (abs(y1 - y0) > abs(x1 - x0))
        {
            swap_int(&x0, &y0); // Перетасовка координат вынесена в отдельную функцию для красоты
            swap_int(&x1, &y1);
            res = 1;
        }
        // Если линия растёт не слева направо, то меняем начало и конец отрезка местами
        if (x0 > x1)
        {
            swap_int(&x0, &x1);
            swap_int(&y0, &y1);
        }
        int dx = x1 - x0;
        int dy = abs(y1 - y0);
        int error = dx / 2; // Здесь используется оптимизация с умножением на dx, чтобы избавиться от лишних дробей
        int ystep = (y0 < y1) ? 1 : -1; // Выбираем направление роста координаты y
        int y = y0;
        for (int x = x0; x <= x1; x++)
        {
            if (res)
            {
                arr[y][x].r = r;
                arr[y][x].g = g;
                arr[y][x].b = b;
            }else{
                arr[x][y].r = r;
                arr[x][y].g = g;
                arr[x][y].b = b;
            }
            error -= dy;
            if (error < 0)
            {
                y += ystep;
                error += dx;
            }
        }
    }
}
