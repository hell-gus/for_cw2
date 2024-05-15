#include "structs.h"

char* my_strdup(const char* str);
bool checkColor(rgb a1, rgb a2);
bool check_coords(int y, int x, BitmapInfoHeader data);
bool checkFormatCoords(const char* coord);
bool checkThickness(void* data_ptr);
bool checkColorFormat(const char* color);
bool checkRadius(hexagon data);
