#include <iostream>
#include <algorithm>
#include <getopt.h>
#include <fstream>
#include <vector>
using namespace std;

struct Hash {
    int BUCKET;

    vector<vector<int>> table;

    void insertItem(int key, int value) {
        table[key].push_back(value);
    }

    int checkValue(int value){
        for (int i = 0; i < BUCKET; i++)
        {
            if (! this->table[i].empty())
            {
                if(this->table[i].back() == value)
                {
                    return value;
                }
            }
        }
        return -1;
    }

    int checkPosition(int value){
        for (int i = 0; i < BUCKET; i++)
        {
            if (! this->table[i].empty())
            {
                if(this->table[i].back() == value)
                {
                    return i;
                }
            }
        }
        return -1;
    }
    int getLastValue(int key)
    {
        if (! this->table[key].empty())
        {
            return this->table[key].back();
        }
        return -1;
    }

    int isEmpty();
    
    void displayHash();

    Hash(int b) {
        this->BUCKET = b;
        table.resize(BUCKET);
    }
};

int Hash::isEmpty(){
    for (size_t i = 0; i < BUCKET; i++)
    {
        if (getLastValue(i) == -1)
        {
            return i;
        }
    }
    return -1;
}

void Hash::displayHash() {
    for (int i = 0; i < BUCKET; i++) {
        cout << i << "°"; 
        for (int x : table[i]) {
            cout << " --> " << x;
        }
        cout << endl;
    }
}

void fifo(int marcos, vector<int> entrada) {
    int fallos = 0;
    Hash h(marcos);

    for (int i = 0; i < entrada.size(); i++)
    {
        if(h.checkValue(entrada[i]) == -1){
            h.insertItem(fallos % marcos, entrada[i]);
            fallos++;
        }
    }
    cout << "fallos " << fallos << endl;
    h.displayHash();
}

void lru(int marcos = 4, vector<int> entrada = {0, 1, 3, 4, 1, 2, 5, 1, 2, 3, 4}) {
    int fallos = 0;
    vector<int> aux = {};
    Hash h(marcos);
    aux.resize(marcos);

    for (size_t i = 0; i < marcos; i++)
    {
        aux[i] = i;
    }
    
    for (int i = 0; i < entrada.size(); i++)
    {
        if(h.checkValue(entrada[i]) == -1){
            h.insertItem(aux[0], entrada[i]);
            fallos++;
        }
        auto x = find(aux.begin(), aux.end(), h.checkPosition(entrada[i]));
        int m = aux[distance(aux.begin(), find(aux.begin(), aux.end(), h.checkPosition(entrada[i])))];
        aux.erase(x);
        aux.push_back(m);
    }

    cout << "fallos " << fallos << endl;
    h.displayHash();
}

void lruClock(int marcos, vector<int> entrada) {
    int fallos = 0;
    int pointer = 0;

    vector<int> reference = {};
    
    Hash h(marcos);

    reference.resize(marcos);

    for (size_t i = 0; i < marcos; i++)
    {
        reference[i] = 0;
    }
    
    for (int i = 0; i < entrada.size(); i++)
    {
        if(h.checkValue(entrada[i]) == -1){
            while (true)
            {
                if (reference[pointer % marcos] == 0)
                {
                    reference[pointer % marcos] = 0;
                    h.insertItem(pointer % marcos, entrada[i]);
                    pointer += 1;
                    break;
                }
                reference[pointer % marcos] -= 1;
                pointer += 1;
            }
            fallos++;
        }else{
            reference[h.checkPosition(entrada[i])] = 1;
        }
    }
    cout << "fallos " << fallos << endl;
    h.displayHash();
}

void optimo(int marcos, vector<int> entrada) {
    int fallos = 0;
    int pointer = 0;
    int n = 0;

    vector<int> aux = {};
    Hash h(marcos);
    aux.resize(marcos);

    for (size_t i = 0; i < entrada.size(); i++)
    {
        pointer = i;
        n = entrada.size();
        if(h.checkValue(entrada[i]) == -1){

            fallos++;
            if(h.isEmpty() != -1)
            {
                h.insertItem(h.isEmpty(), entrada[i]);
            }
            else{
                for (size_t j = 0; j < marcos; j++)
                    {
                        n = distance(entrada.begin(), find(entrada.begin() + i, entrada.end(), h.getLastValue(j)));
                        if ((n > pointer and n < entrada.size()))
                        {
                            pointer = n;
                        }else if(n < 0 or n >= entrada.size()){
                            n = j;
                            pointer = -1;
                            break;
                        }
                }
                if (pointer > i and pointer < entrada.size()){
                    h.insertItem(h.checkPosition(entrada[pointer]),entrada[i]);
                }else{
                    h.insertItem(n,entrada[i]);
                }
            }
        }
    }
    cout << "fallos " << fallos << endl;
    h.displayHash();
}

vector<int> parser(const string &archivo)
{
    vector<int> entrada = {};
    ifstream archivoIngresado(archivo);
    if (!archivoIngresado.is_open())
    {
        std::cerr << "Error:" << archivo << "no se pudo abrir." << std::endl;
        exit(EXIT_FAILURE);
    }

    int valor;
    while (archivoIngresado >> valor)
    {
        entrada.push_back(valor);
    }
    archivoIngresado.close();
    return entrada;
}

int main(int argc, char **argv) {
    int marcos = 0;
    string algoritmo;
    string archivo;
    vector<int> entrada;

    int opt;
    while ((opt = getopt(argc, argv, "m:a:f:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            marcos = std::stoi(optarg);
            break;
        case 'a':
            algoritmo = optarg;
            break;
        case 'f':
            archivo = optarg;
            break;
        default:
            cerr << "./mvirtual -m <marcos> -a <algoritmo> -f <archivo>" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (marcos <= 0 || algoritmo.empty() || archivo.empty())
    {
        cerr << "Error: datos ingresados incorrectamente" << endl;
        return EXIT_FAILURE;
    }

    entrada = parser(archivo);

    if (algoritmo == "FIFO")
    {
        fifo(marcos, entrada);
    }
    else if (algoritmo == "LRU")
    {
        lru(marcos, entrada);
    }
    else if (algoritmo == "OPTIMO")
    {
        optimo(marcos, entrada);
    }
    else if (algoritmo == "LRURELOJ")
    {
        lruClock(marcos, entrada);
    }
    else
    {
        cerr << "Error: Algoritmo no reconocido. Use FIFO, LRU, LRUR o OPTIMO." << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}