#include "matrix/matrix.h"
#include "matrix/ops.h"
#include "gui/gui.h"
#include "neural/nn.h"
#include "util/img.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main() {
    srand(time(NULL));

    // int number_imgs = 60000;
    // Img **imgs = csv_to_imgs("./data/mnist_train.csv", number_imgs);
    // NeuralNetwork *net = network_create(784, 300, 10, 0.1);
    // network_train_batch_imgs(net, imgs, number_imgs);
    // network_save(net, "test_net");
    // imgs_free(imgs, number_imgs);
    // network_free(net);

    // int number_imgs = 10000;
    // Img **imgs = csv_to_imgs("data/mnist_train.csv", number_imgs);
    // NeuralNetwork *net = network_load("test_net");
    // int rimg = rand() % 10000;
    // img_print(imgs[rimg]);
    // Matrix *res = network_predict_img(net, imgs[rimg]);
    // printf("Network Predicted %d\n", matrix_argmax(res));
    // imgs_free(imgs, number_imgs);
    // network_free(net);

    Img *img = malloc(sizeof(Img));
    img->img_data = executeGui();
    NeuralNetwork *net = network_load("test_net");
    matrix_print_ascii(img->img_data);
    Matrix *res = network_predict_img(net, img);
    printf("Network Predicted %d\n", matrix_argmax(res));
    img_free(img);
    network_free(net);

    // int number_imgs = 10000;
    // Img** imgs = csv_to_imgs("./data/mnist_test.csv", number_imgs);
    // NeuralNetwork* net = network_load("test_net");
    // double score = network_predict_imgs(net, imgs, 1000);
    // printf("Score: %1.5f\n", score);
    // imgs_free(imgs, number_imgs);
    // network_free(net);

    return 0;
}
