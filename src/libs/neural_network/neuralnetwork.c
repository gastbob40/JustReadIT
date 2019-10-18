#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "neuralnetwork.h"

double sigmoide(double value) {
    return 1 / (1 + exp((-1) * value));
}

double derivative(double value) {
    return sigmoide(value) * (1 - sigmoide(value));
}

// Initialize the network from scratch
NeuralNetwork Init() {
    Neuron n, n1, n2, n3, n4;
    n.value = 0.9;
    n.bias = 0.2;
    n.link[0] = 1;
    n.link[1] = 1;
    n.nb_link = 2;
    n1 = n2 = n3 = n4 = n;
    n1.bias = 0.1;
    n2.bias = 0.7;
    n3.bias = 0.9;
    n4.bias = 0.1;

    NeuralNetwork net;
    net.error = 0.05;
    net.neurons[0] = n;
    net.neurons[1] = n1;
    net.neurons[2] = n2;
    net.neurons[3] = n3;
    net.neurons[4] = n4;
    net.nb_neurons = 5;
    return net;
}

void Print_Info(NeuralNetwork *network) {
    printf("=================Network Information=================\n");
    // Print all links information
    printf("Link :\n");
    printf("%f, %f\n", network->neurons[2].link[0], network->neurons[2].link[1]);
    printf("%f, %f\n", network->neurons[3].link[0], network->neurons[3].link[1]);
    printf("%f, %f\n", network->neurons[4].link[0], network->neurons[4].link[1]);

    // Print all neurons values
    printf("Values :\n");
    printf("%f, %f \n", network->neurons[0].value, network->neurons[1].value);
    printf("%f, %f \n", network->neurons[2].value, network->neurons[3].value);
    printf("%f \n", network->neurons[4].value);

    printf("=====================================================\n\n");
}

void Propagation(NeuralNetwork *network, int entry[], size_t len) {
    // Check if there is enough entry or neuron
    if (len == 2) {
        // Place the entry data on the first layer
        for (int i = 0; i < 2; i++) {
            network->neurons[i].value = entry[i];
        }
        double val_neuron = 0;
        // Propagation throught the neural network
        for (int j = 2; j < 5; j++) {
            val_neuron = 0;
            for (int k = 0; k < 2; k++) {
                val_neuron += network->neurons[j - 1 - k - (j % 2)].value * network->neurons[j].link[k];
            }
            val_neuron += network->neurons[j].bias;
            // Activation function
            network->neurons[j].value = sigmoide(val_neuron);
        }
    } else {
        printf("Error in Propagation\n");
    }
}

void Backpropagation(NeuralNetwork *network, int expected) {
    // double Co = (1/2)*(network->neurons[4].value - expected);
    double Activation[5];
    double Weight[6];
    double Bias[5];

    for (int i = 0; i < 5; i++) {
        Activation[i] = network->neurons[i].value;
    }

    Weight[0] = network->neurons[2].link[0];
    Weight[1] = network->neurons[2].link[1];
    Weight[2] = network->neurons[3].link[0];
    Weight[3] = network->neurons[3].link[1];
    Weight[4] = network->neurons[4].link[0];
    Weight[5] = network->neurons[4].link[1];

    for (int i = 0; i < 5; i++) {
        Bias[i] = network->neurons[i].bias;
    }

    double Gw[6];
    double Gb[3];

    //neurone 4
    Gw[5] = Activation[4] * derivative(Weight[5] * Activation[4] + Bias[4]) * (Activation[4] - expected);
    Gw[4] = Activation[4] * derivative(Weight[4] * Activation[4] + Bias[4]) * (Activation[4] - expected);

    //neurone 3
    Gw[3] = Activation[3] * derivative(Weight[3] * Activation[3] + Bias[3]) * (Activation[3] - expected);
    Gw[2] = Activation[3] * derivative(Weight[2] * Activation[3] + Bias[3]) * (Activation[3] - expected);

    //neurone 2
    Gw[1] = Activation[2] * derivative(Weight[1] * Activation[2] + Bias[2]) * (Activation[2] - expected);
    Gw[0] = Activation[2] * derivative(Weight[0] * Activation[2] + Bias[2]) * (Activation[2] - expected);


    Gb[2] = (derivative(Weight[4] * Activation[4] + Bias[4]) * (Activation[4] - expected) +
             (derivative(Weight[5] * Activation[4] + Bias[4]) * (Activation[4] - expected)));
    Gb[1] = (derivative(Weight[3] * Activation[4] + Bias[4]) * (Activation[4] - expected) +
             (derivative(Weight[2] * Activation[4] + Bias[4]) * (Activation[4] - expected)));
    Gb[0] = (derivative(Weight[0] * Activation[4] + Bias[4]) * (Activation[4] - expected) +
             (derivative(Weight[1] * Activation[4] + Bias[4]) * (Activation[4] - expected)));


    network->neurons[2].link[0] -= Gw[0];
    network->neurons[2].link[1] -= Gw[1];
    network->neurons[3].link[0] -= Gw[2];
    network->neurons[3].link[1] -= Gw[3];
    network->neurons[4].link[0] -= Gw[4];
    network->neurons[4].link[1] -= Gw[5];


    network->neurons[4].bias -= Gb[2];
    network->neurons[3].bias -= Gb[1];
    network->neurons[2].bias -= Gb[0];




    //network->neurons[j-1].link[k] -= network->error * ((-1)*network->neurons[j].value * sum * (1-sum)*network->neurons[j-1-(j%2)].value);

    /*
       for (size_t i = network.lenvalue -1; i > 0; i--)
       {

       for (size_t j = 0; j < network.lenlayer-1; j++)
       {

       for (size_t k = 0; k < network.lenlayer[i]; k++)
       {
       sum = 0;

       for (size_t l = 0; l < &network.valuesize[i]; l++)
       {
    // On effectue la somme pondérée du neurone vers lequel pointe la connexion :
    sum += network.value[i-1][l] * network.link[i-l][l][k];
    }
    sum = sigmoide(sum);// fonction sigmoide je crois

    // On met à jour le poids de la connexion
    network.link[i-1][j][k] -= network.error * ((-1) * network.value[i][k] * sum * (1-sum)* network.value[i-1][j]);
    }

    for (size_t j = 0; j < &network.valuesize[i-1]; j++)
    {
    sum = 0;

    for (size_t k = 0; k < &network.value[i]; k++)
    {
    // On met à jour les neurones de la prochaine couche en fonction de l'erreur qui se rétropropage
    sum += network.value[i][k] * network.link[i-1][j][k];
    }
    network.value[i-l][j] = sum;
    }
    }
    }*/
}

// Machine learning function
void Learn(NeuralNetwork *network, int entry[], int expected) {
    Propagation(network, entry, 2);
    Backpropagation(network, expected);
    printf("Output : %f\n", network->neurons[4].value);
}

// Pass through the neural network and return the output
double Run(NeuralNetwork *network, int entry[], size_t len) {
    Propagation(network, entry, len);
    printf("Output : %f\n", network->neurons[4].value);
    return network->neurons[4].value;
}

// Save network into save_network.txt
void Save_Neural_Network(NeuralNetwork *network) {
    FILE *file = NULL;
    // Set the file where to save data
    file = fopen("src/libs/neural_network/save_network.txt", "w");

    // Write all info
    for (size_t i = 0; i < network->nb_neurons; i++) {
        fprintf(file, "Value :\n%f\n", network->neurons[i].value);
        fprintf(file, "Links :\n%f; %f\n\n", network->neurons[i].link[0], network->neurons[i].link[1]);
    }

    // Close the file
    fclose(file);
}

// Get information from one neuron in save_network.txt
Neuron Load_Neuron(FILE *file) {
    double neuron_data[3];
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neuron\n");
        exit(EXIT_FAILURE);
    } else {
        // Get neuron data through thanks to a pattern
        fscanf(file, "Value :\n%lf\nLinks :\n%lf; %lf\n\n", &neuron_data[0], &neuron_data[1], &neuron_data[2]);

        // Creation of the neuron with data from save_network information
        Neuron n;
        n.value = neuron_data[0];
        n.link[0] = neuron_data[1];
        n.link[1] = neuron_data[2];
        return n;
    }
}

// Create a network with data from save_network.txt
NeuralNetwork Load_Neural_Network() {
    FILE *file = NULL;
    // Get the file where network data is saved
    file = fopen("src/libs/neural_network/save_network.txt", "r");
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neural network\n");
        exit(EXIT_FAILURE);
    } else {
        // Creation of the network with all data in save_network.txt
        NeuralNetwork net;
        // Init all neurons
        for (int i = 0; i < 5; i++) {
            net.neurons[i] = Load_Neuron(file);
        }

        // Close file
        fclose(file);
        return net;
    }
}

