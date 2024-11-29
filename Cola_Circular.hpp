#ifndef COLA_CIRCULAR_H
#define COLA_CIRCULAR_H

#include <iostream>
#include <vector>
#include <mutex>
#include <fstream>
#include <condition_variable>

class Cola_Circular {
private:
    std::vector<int> buffer;
    int capacidad;
    int inicio;
    int fin;
    int elementos;
    std::mutex mutex;
    std::condition_variable producir;
    std::condition_variable consumir;
    std::ofstream log; 

public:
    // Constructor
    Cola_Circular(int capacidad);

    // Destructor
    ~Cola_Circular();

    // Métodos para insertar y extraer elementos
    void insertar(int num, int id);
    bool extraer(int &num, int id, int time);

    // Método para verificar si la cola está vacía
    bool estaVacia() const;

    // Método para verificar si la cola está llena
    bool estaLlena() const;

    // Método para duplicar el tamaño de la cola
    void duplicar();

    //Metodo para reducir el tamaño de la cola
    void reducir();

    void mostrar();
};

#endif