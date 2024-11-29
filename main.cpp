#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include "Cola_Circular.hpp"

void productor(Cola_Circular& cola_circular, int id, int cantidad){
    for (int i = 0; i < cantidad; i++) {
        cola_circular.insertar(i, id);    // Inserta el dato en la cola
        printf("Productor: %d produjo: %d\n", id, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simula un tiempo de trabajo
    }
}

void consumidor(Cola_Circular& cola_circular, int id, int time) {
    int dato;
    while (true){
        if(!cola_circular.extraer(dato, id, time)){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simula un tiempo de trabajo
    }
}

int main(int argc, char **argv) {
    
    std::srand(time(0));

    if (argc != 9) {
        std::cerr << "Error de entrada: ./simulapc -p <productores> -c <consumidores> -s <cap_inicial> -t <tiempo_espera>" << std::endl;
        return 1;
    }
     
    int num_productores = atoi(argv[2]);
    int num_consumidores = atoi(argv[4]);
    int capacidad_inicial = atoi(argv[6]);
    int datos_por_productor = std::rand() % 20;
    int time = atoi(argv[8]);;

    //Creacion de la cola
    Cola_Circular cola_circular(capacidad_inicial);
    
    //Creación de hebras porductoras
    std::vector<std::thread> productores;
    for(int i = 0; i < num_productores; i++){
        productores.emplace_back(productor, std::ref(cola_circular), i, datos_por_productor);
    }

    //Cracion de hebras consumidoras
    std::vector<std::thread> consumidores;
    for(int i = 0; i < num_consumidores; i++){
        consumidores.emplace_back(consumidor, std::ref(cola_circular), i, time);
    }

    // Esperar a los productores
    for (auto& hebra : productores) {
        hebra.join();
    }

    // Esperar a los consumidores
    for (auto& hebra : consumidores) {
        hebra.join();
    }

    return 0;
}