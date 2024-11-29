#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Cola_Circular.hpp"

        //Constructor
        Cola_Circular::Cola_Circular(int cap_inicial): capacidad(cap_inicial), inicio(0), fin(0), elementos(0){
            buffer.resize(cap_inicial);
            log.open("log.txt");
            log << "Log de la implementacíon" << std::endl;
        }

        Cola_Circular::~Cola_Circular(){
            log.close();
        }

        //Metodo para ingresar un elemento
        void Cola_Circular::insertar(int num, int id){
            //Bloqueo del mutex
            std::unique_lock<std::mutex> lock(mutex);

            //Esperear por si la cola está llena
            producir.wait(lock, [this](){ return !estaLlena(); });

            buffer[fin] = num;
            fin = (fin + 1) % capacidad;
            elementos++;

            log << "Productor: " << id << " produjo: " << num << std::endl;

            if(elementos == capacidad){
                std::cout << "La cola está llena *se duplica*" << std::endl; //Verificar si los elementos se ingresan en el buffer luego de que este se duplique
                log << "La cola está llena *se duplica*" << std::endl;
                duplicar();
            }

            //Despertar una hebra
            consumir.notify_one();
        }

        //Metodo para "sacar" un elemento
        bool Cola_Circular::extraer(int &num, int id, int time){

            //Bloqueo del mutex
            std::unique_lock<std::mutex> lock(mutex);

            //Esperar por si la cola está vacía
            //consumir.wait_for(lock, [this](){ return !estaVacia(); });

            if(!consumir.wait_for(lock, std::chrono::seconds(time), [this](){ return !estaVacia(); })) {
            printf("Consumidor %d no pudo consumir: timeout\n", id);
            return false; /* timeout alcanzado */
            }

            num = buffer[inicio];
            inicio = (inicio + 1) % capacidad;
            elementos--;

            log << "Consumidor: " << id << " consumío: " << num << std::endl;
            printf("Consumidor: %d consumío: %d\n", id, num);

            if(elementos == 0){
                printf("La cola está vacía\n");
                log << "La cola está vacía" << std::endl;

            }else if(elementos == capacidad/4){
                printf("La cola está al 25 porciento de su uso *se reduce*\n");
                log << "La cola está al 25 porciento de su uso *se reduce*" << std::endl;
                reducir();
            }
            //Despertar a una hebra
            producir.notify_one();
            return true;

        }

        //Metodos adicionales
        bool Cola_Circular::estaVacia() const{
            return elementos == 0;
        }
        
        bool Cola_Circular::estaLlena() const{
            return elementos == capacidad;
        }

        void Cola_Circular::duplicar(){
            capacidad = capacidad*2;
            std::vector<int> nuevo_buffer(capacidad);

            log << "Aumentando" << " el tamaño de la cola a " << capacidad << " elementos" << std::endl;

            //Copia de los elementos del buffer anterior
            for(int i = 0; i < elementos; i++){
                nuevo_buffer[i] = buffer[(inicio + i) % capacidad / 2];
            }

            //Cambio del puntero
            buffer = nuevo_buffer;
            inicio = 0;
            fin = elementos;

        }

        void Cola_Circular::reducir(){
            capacidad = capacidad / 2;
            std::vector<int> nuevo_buffer(capacidad);

            log << "Reduciendo" << " el tamaño de la cola a " << capacidad << " elementos" << std::endl;

            //Copia de los elementos del buffer anterior
            for(int i = 0; i < elementos; i++){
                nuevo_buffer[i] = buffer[(inicio + i) % capacidad]; //Revisar los numeros que devuelve la expresion (inicio + i) % capacidad / 2
            }
            //Cambio del puntero
            buffer = nuevo_buffer;
            inicio = 0;
            fin = elementos;
        }

        void Cola_Circular::mostrar(){
            for(int i = 0; i < elementos; i++){
                //std::cout << buffer[(inicio + i) % capacidad/2] << std::endl;
                std::cout << buffer[(inicio + i) % capacidad] << std::endl;
            }
        }