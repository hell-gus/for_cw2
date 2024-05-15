#include "checks.h"

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

#define getType(var) _Generic((var), \
    int: "int", \
    float: "float", \
    double: "double", \
    default: "unknown" \
)


bool checkFormatCoords(const char* coord) {
    coords a_coord;

    // Проверка наличия запятой в координатах
    if (strchr(coord, ',') != NULL) {
        printf("Неверный формат вводимой координаты: используйте точку вместо запятой\n");
        return false;
    }

    // Проверка что точка одна
    int dot_count = 0;
    for (int i = 0; i < strlen(coord); i++) {
        if (coord[i] == '.') {
            dot_count++;
        }
    }
    if (dot_count != 1) {
        printf("Неверный формат вводимой координаты: используйте одну точку для разделения координат\n");
        return false;
    }

    // Копия строки для безопасности
    char* coord_copy = strdup(coord);

    char* token = strtok(coord_copy, ".");
    int x = atoi(token);
    token = strtok(NULL, ".");
    int y = atoi(token);
    a_coord.x = x;
    a_coord.y = y;

    free(coord_copy);

    //printf("Тип x: %s\n", getType(a_coord.x));
    //printf("Тип y: %s\n", getType(a_coord.y));

    return true;
}

bool checkThickness(void* data_ptr) {
    // Приведение указателя к типу прямоугольника
    rectangle* rect_data = (rectangle*)data_ptr;
    // Приведение указателя к типу шестиугольника
    hexagon* hex_data = (hexagon*)data_ptr;

    // Проверка толщины линии для прямоугольника
    if (rect_data != NULL) {
        if (rect_data->thickness <= 0) {
            printf("Толщина линии меньше либо равна нулю\n");
            return false;
        }
    }

    // Проверка толщины линии для шестиугольника
    if (hex_data != NULL) {
        if (hex_data->thickness <= 0) {
            printf("Толщина линии меньше либо равна нулю\n");
            return false;
        }
    }

    return true;
}
bool checkColorFormat(const char* color) {
    // Проверка что нет запятой
    if (strchr(color, ',') != NULL) {
        printf("Неверный формат вводимой координаты: используйте точку вместо запятой\n");
        return false;
    }

    // Проверка что точек две
    int dot_count = 0;
    for (int i = 0; i < strlen(color); i++) {
        if (color[i] == '.') {
            dot_count++;
        }
    }
    if (dot_count != 2) {
        printf("Неверный формат вводимой координаты: используйте две точки для разделения цвета.\n");
        return false;
    }

    char* copys = strdup(color);

    char* token = strtok(copys, ".");
    int r = atoi(token);
    token = strtok(NULL, ".");
    int g = atoi(token);
    token = strtok(NULL, ".");
    int b = atoi(token);

    // Проверка диапазона значений RGB
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        printf("Неверный формат вводимой координаты: значения RGB должны быть в диапазоне от 0 до 255\n");
        return false;
    }

    // Формат правильный
    //printf("Цвет проверен: R=%d, G=%d, B=%d\n", r, g, b);
    return true;
}
bool checkRadius(hexagon data) {
    if ((int)data.radius <= 0) {
        printf("Радиус должен быть больше нуля\n");
        return false;
    }
    return true;
}
/*
int main() {
    char coord[20]; // Выделите достаточно памяти для ввода координат
    //printf("Введите координату в формате X.Y: ");
    //scanf("%s", coord);

    //rectangle rect;
    //scanf("%d", &rect.thickness);

    hexagon hex;
    scanf("%d", &hex.radius);
    //checkThickness(&hex); 
    checkRadius(hex);

    //char color[20]; // Выделите достаточно памяти для ввода цвета
    //printf("Введите цвет в формате R.G.B: ");
    //fgets(color, sizeof(color), stdin); // Считывание цвета как строки
    //checkColorFormat(color);

    

    return 0;
}*/
