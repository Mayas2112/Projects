
#ifndef MAIN23A2_CPP_HASHTABLE_H
#define MAIN23A2_CPP_HASHTABLE_H
#include <exception>
#include <stdexcept>
#include "List.h"
#include "memory"
#include "Player.h"
class Player;

class dynamicArray{
public:
    List<std::shared_ptr<Player>>** array;
    int arraySize;
    int totalUsed;
    int initialSize;
    bool needToChange;

    explicit dynamicArray(int size);
    ~dynamicArray();
  //  dynamicArray& operator=(dynamicArray& other)=default;
    void add(std::shared_ptr<Player> toAdd,int index);
    List<std::shared_ptr<Player>>* find(int index);

};


class HashTable{
public:
    dynamicArray* array;
    int size;

    explicit HashTable(int arraySize=10);
    ~HashTable()
    {
        delete array;
        array = nullptr;
    }

    void insert(std::shared_ptr<Player> toAdd);
    void enlargeArray();
    int hash(int index);
    std::shared_ptr<Player> find(int index);
    };


#endif //MAIN23A2_CPP_HASHTABLE_H
