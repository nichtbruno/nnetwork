#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

Img **csv_to_imgs(char *file_string, int number_of_imgs) {
    FILE *file;
    Img **imgs = malloc(number_of_imgs * sizeof(Img*));
    char row[MAXCHAR];
    file = fopen(file_string, "r");

    fgets(row, MAXCHAR, file);
    int i=0;
    while (feof(file) != 1 && i < number_of_imgs) {
        imgs[i] = malloc(sizeof(Img));
        int j = 0;
        fgets(row, MAXCHAR, file);
        char *token = strtok(row, ",");
        imgs[i]->img_data = matrix_create(28, 28);
        while (token != NULL) {
            if (j == 0) {
                imgs[i]->label = atoi(token);
            } else {
                imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = atoi(token) / 256.0;
            }
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    fclose(file);
    return imgs;
}

void img_print(Img *img) {
    matrix_print_ascii(img->img_data);
    printf("Img Label: %d\n", img->label);
}

void img_free(Img* img) {
    matrix_free(img->img_data);
    free(img);
    img = NULL;
}

void imgs_free(Img** imgs, int n) {
    for (int i = 0; i < n; i++) {
        img_free(imgs[i]);
    }
    free(imgs);
    imgs = NULL;
}
