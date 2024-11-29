#include <iostream>
#include <algorithm>
#include <getopt.h>
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
    bool acierto;

    Hash h(marcos);

    for (int i = 0; i < entrada.size(); i++)
    {
        if(h.checkValue(entrada[i]) == -1){
            h.insertItem(fallos % marcos, entrada[i]);
            fallos++;
        }
    }
    cout << fallos << endl;
    h.displayHash();
}

void lru(int marcos = 4, vector<int> entrada = {0, 1, 3, 4, 1, 2, 5, 1, 2, 3, 4}) {
    int fallos = 0;
    int pointer = 0;
    bool acierto;

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

    cout << fallos << endl;
    h.displayHash();
}

void lruclock(int marcos, vector<int> entrada) {
    int fallos = 0;
    int pointer = 0;
    bool acierto;
    bool insert = true;

    vector<int> aux = {};
    vector<int> reference = {};
    
    Hash h(marcos);

    aux.resize(marcos);
    reference.resize(marcos);

    for (size_t i = 0; i < marcos; i++)
    {
        reference[i] = 0;
    }
    
    for (int i = 0; i < entrada.size(); i++)
    {
        insert = true;
        if(h.checkValue(entrada[i]) == -1){
            while (insert)
            {
                if (reference[pointer % marcos] = 0)
                {
                    reference[pointer % marcos] = 0;
                    insert = false;
                    h.insertItem(pointer % marcos, entrada[i]);
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
    cout << fallos << endl;
    h.displayHash();
}

void optimo(int marcos, vector<int> entrada) {
    int fallos = 0;
    int pointer = 0;
    int n = 0;
    bool insert;

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

int main(int argc, char **argv) {
    lruclock(4, {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6});
    lru(4, {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6});
    return 0;
}