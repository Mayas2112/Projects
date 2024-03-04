
#ifndef WORLDCUP23A2_CPP_AVLTREE_H
#define WORLDCUP23A2_CPP_AVLTREE_H
#include <iostream>

template<typename KEY,typename DATA>
class Node{
public:
    KEY key;
    DATA data;
    Node* parent;
    Node* rightSon;
    Node* leftSon;
    int height;
    int Lheight;
    int Rheight;
    int weight;
    int bf;


    Node():key(nullptr),data(nullptr),parent(nullptr),rightSon(nullptr),leftSon(nullptr),height(0),Lheight(0),Rheight(0)
    ,weight(0),bf(0){};
    Node(KEY key,DATA& data):key(key),data(data),parent(nullptr),rightSon(nullptr),leftSon(nullptr),height(0),Lheight(0),
    Rheight(0),weight(0),bf(0){}
    ~Node()=default;
    void updateBalanceFactor();
    void balanceFactor();
    void fixHeight();
    void rotateLL();
    void rotateRR();
    void rotateLR();
    void rotateRL();
    void print2DUtil(Node<KEY,DATA>* root, int space);
    void print2D(Node<KEY,DATA>* root);
    Node<KEY,DATA>* findNextInOrder();
    Node<KEY,DATA>* findPreviousInOrder();
};

template<class KEY, class DATA>
void Node<KEY,DATA>::print2DUtil(Node<KEY,DATA>* root, int space)
{
    // Base case
    if (root == nullptr)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->rightSon, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->leftSon, space);
}

// Wrapper over print2DUtil()
template<class KEY, class DATA>
void Node<KEY,DATA>::print2D(Node<KEY,DATA>* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}



template<typename KEY, typename DATA>
void Node<KEY, DATA>::updateBalanceFactor() {
        bf=Lheight-Rheight;
}

template<typename KEY, typename DATA>
void Node<KEY , DATA>::rotateLL(){
    Node<KEY,DATA>* A=this->leftSon;
    Node<KEY,DATA>* C=A->rightSon;

    if(C== nullptr && this->rightSon== nullptr)
        this->weight = 1;
    else if(C== nullptr && this->rightSon)
        this->weight = this->rightSon->weight + 1;
    else if (C && this->rightSon==nullptr)
        this->weight = C->weight + 1;
    else
        this->weight= this->rightSon->weight + C->weight + 1;

    if(A->leftSon) {
        A->weight = A->leftSon->weight + this->weight + 1;
    }
    else
        A->weight= this->weight +1;


    A->rightSon=this;
    A->parent=this->parent;
    if(C)
    {
        C->parent = this;
        this->leftSon = C;
    }
    else
        this->leftSon = nullptr;
    Node<KEY,DATA>* D=this->parent;
    this->parent=A;
    if(D != nullptr) {
        if (D->rightSon == this) {
            D->rightSon = A;
        } else {
            D->leftSon = A;
        }
    }
    fixHeight();
    updateBalanceFactor();
}

template<typename KEY, typename DATA>
void Node<KEY , DATA>::rotateRR(){
    Node<KEY,DATA>* A=this->rightSon;
    Node<KEY,DATA>* C=A->leftSon;

    if(C== nullptr && this->leftSon== nullptr)
        this->weight = 1;
    else if(C== nullptr && this->leftSon)
        this->weight = this->leftSon->weight + 1;
    else if (C && this->leftSon==nullptr)
        this->weight = C->weight + 1;
    else
        this->weight= this->leftSon->weight + C->weight + 1;

    if(A->rightSon) {
        A->weight = A->rightSon->weight + this->weight + 1;
    }
    else
        A->weight= this->weight +1;


    A->leftSon=this;
    A->parent=this->parent;
    if(C) {
        C->parent = this;
        this->rightSon = C;
    }
    else
        this->rightSon= nullptr;
    Node<KEY,DATA>* D=this->parent;
    this->parent=A;
    if(D != nullptr) {
        if (D->rightSon == this) {
            D->rightSon = A;
        } else {
            D->leftSon = A;
        }
    }
    fixHeight();
    updateBalanceFactor();
}

template<typename KEY, typename DATA>
void Node<KEY , DATA>::rotateLR(){
    this->leftSon->rotateRR();
    rotateLL();
}

template<typename KEY, typename DATA>
void Node<KEY , DATA>::rotateRL(){
    this->rightSon->rotateLL();
    rotateRR();
}

template<typename KEY, typename DATA>
void Node<KEY, DATA>::fixHeight() {
 /*   if(rightSon == nullptr && leftSon == nullptr)
    {
        height=0;
        Rheight=0;
        Lheight=0;
        return;
    }*/
    if(rightSon)
    {
        Rheight=rightSon->height;
    } else Rheight=0;
    if(leftSon)
    {
        Lheight=leftSon->height;
    } else Lheight=0;
    if(Rheight>Lheight)
    {
        height=Rheight+1;
    } else height=Lheight+1;
}

template<typename KEY, typename DATA>
void Node<KEY, DATA>::balanceFactor() {
    //like the table we have seen in the lec.

    if(bf==2 && leftSon->bf==-1)
    {
        rotateLR();
    }
    if(bf==2 && leftSon->bf>-1)
    {
        rotateLL();
    }
    if(bf==-2 && rightSon->bf==1)
    {
        rotateRL();
    }
    if(bf==-2 && rightSon->bf<1)
    {
        rotateRR();
    }
}

template<typename KEY, typename DATA>
Node<KEY, DATA>* Node<KEY, DATA>::findNextInOrder() {
    Node<KEY, DATA> *current= nullptr;
    if(this->rightSon != nullptr)
    {
        current = this->rightSon;
        while (current!= nullptr && current->leftSon != nullptr) {
            current = current->leftSon;
        }
        return current;
    }
    current= this;
    Node<KEY, DATA>* currentParent=current->parent;
    while (currentParent!=nullptr && current==currentParent->rightSon)
    {
        current=currentParent;
        currentParent=currentParent->parent;
    }
    return currentParent;
}

template<typename KEY, typename DATA>
Node<KEY, DATA>* Node<KEY, DATA>::findPreviousInOrder() {
    Node<KEY, DATA>* current= nullptr;
    if(this->leftSon != nullptr)
    {
        current = this->leftSon;
        while (current!= nullptr && current->rightSon != nullptr) {
            current = current->rightSon;//left
        }
        return current;
    }
    current= this;
    Node<KEY,DATA>* currentParent=current->parent;
    while (currentParent!=nullptr && current==currentParent->leftSon)
    {
        current=currentParent;
        currentParent=currentParent->parent;
    }
    return currentParent;
}

template<typename KEY, typename DATA>
class AvlTree {
public:
        Node<KEY,DATA>* root;
        Node<KEY,DATA>* min;
        Node<KEY,DATA>* max;
        int treeSize;

        AvlTree();
        AvlTree(Node<KEY,DATA>* root);
        Node<KEY,DATA>* find(KEY keyToFind);
        Node<KEY,DATA>* findAux(KEY keyToFind,Node<KEY,DATA>* current);
        void insert(KEY keyToAdd,DATA data);
        void insertToTree(Node<KEY,DATA>* root,Node<KEY,DATA>* toAdd);
        Node<KEY,DATA>* select(int rank);
        Node<KEY,DATA>* selectAux(int rank , Node<KEY,DATA>* current);
        void balanceTree(Node<KEY,DATA>* current);
        void removeLeaf(Node<KEY,DATA>* leaf);
        void nodeWithOneChildRemover(Node<KEY,DATA>* node);
        void nodeWithTwoChildrenRemover(Node<KEY,DATA>* node);
        void remove(KEY toDeleteKey);
        void deleteTree();
        void nullify(Node<KEY,DATA>* current);
        void print();
        Node<KEY,DATA>** inOrder();
        int inOrderAux(Node<KEY,DATA>** array,Node<KEY,DATA>* current,int& index, int arraySize);
        int getCount(Node<KEY,DATA>* root, KEY min, KEY max);
        KEY* inOrderInRange(KEY min,KEY max,bool returnKeys);
        void inOrderInRangeAux(KEY* array, Node<KEY, DATA>* current,int& index, int arraySize,KEY min,KEY max,bool returnKeys);
    };

template<typename KEY, typename DATA>
AvlTree<KEY, DATA>::AvlTree():root(nullptr),min(nullptr),max(nullptr),treeSize(0){}

template<typename KEY, typename DATA>
AvlTree<KEY, DATA>::AvlTree(Node<KEY, DATA> *root):root(root),min(root),max(root),treeSize(1){
}

template<typename KEY, typename DATA>
Node<KEY,DATA>* AvlTree<KEY, DATA>::find(KEY keyToFind){
    if(treeSize==0)
        return nullptr;
    Node<KEY,DATA>* current=root;
    return findAux(keyToFind,current);
}

template<typename KEY, typename DATA>
Node<KEY, DATA> *AvlTree<KEY, DATA>::findAux(KEY keyToFind, Node<KEY, DATA>* current) {
    if(!current)
        return nullptr;
    if(current->key==keyToFind)
        return current;
    if(current->key>keyToFind)
        return findAux(keyToFind,current->leftSon);
    else
        return findAux(keyToFind,current->rightSon);
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::insert(KEY keyToAdd, DATA data) {
    if(find(keyToAdd) != nullptr) return; //already exists
    if(!root) //empty tree
    {
        root=new Node<KEY,DATA>(keyToAdd,data);
        root->parent= nullptr;
        treeSize++;
        min=root;
        max=root;
        root->weight=1;
    }
    else{
        Node<KEY,DATA>* toAdd=new Node<KEY,DATA>(keyToAdd,data);
        if(min->key>keyToAdd)
            min=toAdd;
        if(max->key<keyToAdd)
            max=toAdd;
        insertToTree(root,toAdd);
        balanceTree(toAdd);
        treeSize++;
    }
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::insertToTree(Node<KEY, DATA>* current,Node<KEY, DATA>* toAdd)
{
    if(toAdd->key>current->key)
    {
        if(current->rightSon== nullptr)
        {
            current->rightSon=toAdd;
            toAdd->parent=current;
            current->weight++;
            toAdd->weight++;
        }
        else{
            current->weight++;
            insertToTree(current->rightSon,toAdd);
        }
    } else{
        if(current->leftSon== nullptr)
        {
            current->leftSon=toAdd;
            toAdd->parent=current;
            current->weight++;
            toAdd->weight++;
        }
        else
        {
            current->weight++;
            insertToTree(current->leftSon,toAdd);
        }
    }
}

template<typename KEY, typename DATA>
Node<KEY,DATA>* AvlTree<KEY, DATA>::select(int rank){
    if( rank > treeSize || rank <  1) // there is no index bigger than the tree size && there's no 0 index
    {
        return nullptr;
    }
    return selectAux(rank , root);
}

template<typename KEY, typename DATA>
Node<KEY,DATA>* AvlTree<KEY, DATA>::selectAux(int rank , Node<KEY,DATA>* current) {
    int leftSonWeight=0;
    if(current->leftSon)
        leftSonWeight=current->leftSon->weight;
    if( leftSonWeight==rank-1 ) //current->leftSon == nullptr && ((rank-1==0) && (current->leftSon == nullptr)) ||
        return current;
    else if(leftSonWeight > rank-1)
        return selectAux(rank, current->leftSon);
    else
        return selectAux(rank - leftSonWeight - 1 ,current->rightSon);
    /*
    if(((rank-1==0) && (current->leftSon == nullptr)) || current->leftSon->weight==rank-1 ) //current->leftSon == nullptr &&
        return current;
    else if(current->leftSon->weight > rank-1)
        return selectAux(rank, current->leftSon);
    else if(current->leftSon->weight < rank-1)
        return selectAux(rank - current->leftSon->weight - 1 ,current->rightSon);
    else
        return nullptr;*/
}


template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::balanceTree(Node<KEY,DATA>* current) {
    if(current== nullptr)
        return;
    if(current->parent== nullptr)
        root=current;
    current->fixHeight();
    current->updateBalanceFactor();
    current->balanceFactor();
    balanceTree(current->parent);
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::nodeWithOneChildRemover(Node<KEY,DATA>* node) {
    Node<KEY,DATA>* nodeParent=node->parent;
    if(node->leftSon == nullptr)
    {
        if(min == node)
            min=node->findNextInOrder();
        if(!nodeParent){
            root=node->rightSon;
            root->parent= nullptr;
            delete node;
            treeSize--;
        }
        else
        {
            if(nodeParent->leftSon==node)
            {
                nodeParent->leftSon=node->rightSon;
                node->rightSon->parent=nodeParent;
                delete node;
                treeSize--;
            }
            else
                {
                    nodeParent->rightSon=node->rightSon;
                    node->rightSon->parent=nodeParent;
                    delete node;
                    treeSize--;
            }
            balanceTree(nodeParent);
        }
    }else{
        if(max == node)
            max=node->findPreviousInOrder();
        if(!nodeParent) {
            root = node->leftSon;
            root->parent= nullptr;
            delete node;
            treeSize--;
        }
        else{
            if(nodeParent->leftSon==node)
            {
                nodeParent->leftSon=node->leftSon;
                node->leftSon->parent=nodeParent;
                delete node;
                treeSize--;
            }
            else
            {
                nodeParent->rightSon=node->leftSon;
                node->leftSon->parent=nodeParent;
                delete node;
                treeSize--;
            }
            balanceTree(nodeParent);
        }
    }
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::removeLeaf(Node<KEY, DATA> *leaf) {
    Node<KEY,DATA>* leafParent=leaf->parent;
    if(!leafParent)
    {
        root= nullptr;
        max= nullptr;
        min= nullptr;
        treeSize=0;
        delete leaf;
    } else {
        if(leaf == min)
        {
            min=leaf->parent;
        }
        if(leaf == max)
        {
            max=leaf->parent;
        }
        if(leafParent->leftSon == leaf){
            leafParent->leftSon = nullptr;
        }
        else{
            leafParent->rightSon = nullptr;
        }
        delete leaf;
        balanceTree(leafParent);
        treeSize--;
    }
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::nodeWithTwoChildrenRemover(Node<KEY, DATA> *node) {
    Node<KEY,DATA>* nextInOrder=node->findNextInOrder();
    //now it have only one son,so we swap the two nodes
    KEY keyNextInOrder=nextInOrder->key;
    DATA dataNextInOrder=nextInOrder->data;
    KEY keyNode=node->key;
    DATA dataNode=node->data;
    nextInOrder->key=keyNode;
    nextInOrder->data=dataNode;
    node->data=dataNextInOrder;
    node->key=keyNextInOrder;
    Node<KEY,DATA>* nextInOrderTemp=nextInOrder;
    while(nextInOrderTemp != node )
    {
        nextInOrderTemp->weight--;
        nextInOrderTemp=nextInOrderTemp->parent;
    }


    //swaping

    /*if(toDeleteRightSon==nextInOrder)
    {
        nextInOrder->leftSon=toDeleteLeftSon;
        nextInOrder->parent=toDeleteParent;

        node->rightSon=nextInOrderRightSon;
        node->leftSon= nullptr;

        nextInOrder->rightSon=node;
        node->parent=nextInOrder;
    }
    else if(toDeleteRightSon == nextInOrderParent)
    {
        nextInOrder->leftSon=toDeleteLeftSon;
        nextInOrder->parent=toDeleteParent;

        node->rightSon=nextInOrderRightSon;
        node->leftSon= nullptr;

        nextInOrder->rightSon=nextInOrderParent;
        node->parent=toDeleteRightSon;


        nextInOrderParent->parent=nextInOrder;
        nextInOrderParent->leftSon=node;

    }
    else
    {
        nextInOrder->leftSon=toDeleteLeftSon;
        nextInOrder->parent=toDeleteParent;

        node->rightSon=nextInOrderRightSon;
        node->leftSon= nullptr;

        nextInOrder->rightSon=toDeleteRightSon;
        node->parent=nextInOrderParent;

        toDeleteRightSon->parent=nextInOrder;
        toDeleteLeftSon->parent=nextInOrder;
        if(toDeleteParent)
        {
            if(toDeleteParent->rightSon=node)
            {
                toDeleteParent->rightSon=nextInOrder;
            }
            else
                toDeleteParent->leftSon=nextInOrder;
        }


    }*/
    if(nextInOrder->rightSon || nextInOrder->leftSon)
    {
        nodeWithOneChildRemover(nextInOrder);
    } else removeLeaf(nextInOrder);

}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::remove(KEY toDeleteKey) {

    Node<KEY,DATA>* current= find(toDeleteKey);
    Node<KEY,DATA>* toDelete=current;
    if(current==root && treeSize == 1 )
    {
        current->parent= nullptr;
    }
    if(toDelete== nullptr)
    {
        return;
    }
    while(current)
    {
        current->weight--;
        current=current->parent;
    }
    if(toDelete->rightSon ==nullptr && toDelete->leftSon==nullptr)
    {
        removeLeaf(toDelete);
    }
    else if(toDelete->rightSon ==nullptr || toDelete->leftSon==nullptr)
    {
        nodeWithOneChildRemover(toDelete);
    }
    else
         nodeWithTwoChildrenRemover(toDelete);
}

template<typename KEY, typename DATA>
Node<KEY,DATA>** AvlTree<KEY,DATA>::inOrder()
{
    int index=0;
    Node<KEY,DATA>** inOrderArray= new Node<KEY,DATA>*[treeSize]();
    inOrderAux(inOrderArray,root,index,treeSize);
    return inOrderArray;
}

template<typename KEY, typename DATA>
int AvlTree<KEY, DATA>::inOrderAux(Node<KEY,DATA>** array, Node<KEY, DATA>* current,int& index, int arraySize) {
    if(!current){
        return index;
    }
    index= inOrderAux(array,current->leftSon,index,arraySize);
    if(index>=arraySize){
        return index;
    }
    array[index]=current;
    index++;
    return inOrderAux(array,current->rightSon,index,arraySize);
}

template<typename KEY, typename DATA>
KEY* AvlTree<KEY,DATA>::inOrderInRange(KEY min,KEY max,bool returnKeys)
{
    int index=0;
    int size=getCount(root,min,max);
    KEY* inOrderArray= new DATA[size]();
    inOrderInRangeAux(inOrderArray,root,index,size,min,max,returnKeys);
    return inOrderArray;
}

template<typename KEY, typename DATA>
void AvlTree<KEY ,DATA>::inOrderInRangeAux(KEY* array, Node<KEY, DATA>* current,int& index, int arraySize,KEY min,KEY max,
                                       bool returnKeys) {
    if(!current || arraySize<=index){
        return;
    }
    if(current->key==max && current->key==min)
    {
        if(returnKeys)
        {
            KEY key=current->key;
            array[index]=key;
        }
        else
        {
            DATA data=current->data;
            array[index]=data;
        }
        return;
    }
    if(current->key<=max && current->key>=min)
    {
        //index++;
        inOrderInRangeAux(array,current->leftSon,index,arraySize,min,max,returnKeys);
        if(returnKeys)
        {
            KEY key=current->key;
            array[index]=key;
        }
        else
        {
            DATA data=current->data;
            array[index]=data;
        }
        index++;
        inOrderInRangeAux(array,current->rightSon,index,arraySize,min,max,returnKeys);
    }
    else{
        if(current->getKey()<min){
            inOrderInRangeAux(array,current->rightSon,index,arraySize,min,max,returnKeys);
        } else{
            inOrderInRangeAux(array,current->leftSon,index,arraySize,min,max,returnKeys);
        }
    }
}


template<typename KEY,typename DATA>
int AvlTree<KEY,DATA>::getCount(Node<KEY,DATA>* root, KEY min, KEY max)
{
    if (!root) return 0;
    if (root->key== max && root->key == min)
        return 1;

    if (root->key <= max && root->key >= min){
        return 1 + getCount(root->leftSon, min, max) + getCount(root->rightSon, min, max);
    }
    else{
        if (root->key < min){
            return getCount(root->rightSon, min, max);
        }
        else
        {
            return getCount(root->leftSon, min, max);}
    }
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::print() {
    root->print2D(root);
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::deleteTree() {
    nullify(root);
    root= nullptr;
}

template<typename KEY, typename DATA>
void AvlTree<KEY, DATA>::nullify(Node<KEY,DATA>* current) {
    if(!current)
    {
        return;
    }
    nullify(current->leftSon);
    nullify(current->rightSon);
    delete current;
}

#endif //WORLDCUP23A2_CPP_AVLTREE_H
