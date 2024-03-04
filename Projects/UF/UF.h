#ifndef MAIN23A2_CPP_UF_H
#define MAIN23A2_CPP_UF_H
#include "PlayerNodeUF.h"
#include "TeamIdentifier.h"
#include "AvlTree.h"
#include <memory>
#include "HashTable.h"


/*
class UpSideTree{
    PlayerNodeUF* root;
    UpSideTree(PlayerNodeUF* root);
    ~UpSideTree()=default;
};
*/

class UF {
public:
    AvlTree<int,std::shared_ptr<Team>>* teamsIdTree;
    HashTable* playerTable;


    UF(AvlTree<int,std::shared_ptr<Team>>* teamsTree= nullptr, HashTable* table= nullptr);
    ~UF() = default;

  //  TeamIdentifier* makeSet(int playerId);
    PlayerNodeUF* findNode(int playerId);
    PlayerNodeUF* find(int playerId);
    PlayerNodeUF* unite(PlayerNodeUF* first, PlayerNodeUF* second);

};


#endif //MAIN23A2_CPP_UF_H
