
#include "HashTable.h"

dynamicArray::dynamicArray(int size):array(new List<std::shared_ptr<Player>>*[size]),arraySize(size),
                                        totalUsed(0),initialSize(size),needToChange(false)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = new List<std::shared_ptr<Player>>();
    }
}

dynamicArray::~dynamicArray() {
    for (int i = 0; i < arraySize; ++i)
    {
        delete array[i];
        array[i] = nullptr;
    }
    arraySize = 0;
    delete[] array;
}

void dynamicArray::add(std::shared_ptr<Player> toAdd,int index) {
    if(array[index] == nullptr)
    {
        array[index] = new List<std::shared_ptr<Player>>();
        array[index]->addInFront(toAdd);
        totalUsed++;
        if(totalUsed == arraySize)
        {
            needToChange= true;
        }
    }
    else
    {
        totalUsed++; //check if after all full or after the players is the same size of the table
        array[index]->addInFront(toAdd);
        if(totalUsed == arraySize)
        {
            needToChange= true;
        }
    }
}





List<std::shared_ptr<Player>>* dynamicArray::find(int index) {
    if(index > arraySize || index < 0)
    {
        throw std::out_of_range("invalid Index");
    }

    if(array[index])
    {
        return array[index];
    }
    return nullptr;
}


void HashTable::insert(std::shared_ptr<Player> toAdd) {
    if(toAdd == nullptr)
        return;
    array->add(toAdd, hash(toAdd->id));
    if(array->needToChange)
    {
        enlargeArray();
        array->needToChange= false; // maybe check it??
    }
}

void HashTable::enlargeArray() {
    int oldArraySize=size;
    size=2*size;
    dynamicArray* newArray = new dynamicArray(size);
    for(int i=0;i<oldArraySize;i++)
    {
        if(!array->array[i])
            continue;
        ListNode<std::shared_ptr<Player>>* current=array->array[i]->head;
        while(current)
        {
            newArray->add(current->data, hash(current->data->id));
            current=current->prev;
        }
        /// might be error :D
        //delete array->array[i];

    }
    delete array;
    array=newArray;
}

int HashTable::hash(int index) {
    return index % size;
}

std::shared_ptr<Player> HashTable::find(int index) {
    List<std::shared_ptr<Player>>* playersList=array->array[(hash(index))];
    if(playersList)
    {
        ListNode<std::shared_ptr<Player>>* current = playersList->head;
        while(current && current->data->id != index)
        {
            current=current->prev;
        }
        if(current)
        {
            return current->data;
        } else
            return nullptr;
    } else
        return nullptr;
}

HashTable::HashTable(int arraySize):array(new dynamicArray(arraySize)),size(arraySize)
{
}


