
#include "UF.h"
/*
UpSideTree::UpSideTree(PlayerNodeUF *root):root(root){
}
*/

UF::UF(AvlTree<int, std::shared_ptr<Team>>* teamsTree, HashTable* table):teamsIdTree(teamsTree),playerTable(table)
{
}
/*
TeamIdentifier *UF::makeSet(int playerId) {
    return nullptr;
} //check it later maybe remove it
*/
PlayerNodeUF* UF::findNode(int playerId) {
    Player* player=playerTable->find(playerId).get();
    return player->playerNode;
}

PlayerNodeUF* UF::find(int playerId) { //add the extra for perm and gamesPlayed
    Player* player=playerTable->find(playerId).get();
    PlayerNodeUF* root=player->playerNode;
    PlayerNodeUF* current=player->playerNode;
    PlayerNodeUF* nodeToFixPath=player->playerNode;
    PlayerNodeUF* actualRoot=player->playerNode;
    while (actualRoot->parent)
        actualRoot=actualRoot->parent;
    int sumExtraGP=0;
    int toDecrease=0;
    permutation_t sumPerm= permutation_t::neutral();
    permutation_t toDecreasePerm= permutation_t::neutral();

    while(root->parent)
    {
        if(root != actualRoot){ //fix this
            sumExtraGP+=root->GPExtra;
            sumPerm=root->permExtra * sumPerm;
        }
        root=root->parent; //I changed this
    }
    while(current != root && current->parent != root )
    {
        nodeToFixPath=current;
        current=current->parent;
        nodeToFixPath->parent=root;
        int temp=nodeToFixPath->GPExtra;
        permutation_t tempPerm=nodeToFixPath->permExtra;
        nodeToFixPath->GPExtra=sumExtraGP-toDecrease;
        nodeToFixPath->permExtra=sumPerm * toDecreasePerm.inv(); //sumPerm*toDecreasePerm.inv();
        toDecrease+=temp;
        toDecreasePerm=  tempPerm * toDecreasePerm; //check the order and inverse
    }
    return root;
}

PlayerNodeUF* UF::unite(PlayerNodeUF *first, PlayerNodeUF *second) { //check the team id cuz not always the big buys the small
    if(!first || !second)
        return nullptr;

    if(!second->identifier) //when we add player we use union with one player
    {
        second->parent=first;
        second->permExtra=first->permExtra.inv() * first->identifier->team->m_teamSpirit  * second->permExtra;
      //  second->permExtra= second->permExtra * first->identifier->team->m_teamSpirit * first->permExtra.inv(); //fix it
        second->GPExtra=second->GPExtra - first->GPExtra;
        return first;
    }

    if( first->identifier->teamSize >= second->identifier->teamSize)
    {
        second->parent=first;
        second->permExtra=first->permExtra.inv() * first->identifier->team->m_teamSpirit  * second->permExtra;
        second->GPExtra=second->GPExtra - first->GPExtra;
        first->identifier->teamSize+=second->identifier->teamSize;
    //    delete second->identifier; just changed it to shared :D
        second->identifier = nullptr; // maybe use delete.
        return first;
    }
    else //think here is the problem .
    {
        first->parent=second;
        first->identifier->teamSize+=second->identifier->teamSize;
        first->GPExtra=first->GPExtra - second->GPExtra;
        second->permExtra=first->identifier->team->m_teamSpirit * second->permExtra;
        first->permExtra=second->permExtra.inv() * first->permExtra;

    //    delete second->identifier; // khaled added this! (this fixed 9 errors lmao)
        second->identifier = first->identifier;
        second->identifier->root=second;
        first->identifier = nullptr;
        return second;
    }
}

