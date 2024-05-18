//в структуре создать поля старт и энд
//добавить соотв структуру, флаг,гетопт,кейс

void makePaving(BMPFile* img, coords start, coords end) {
    int razn_y = end.y - start.y;
    int razn_x = end.x - start.x;

    if (razn_y <= 0 || razn_x <= 0) {
        printf("Invalid coordinates for the area.\n");
        return;
    }

    rgb** temp_buffer = (rgb**)malloc(sizeof(rgb*) * razn_y);
    if (!temp_buffer) {
        printf("Memory allocation failed.\n");
        return;
    }
    for (int i = 0; i < razn_y; i++) {
        temp_buffer[i] = (rgb*)malloc(sizeof(rgb) * razn_x);
        if (!temp_buffer[i]) {
            printf("Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {
                free(temp_buffer[j]);
            }
            free(temp_buffer);
            return;
        }
    }

    // Копирование выбранной области в временный буфер
    for (int y = 0; y < razn_y; y++) {
        for (int x = 0; x < razn_x; x++) {
            if (start.y + y < img->bmih.height && start.x + x < img->bmih.width) {
                temp_buffer[y][x] = img->data[start.y + y][start.x + x];
            }
        }
    }

    // Повторение узора по всей области изображения
    for (int y = 0; y < img->bmih.height; y++) {
        for (int x = 0; x < img->bmih.width; x++) {
            img->data[y][x] = temp_buffer[y % razn_y][x % razn_x];
        }
    }

    // Освобождение временного буфера
    for (int i = 0; i < razn_y; i++) {
        free(temp_buffer[i]);
    }
    free(temp_buffer);
}
