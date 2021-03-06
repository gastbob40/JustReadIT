#include <stdio.h>
#include <stdlib.h>
#include "neuralnetwork.h"
#include <SDL/SDL.h>
#include "../image_segmentation/index.h"

extern int nb_fonts;
extern int nb_results;

char result_elements[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '.', ';', ':', '!', '?',
        '\'', '(', ')', '[', ']', '-', '%'
};

double *get_matrix(SDL_Surface *image) {
    // Create the new array
    int length = 32;
    double *array = calloc(32 * 32, sizeof(double));

    // convert the image into matrix
    Uint8 r, g, b;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i >= image->w || j >= image->h) {
                array[i * length + j] = 0;
            } else {
                Uint32 pixel = (get_pixel(image, i, j));
                SDL_GetRGB(pixel, image->format, &r, &b, &g);

                // if the pixel is black ->1 else -> 0
                if (r < 240 && g < 240 && b < 240)
                    array[i * length + j] = 1;
                else
                    array[i * length + j] = 0;
            }
        }
    }
    return array;
}

char get_letter_by_image(SDL_Surface *image) {
    // Load or init the NeuralNetwork
    extern NeuralNetwork *n;
    double *yes = get_matrix(image);

    /*for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%.0f.", yes[j * 32 + i]);
        }
        printf("\n");
    }*/
//    printf("\n\n\n\n");
    size_t result = run(n, yes);
    return result_elements[result];
}

void training(size_t len, size_t nb_layer, size_t hidden, int load, int save) {


    // Save all images into an array
    double **models = malloc(sizeof(char *) * nb_fonts * nb_results);

    char paths[] = "src/assets/training/font-01/letter000.bmp";

    int index_in = 0;
    for (int m = 0; m < nb_fonts; m++) {
        paths[36] = '0';
        paths[35] = '0';

        for (int m = 0; m < nb_results; m++) {


            SDL_Surface *image = SDL_LoadBMP(paths);
            models[index_in] = get_matrix(image);
            index_in++;
            if (paths[36] == '9') {
                paths[36] = '0';
                paths[35]++;
            } else
                paths[36]++;
        }

        if (paths[26] == '9') {
            paths[26] = '0';
            paths[25]++;
        } else
            paths[26]++;
    }

    // Array of 0
    double *expected = calloc(len, sizeof(double));

    // Load or init the NeuralNetwork
    NeuralNetwork network;
    NeuralNetwork *n = &network;
    if (load)
        load_neural_network(n);
    else {
        size_t nb_input = 32 * 32;
        size_t nb_output = nb_results;
        size_t nb_neurons_per_layer[] = {nb_input, hidden, hidden, nb_output};

        init(n, nb_layer, nb_neurons_per_layer);
    }

    // learning rate
    double coef = 0.1;

    // Principal learn function
    int k = 0;
    int result = 0;
    for (int i = 1; i < 100000; i++) {
        for (int j = 0; j <= 10; j++) {
            // Random letter
            k = random() % (nb_results * nb_fonts);

            // Expected response
            expected[k % nb_results] = (double) 1;

            // Update all errors and return result
            result = learn(n, models[k], expected);

            // Reset expected to 0
            expected[k % nb_results] = (double) 0;
        }
        // Update all links with the errors
        backpropagation(n, coef);

        // Print th result of the last test
        if (result_elements[k % nb_results] != result_elements[result])
            printf("\033[1;31m");
        else
            printf("\033[0;36m");

        printf("(%u) Expected : %c  ", i, result_elements[k % nb_results]);
        printf("result : %c   |   <input>: %i \n", result_elements[result], k);
        printf("\033[0m");
        // All 1000 call, save th network into a file
        if (i % 1000 == 0 && save)
            save_neural_network(n);
    }

    if (save)
        save_neural_network(n);
}

