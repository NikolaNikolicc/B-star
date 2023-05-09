#include "tree.h"

Node::Node(int numOfPointers) {
    numOfKeys = 0;
    parent = nullptr;
    keys = new string[numOfPointers - 1];
    next = new Node * [numOfPointers];
    for (int i = 0; i < numOfPointers; i++) {
        next[i] = nullptr;
    }
}

Node::~Node() {
    //ovde bi se javio rekurzivni poziv
    /*for (int i = 0; i < numOfPointers; i++) {
        if (next[i]) {
            delete next[i];
        }
    }*/
    delete[] next;
    delete[] keys;
    parent = nullptr;
}

int roundUp(double x) {
    double tmp = x;
    while (tmp > 0) {
        tmp -= 1;
    }
    return x - tmp;
}

int roundDown(double x) {
    int tmp = x;
    return tmp;
}
//pre konstruktora mora se proveriti 2 < m < 11
Tree::Tree(int dimension) {
    m = dimension;
    //ova dva su max i min br pokazivaca
    rootMax = 2 * roundDown((2 * (double)m - 2) / 3) + 1;
    nodeMin = roundUp((2 * (double)m - 1) / 3);

    //pravimo stablo sa sve korenom
    root = new Node(rootMax);
}

bool isLeaf(Node*& n) {
    if (n->next[0])return false;
    else return true;
}

bool isRoot(Node*& n, Tree& t) {
    if (n->parent == t.root) return true;
    else return false;
}

ostream& operator<<(ostream& os, const Tree& t) {
    queue<Node*> Q;
    Node* emptyNode = new Node(2);//korisnik ne sme da pravi cvor reda dva
    emptyNode->keys[0] = "Ovo je cvor za novi red";
    emptyNode->numOfKeys++;
    Q.push(t.root);
    Q.push(emptyNode);
    int indIfPreviousWasEmpty = 0;
    while (!Q.empty()) {
        Node* tmp = Q.front();
        Q.pop();

        //ovaj deo sluzi za ispis novog reda
        if (tmp->keys[0] == "Ovo je cvor za novi red") {
            cout << endl;
            Q.push(tmp);
            if (!indIfPreviousWasEmpty) {
                indIfPreviousWasEmpty = 1;
                continue;
            }
            else return os;
        }
        else indIfPreviousWasEmpty = 0;

        //ispisi cvor tmp
        cout << " (";
        if (tmp->next[0]) {
            Q.push(tmp->next[0]);
        }

        for (int i = 0; i < tmp->numOfKeys; i++) {
            cout << tmp->keys[i];
            if (i != tmp->numOfKeys - 1)cout << " ";
            if (tmp->next[i + 1]) {
                Q.push(tmp->next[i + 1]);
            }

        }
        cout << ") ";

    }
}

Node*& findNodeWhereToInsert(Node*& tmp, string s) {
    while (!isLeaf(tmp)) {
        if (s.compare(tmp->keys[0]) < 0) {
            tmp = tmp->next[0];
            continue;
        }
        for (int i = 0; i <= tmp->numOfKeys; i++) {
            //u slucaju i == tmp->numOfKeys idemo u desno podstablo, u ostalim sluc i levo podstablo
            if (i == tmp->numOfKeys) {
                tmp = tmp->next[i];
                break;
            }
            else if (s.compare(tmp->keys[i]) == -1) {
                tmp = tmp->next[i];
                break;
            }
        }
    }
    return tmp;
}

//ukoliko je van vraca numOfKeys, a ako ne vraca poziciju na kojoj treba da se nalazi string u cvoru
int findPositionOfString(Node*& tmp, string s) {
    int i = 0;
    while (i < tmp->numOfKeys&& tmp->keys[i].compare(s) <= 0) {
        i++;
    }
    return i;
}

void shiftRightFromPosition(int position, Node*& tmp) {
    for (int j = tmp->numOfKeys; j > position; j--) {
        tmp->keys[j] = tmp->keys[j - 1];
        tmp->next[j + 1] = tmp->next[j];
    }
}

void shiftRightFromPositionWithSize(int position, int size, Node*& tmp) {
    for (int j = tmp->numOfKeys; j > position; j--) {
        if(j != size - 1)tmp->keys[j] = tmp->keys[j - 1];
        tmp->next[j + 1] = tmp->next[j];
    }
}

void shiftRightFromPositionNormal(int position, Node*& tmp) {
    for (int j = tmp->numOfKeys - 1; j > position; j--) {
        tmp->keys[j] = tmp->keys[j - 1];
        tmp->next[j + 1] = tmp->next[j];
    }
    tmp->next[position + 1] = tmp->next[position];
}

void shiftLeftFromPosition(int position, Node* tmp) {
    for (int j = position; j < tmp->numOfKeys; j++) {
        if (j < tmp->numOfKeys - 1)tmp->keys[j] = tmp->keys[j + 1];
        tmp->next[j] = tmp->next[j + 1];
    }
}


void shiftLeftUntilPosition(int position, Node* tmp) {
    if (position > tmp->numOfKeys)position = tmp->numOfKeys;
    for (int i = 0; i < position; i++) {
        if (i < position - 1) tmp->keys[i] = tmp->keys[i + 1];
        tmp->next[i] = tmp->next[i + 1];
    }
    tmp->numOfKeys--;
}

void insertStringInNode(Node*& tmp, string s) {
    int i = findPositionOfString(tmp, s);
    //pomeramo sve za jednu poziciju
    shiftRightFromPosition(i, tmp);
    //umecemo na nadjenu poziciju
    tmp->keys[i] = s;
    tmp->numOfKeys++;
    tmp->next[i + 1] = nullptr;
}

//void insertStringInNodeFraction(Node*& tmp, string s, int size) {
//    int i = findPositionOfString(tmp, s);
//    //pomeramo sve za jednu poziciju
//    shiftRightFromPositionNormal(i - 1, tmp);
//    //umecemo na nadjenu poziciju
//    tmp->keys[i] = s;
//    tmp->numOfKeys++;
//    tmp->next[i + 1] = nullptr;
//}

void insertStringInNodeFraction(Node*& tmp, string s, int size) {
    Node* zeroPtr = tmp->next[0];
    insertStringInNode(tmp, s);
    tmp->next[1] = zeroPtr;
}

bool equal(Node* n1, Node* n2) {
    for (int i = 0; i < n1->numOfKeys; i++) {
        if (n1->keys[i] != n2->keys[i])return false;
    }
    return true;
}

int findPositionOfNodeInParent(Node*& tmp) {
    Node* parentNode = tmp->parent;
    for (int i = 0; i <= parentNode->numOfKeys; i++) {
        if (/*parent->next[i] == tmp*/equal(parentNode->next[i], tmp)) {
            return i;
        }
    }
    //return -1;
}

bool overflowRightPossible(Node*& tmp, Tree& t) {
    //za slucaj korena stabla
    if (!tmp->parent)return false;

    int i = findPositionOfNodeInParent(tmp);
    if (/*tmp->parent->next[i + 1]*/i < tmp->parent->numOfKeys && /*ako postoji desni*/
        tmp->parent->next[i + 1]->numOfKeys < t.m - 1) return true;
    else return false;
}

bool overflowLeftPossible(Node*& tmp, Tree& t) {
    //za slucaj korena stabla
    if (!tmp->parent)return false;

    int i = findPositionOfNodeInParent(tmp);
    //cout << i;
    if (i > 0 && /*ako postoji levi*/
        tmp->parent->next[i - 1]->numOfKeys < t.m - 1) return true;
    else return false;
}

void Tree::overflow(Node*& tmp, int i) { //i == 1 desni overflow, i == -1 levi overflow
    int pos = findPositionOfNodeInParent(tmp);
    if (i == 1) {
        insertStringInNode(tmp->parent->next[pos + 1], tmp->parent->keys[pos]);
        tmp->parent->keys[pos] = tmp->keys[tmp->numOfKeys - 1];
        tmp->next[tmp->numOfKeys] = nullptr;
    }
    //i = -1
    else {
        insertStringInNode(tmp->parent->next[pos - 1], tmp->parent->keys[pos - 1]);
        tmp->parent->keys[pos - 1] = tmp->keys[0];
        //pomeramo next i keys niz
        shiftLeftFromPosition(0, tmp);
    }
    //na kraju smanjujemo broj kljuceva u cvoru
    tmp->numOfKeys--;
}

void formNode(Node*& node, Node*& copyFrom1, Node* copyFrom2, string splitter, string s) {
    int k = 0;

    node->next[0] = copyFrom1->next[0];
    for (int i = 0; i < copyFrom1->numOfKeys; i++) {
        node->keys[k] = copyFrom1->keys[i];
        node->next[++k] = copyFrom1->next[i + 1];
        node->numOfKeys++;
    }
    node->next[k] = copyFrom2->next[0];
    node->keys[k++] = splitter;
    node->numOfKeys++;

    for (int i = 0; i < copyFrom2->numOfKeys; i++) {
        node->keys[k] = copyFrom2->keys[i];
        node->next[++k] = copyFrom2->next[i + 1];
        node->numOfKeys++;
    }

}

void writeNode(Node*& writeNode, Node*& fromNode, int from, int to, int k = 0, int indResetNumOfKeys = 1) {
    //int k = 0;
    if (indResetNumOfKeys)writeNode->numOfKeys = 0;
    writeNode->next[k] = fromNode->next[from];
    for (int i = from; i < to; i++) {
        writeNode->keys[k] = fromNode->keys[i];
        writeNode->next[++k] = fromNode->next[i + 1];
        writeNode->numOfKeys++;
    }
    for (int i = k + 1; i < writeNode->numOfKeys; i++) {
        writeNode->next[i] = nullptr;
        //writeNode->numOfKeys++;
    }

}

void insertInRoot(Node*& root, string s, Node*& carry, int& indEndOfWhile) {
    int pos = findPositionOfString(root, s);
    insertStringInNode(root, s);
    root->next[pos + 1] = carry;
    carry->parent = root;
    indEndOfWhile = 1;
}

void breakRoot(Node* root, string s, Node*& carry, int& indEndOfWhile, Tree& t) {
    Node* big = new Node(t.rootMax + 1);
    writeNode(big, root, 0, root->numOfKeys);
    int pos = findPositionOfString(big, s);
    insertStringInNode(big, s);
    big->next[pos + 1] = carry;
    int positionOfCarry = pos + 1;

    int mid = roundUp(((double)big->numOfKeys) / 2.) - 1;
    //ako je prethodio desni prelom onda lSon = root, ako je prethodio levi prelom rSon = root
    Node* newRoot = new Node(t.rootMax), * rSon = root, * lSon = new Node(t.m);
    t.root = newRoot;

    for (int i = 0; i <= mid; i++) {
        if(big->next[i])big->next[i]->parent = lSon;
    }
    for (int i = mid + 1; i <= big->numOfKeys; i++) {
        if(big->next[i])big->next[i]->parent = rSon;
    }

    delete[] rSon->next;
    delete[] rSon->keys;
    rSon->next = new Node * [t.m];
    rSon->keys = new string[t.m - 1];

    insertStringInNode(newRoot, big->keys[mid]);
    newRoot->next[0] = lSon; lSon->parent = newRoot;
    newRoot->next[1] = rSon; rSon->parent = newRoot;

    //parent za carry
    if (carry) {
        if (positionOfCarry <= mid)carry->parent = lSon;
        else carry->parent = rSon;
    }

    writeNode(lSon, big, 0, mid);
    writeNode(rSon, big, mid + 1, big->numOfKeys);

    indEndOfWhile = 1;
    delete big;
}

Node* makeBig(Node*& lNode, Node*& rNode, string s, Node*& carry, int& positionOfCarry) {
    Node* big = new Node(lNode->numOfKeys + 1 + rNode->numOfKeys + 1 + 1);//m + m + 1

    //prepisujemo lNode
    writeNode(big, lNode, 0, lNode->numOfKeys);
    //dodajemo razdelni kljuc
    int splitter = findPositionOfNodeInParent(lNode);
    insertStringInNode(big, lNode->parent->keys[splitter]); // dodaje se na kraj jer je u pitanju razdelni cvor
    //prepisujemo rNode
    writeNode(big, rNode, 0, rNode->numOfKeys, lNode->numOfKeys + 1, 0);
    //dodajemo string i setujemo pokazivac
    int pos = findPositionOfString(big, s);
    insertStringInNode(big, s);
    big->next[pos + 1] = carry;

    positionOfCarry = pos + 1;
    return big;
}

void overflowFraction(Node* parentNode, Node* thirdNode, string s, int position, int size, int right) {
    if (right) {
        //razdelnik
        int splitter = findPositionOfNodeInParent(parentNode);
        //nadjemo desnog brata
        Node* rBro = parentNode->parent->next[splitter + 1];
        //spustimo splitter
        insertStringInNodeFraction(rBro, parentNode->parent->keys[splitter], size);
        //preuzima se pokazivac
        rBro->next[0] = parentNode->next[parentNode->numOfKeys];
        parentNode->next[parentNode->numOfKeys]->parent = rBro;
        //pregazimo splitter
        parentNode->parent->keys[splitter] = parentNode->keys[parentNode->numOfKeys - 1];
        //shiftRight 
        shiftRightFromPositionWithSize(position, size, parentNode);
        parentNode->keys[position] = s;
        parentNode->next[position + 1] = thirdNode;
        if(thirdNode)thirdNode->parent = parentNode;
        //parentNode->numOfKeys; - nema potreba azurirati jer u shiftRightu ne dekrementiramo
    }
    else {
        //razdelnik
        int splitter = findPositionOfNodeInParent(parentNode);
        //nadjemo levog brata
        Node* lBro = parentNode->parent->next[splitter - 1];
        //spustamo splitter
        insertStringInNode(lBro, parentNode->parent->keys[splitter - 1]);
        //preuzima se pokazivac 
        lBro->next[lBro->numOfKeys] = parentNode->next[0];
        parentNode->next[0]->parent = lBro;
        //pregazimo splitter
        parentNode->parent->keys[splitter - 1] = parentNode->keys[0];
        //shiftLeft
        shiftLeftUntilPosition(position, parentNode);
        //sad u njegov position stavi string s
        parentNode->keys[position - 1] = s;
        parentNode->next[position] = thirdNode;
        if(thirdNode)thirdNode->parent = parentNode;
        parentNode->numOfKeys++;
    }
}

string breakNode(Node*& lNode, Node*& rNode, string s, Node*& carry,
    int& indCarry, int& indEndOfWhile, int first, int second, int third, Tree& t, int breakRight) {
    int positionOfCarry = -1;
    Node* big = makeBig(lNode, rNode, s, carry, positionOfCarry), * thirdNode = new Node(lNode->numOfKeys + 1); // reda m

    writeNode(lNode, big, 0, first);
    writeNode(rNode, big, first + 1, second);
    writeNode(thirdNode, big, second + 1, big->numOfKeys);

    //parent za carry
    if (carry) {
        if (positionOfCarry <= first) carry->parent = lNode;
        else if (positionOfCarry <= second) carry->parent = rNode;
        else carry->parent = thirdNode;
    }

    //proveri da li staje u oca, ako ima mesta smestamo big->next second, ako ne saljemo ga kao pripremu za dalje prelome
    Node* parent = lNode->parent;

    int positionForInsert = findPositionOfNodeInParent(lNode);
    parent->keys[positionForInsert] = big->keys[first];

    if (parent == t.root && parent->numOfKeys < t.rootMax - 1 ||
        parent != t.root && parent->numOfKeys < t.m - 1) {
        int pos = findPositionOfString(parent, big->keys[second]);
        insertStringInNode(parent, big->keys[second]);
        parent->next[pos + 1] = thirdNode;
        thirdNode->parent = parent;
        indCarry = 0;
        indEndOfWhile = 1;
    }
    else if (overflowRightPossible(parent, t)) {
        s = big->keys[second];
        int position = findPositionOfString(parent, big->keys[second]);

        overflowFraction(parent, thirdNode, s, position, t.m, 1);

        indCarry = 0;
        indEndOfWhile = 1;
    }
    else if (overflowLeftPossible(parent, t)) {
        //prvo nadji gde bi big->keys[second] stajao u ocu 
        s = big->keys[second];
        int position = findPositionOfString(parent, s);

        overflowFraction(parent, thirdNode, s, position, t.m, 0);

        indCarry = 0;
        indEndOfWhile = 1;
    }
    else {
        carry = thirdNode; //- nema potrebe jer svakako vracamo thirdNode
        lNode->parent = parent;
        rNode->parent = parent;
        s = big->keys[second];
        indCarry = 1;
        indEndOfWhile = 0;
    }


    return s;
}

void Tree::fracture(Node*& nodeStart, string s) {
    int indCarry = 0, indEndOfLoop = 0;
    Node* tmp = nullptr, * thirdNode = nullptr;

    int first = roundDown((2 * (double)m - 2) / 3);
    int second = roundDown((2 * (double)m - 1) / 3);
    second = first + second + 1;
    int third = roundDown((2 * (double)m) / 3);

    tmp = nodeStart;

    while (!indEndOfLoop) {
        if (tmp == root) {
            //smestamo u cvor
            //npr rootMax = 5 indeksi su 0, 1, 2, 3, 4, numOfkeys < 4 i to je vrednost preko koje se ne ide u slucaju dodavanja
            Node* carry = (indCarry) ? thirdNode : nullptr;
            if (tmp->numOfKeys < rootMax - 1) {
                //numOfKeys = index najdesnjeg podstabla 
                //fali slucaj kad imamo prenos, da se doda
                insertInRoot(tmp, s, carry, indEndOfLoop);
            }
            //radimo prelom
            else {
                //int positionInParent = findPositionOfNodeInParent(tmp); - ovo ne sme jer prosledjujem parenta
                //numOfKeys = 4, positionInParent = 4 - nalazi se u najdesnjem podstablu, nema desnog brata
                //numOfKeys = 4, positionInParent = 3 - postoji desni brat
                breakRoot(root, s, carry, indEndOfLoop, *this);
            }
        }
        //tmp != root
        else {
            int positionInParent = findPositionOfNodeInParent(tmp);
            //npr numOfKeys = 5, positionInParent = 4 - nema vise kljuceva sa desne
            Node* carry = (indCarry) ? thirdNode : nullptr;
            //prelom sa desnim
            if (positionInParent < tmp->parent->numOfKeys) {
                s = breakNode(tmp, tmp->parent->next[positionInParent + 1], s, carry, indCarry, indEndOfLoop,
                    first, second, third, *this, 1);
                thirdNode = carry;
                if (!indEndOfLoop)tmp = tmp->parent;
            }
            //prelom sa levim
            else {
                s = breakNode(tmp->parent->next[positionInParent - 1], tmp, s, carry, indCarry, indEndOfLoop,
                    first, second, third, *this, 0);
                thirdNode = carry;
                if (!indEndOfLoop)tmp = tmp->parent;
            }
        }
    }
}

bool Tree::isInTree(string s){
    Node* tmp = root;

    if (root->numOfKeys == 0)return false;

    while (tmp) {
        for (int i = 0; i < tmp->numOfKeys; i++) {
            if (s.compare(tmp->keys[i]) < 0) {
                tmp = tmp->next[i];
                break;
            }
            else if (s.compare(tmp->keys[i]) == 0) {
                return true;
            }
            else if (i == tmp->numOfKeys - 1) {
                tmp = tmp->next[tmp->numOfKeys];
                break;
            }
        }
    }

    return false;
}

bool Tree::insertBool(string s){
    int indIsInTree = isInTree(s);
    if (!indIsInTree) {
        insert(s);
        return true;
    }
    else {
        return false;
    }

}

int Tree::howManyLesserThen(string s){
    queue<Node*>Q;
    Q.push(root);
    int cnt = 0;
    while (!Q.empty()) {
        Node* tmp = Q.front();
        Q.pop();
        int indAtLeastOne = 0;
        
        for (int i = 0; i <= tmp->numOfKeys; i++) {
            if(tmp->next[i])Q.push(tmp->next[i]);
            if (i != tmp->numOfKeys) {
                if (tmp->keys[i].compare(s) < 0) {
                    cnt++;
                }
            }
        }
    }
    return cnt;
}

Node* findStringInTree(Tree& t,string s, int& indexOfStringInNode) {
    Node* tmp = t.root;
    while (true) {
        for (int i = 0; i < tmp->numOfKeys; i++) {
            if (s.compare(tmp->keys[i]) < 0) {
                tmp = tmp->next[i];
                break;
            }
            else if (s.compare(tmp->keys[i]) == 0) {
                indexOfStringInNode = i;
                return tmp;
            }
        }
    }
}

Node* findSuccesor(Node*& n, int indexOfString) {
    //udji u desno podstablo od nadjenog stringa
    Node* tmp = n->next[indexOfString + 1];
    while (!isLeaf(tmp)) {
        tmp = tmp->next[0];
    }
    return tmp;
}

bool borrowRightPossible(Node* tmp, Tree& t) {
    Node* parent = tmp->parent;
    if (!parent)return false;

    int pos = findPositionOfNodeInParent(tmp);
    //ako ne postoji desni brat
    if (pos == parent->numOfKeys)return false;

    if (parent->next[pos + 1]->numOfKeys > t.nodeMin - 1) {
        return true;
    }
    return false;
}

bool borrowLeftPossible(Node* tmp, Tree& t) {
    Node* parent = tmp->parent;
    if (!parent)return false;

    int pos = findPositionOfNodeInParent(tmp);
    //ako ne postoji levi brat
    if (pos == 0) return false;

    if (parent->next[pos - 1]->numOfKeys > t.nodeMin - 1) {
        return true;
    }
    return false;
}

bool borrowRRPosible(Node* tmp, Tree& t) {
    Node* parent = tmp->parent;
    if (!parent)return false;

    int pos = findPositionOfNodeInParent(tmp);
    //ako ne postoji 
    if (pos >= parent->numOfKeys - 1)return false;

    if (parent->next[pos + 2]->numOfKeys > t.nodeMin - 1) {
        return true;
    }
    return false;
}

bool borrowLLPosible(Node* tmp, Tree& t) {
    Node* parent = tmp->parent;
    if (!parent)return false;

    int pos = findPositionOfNodeInParent(tmp);

    //ako ne postoji
    if (pos <= 1)return false;

    if (parent->next[pos - 2]->numOfKeys > t.nodeMin - 1) {
        return true;
    }
    return false;
}

void rmvStringFromLeaf(Node* tmp, string s) {
    int pos = findPositionOfString(tmp, s);
    shiftLeftFromPosition(pos - 1, tmp);
    tmp->numOfKeys--;
}

//za position prosledjuj poslednji kraj tako da se ne izvrsi shiftovanje
void overflowRemove(Node* parentNode, /* Node* thirdNode, string s,*/ int position, int size, int right) {
    if (right) {
        //razdelnik
        int splitter = findPositionOfNodeInParent(parentNode);
        //nadjemo desnog brata
        Node* rBro = parentNode->parent->next[splitter + 1];
        //spustimo splitter
        insertStringInNodeFraction(rBro, parentNode->parent->keys[splitter], size);
        //preuzima se pokazivac
        rBro->next[0] = parentNode->next[parentNode->numOfKeys];
        if(parentNode->next[parentNode->numOfKeys])parentNode->next[parentNode->numOfKeys]->parent = rBro;
        //pregazimo splitter
        parentNode->parent->keys[splitter] = parentNode->keys[parentNode->numOfKeys - 1];

        parentNode->numOfKeys--;
    }
    else {
        //razdelnik
        int splitter = findPositionOfNodeInParent(parentNode);
        //nadjemo levog brata
        Node* lBro = parentNode->parent->next[splitter - 1];
        //spustamo splitter
        insertStringInNode(lBro, parentNode->parent->keys[splitter - 1]);
        //preuzima se pokazivac 
        lBro->next[lBro->numOfKeys] = parentNode->next[0];
        if(parentNode->next[0])parentNode->next[0]->parent = lBro;
        //pregazimo splitter
        parentNode->parent->keys[splitter - 1] = parentNode->keys[0];
        //shiftLeft

        shiftLeftUntilPosition(position, parentNode); //ovde hocemo da pomerimo ceo parent ulevo, ovde se vrsi decnumOfKeys
    }
}

Node* makeBigForCompression(Node* firstNode, Node* secondNode, Node* thirdNode) {
    Node* big = new Node(firstNode->numOfKeys + 1 + secondNode->numOfKeys + 1 + thirdNode->numOfKeys + 1);
    
    //prepisujemo firstNode
    writeNode(big, firstNode, 0, firstNode->numOfKeys);
    //dodajemo razdelni kljuc
    int splitter1 = findPositionOfNodeInParent(firstNode);
    insertStringInNode(big, firstNode->parent->keys[splitter1]); // dodaje se na kraj jer je u pitanju razdelni cvor
    //prepisujemo secondNode
    writeNode(big, secondNode, 0, secondNode->numOfKeys, firstNode->numOfKeys + 1, 0);
    //dodajemo drugi razdelni
    int splitter2 = findPositionOfNodeInParent(secondNode);
    insertStringInNode(big, secondNode->parent->keys[splitter2]);
    //prepisujemo thirdNode
    int k = firstNode->numOfKeys + 1 + secondNode->numOfKeys + 1;
    writeNode(big, thirdNode, 0, thirdNode->numOfKeys, k, 0);
    
    return big;

}

Node* makeBig2in1(Node* lNode, Node* rNode) {
    Node* big = new Node(lNode->numOfKeys + 1 + rNode->numOfKeys + 1);
    //prepisujemo lNode
    writeNode(big, lNode, 0, lNode->numOfKeys);
    //dodajemo razdelni kljuc
    int splitter = findPositionOfNodeInParent(lNode);
    insertStringInNode(big, lNode->parent->keys[splitter]); // dodaje se na kraj jer je u pitanju razdelni cvor
    //prepisujemo rNode
    writeNode(big, rNode, 0, rNode->numOfKeys, lNode->numOfKeys + 1, 0);

    //delete lNode;
    //delete rNode;

    return big;
}

Node* compress3in2(Node* firstNode, Node* secondNode, Node* thirdNode, Tree& t, int& indEndOfLoop) {
    Node* parent = firstNode->parent;
    //napravi i popuni veliki
    Node* big = makeBigForCompression(firstNode, secondNode, thirdNode);
    //nadji mid
    int mid = roundUp(((double)big->numOfKeys) / 2.) - 1;
    //override parenta (od pozicije)
    int splitter1 = findPositionOfNodeInParent(firstNode);
    //dva puta shift left parenta i upisi <=> shiftLeft i pregazi
    shiftLeftFromPosition(splitter1, parent);
    parent->keys[splitter1] = big->keys[mid];
    //writeNode(parent, big, mid, mid + 1, parent->numOfKeys - 2, 0);
    parent->numOfKeys--;

    //override prvi i override drugi 
    writeNode(firstNode, big, 0, mid);
    writeNode(secondNode, big, mid + 1, big->numOfKeys);
    //sredi pokazivace na decu i decine na oca i numOfKeys
    parent->next[splitter1/*parent->numOfKeys - 1*/] = firstNode;
    parent->next[splitter1 + 1/*parent->numOfKeys*/] = secondNode;
    firstNode->parent = parent;
    secondNode->parent = parent;
    //treci se brise deca treceg(ako postoje) pokazuju na drugi cvor
    for (int i = 0; i <= thirdNode->numOfKeys; i++) {
        if (thirdNode->next[i])thirdNode->next[i]->parent = secondNode;
    }
    delete thirdNode;

    //provera za prenos
    if (parent != t.root && parent->numOfKeys < t.nodeMin - 1) {
        indEndOfLoop = 0;
    }
    else {
        indEndOfLoop = 1;
    }
    return parent;
}

Node* compress2in1(Node* lNode, Node* rNode, Tree& t, int& indEndOfLoop) {
    Node* big = makeBig2in1(lNode, rNode);

    t.root = big;
    for (int i = 0; i <= lNode->numOfKeys; i++) {
        if(lNode->next[i])lNode->next[i]->parent = big;
    }
    for (int i = 0; i <= lNode->numOfKeys; i++) {
        if(rNode->next[i])rNode->next[i]->parent = big;
    }

    //delete lNode;
    //delete rNode;

    if (big->parent && big->parent != t.root && big->parent->numOfKeys < t.nodeMin - 1) {
        indEndOfLoop = 0;
    }
    else {
        indEndOfLoop = 1;
    }
    return big;
}

Node* compression(Node* tmp, Tree& t, int& indEndOfLoop) {
    Node* parent = tmp->parent;
    //ako radimo sazimanje 2 u 1, nebitno da li je koren u pitanju
    if (parent->numOfKeys < 2) {
        //desni i levi idu u istu fju jer gledamo po parentu
        return compress2in1(parent->next[0], parent->next[1], t, indEndOfLoop);
    }
    //ako radimo sazimanje 3 u 2, nebitno da li je koren u pitanju
    else {
        int pos = findPositionOfNodeInParent(tmp);
        //ako ima l i d
        if (pos < parent->numOfKeys && pos > 0) {
            return compress3in2(parent->next[pos - 1], parent->next[pos], parent->next[pos + 1], t, indEndOfLoop);
        }
        //ako ima d i d
        else if (pos < parent->numOfKeys - 1) {
            return compress3in2(parent->next[pos], parent->next[pos + 1], parent->next[pos + 2], t, indEndOfLoop);
        }
        //ako ima l i l
        else if (pos > 1) {
            return compress3in2(parent->next[pos - 2], parent->next[pos - 1], parent->next[pos], t, indEndOfLoop);
        }
    }
}

void removeFromLeaf(Node*& tmp, Tree& t, string s) {
    //uklanjanje iz lista, pointeri su svakako null
    int indEndOfLoop = 0, first = 0;
    //rmvStringFromLeaf(tmp, s);
    while (!indEndOfLoop) {
        
        if (tmp == t.root && tmp->numOfKeys >= 1 ||
            tmp != t.root && tmp->numOfKeys >= t.nodeMin) {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            indEndOfLoop = 1;
        }
        //pozajmica od desnog
        else if (borrowRightPossible(tmp, t)) {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            Node* rBro = tmp->parent->next[findPositionOfNodeInParent(tmp) + 1];
            //za sad je nullptr, azuriracemo ga kasnije
            Node* carry = nullptr;
            //proveri da li position treba - 1
            overflowRemove(rBro, rBro->numOfKeys, t.m, 0);//ovde ide sl until position
            indEndOfLoop = 1;
        }
        //pozajmica od levog
        else if (borrowLeftPossible(tmp, t)) {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            Node* lBro = tmp->parent->next[findPositionOfNodeInParent(tmp) - 1];
            //za sad je nullptr, azuriracemo ga kasnije
            Node* carry = nullptr;
            overflowRemove(lBro, lBro->numOfKeys, t.m, 1);
            indEndOfLoop = 1;
        }
        //pozajmica desni-desni
        else if (borrowRRPosible(tmp, t)) {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            Node* rSecBro = tmp->parent->next[findPositionOfNodeInParent(tmp) + 2];
            Node* rBro = tmp->parent->next[findPositionOfNodeInParent(tmp) + 1];
            overflowRemove(rSecBro, rSecBro->numOfKeys, t.m, 0);
            overflowRemove(rBro, rBro->numOfKeys, t.m, 0);
            indEndOfLoop = 1;
        }
        //pozajmica levi-levi
        else if (borrowLLPosible(tmp, t)) {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            Node* lSecBro = tmp->parent->next[findPositionOfNodeInParent(tmp) - 2];
            Node* lBro = tmp->parent->next[findPositionOfNodeInParent(tmp) - 1];
            overflowRemove(lSecBro, lSecBro->numOfKeys, t.m, 1);
            overflowRemove(lBro, lBro->numOfKeys, t.m, 1);
            indEndOfLoop = 1;
        }
        //sazimanje <3
        else {
            if (!first) {
                rmvStringFromLeaf(tmp, s);
                first = 1;
            }
            tmp = compression(tmp, t, indEndOfLoop);
        }
    }
}

void Tree::remove(string s){
    //ako kljuc ne postoji u stablu
    if (!isInTree(s)) {
        cout << "Ovaj kljuc ne postoji u stablu." << endl;
        return;
    }
    //ako kljuc postoji nadji ga, ako nije u listu nadji mu naslednika i zameni ga
    int indexOfString = -1;
    Node* tmp = findStringInTree(*this, s, indexOfString), *succ = tmp;

    //ako trazeni kljuc nije u listu nadji mu sledbenika, i zameni kljuceve
    if (!isLeaf(tmp)) {
        //nadji naslednika
        succ = findSuccesor(tmp, indexOfString);
        //menjamo najlevlji kljuc u succ sa kjucem na poziciji indexOfString u tmp
        string tmpString;
        tmpString = succ->keys[0];
        succ->keys[0] = tmp->keys[indexOfString];
        tmp->keys[indexOfString] = tmpString;
    }
    tmp = succ;
    succ = nullptr;

    //ako imamo samo koren sa jednim kljucem
    if (tmp == root && root->numOfKeys == 1) {
        delete root;
        root = nullptr;
        return;
    }
    //odavde se u tmp nalazi cvor koji zelimo da brisemo
    removeFromLeaf(tmp, *this, s);
}

bool Tree::removeBool(string s){
    if (this->isInTree(s)) {
        return false;
    }
    else {
        this->remove(s);
        return true;
    }
}

Tree::~Tree(){
    if (!(isLeaf(root) && root->numOfKeys == 0)) {
        while (root) {
            cout << *this << endl;
            this->remove(root->keys[0]);
        }
    }
}


void Tree::insert(string s) {
    Node* tmp = root;
    tmp = findNodeWhereToInsert(tmp, s);
    //u ovom slucaju moguce je umetanje
    if (tmp == root && tmp->numOfKeys < rootMax - 1 || tmp != root && tmp->numOfKeys < m - 1) {
        insertStringInNode(tmp, s);
    }
    //u ovom slucaju umetanje u list nije moguce
    else {
        //proveravamo da li je prelivanje moguce
        //prvo sa desnim bratom
        if (overflowRightPossible(tmp, *this)) {
            overflow(tmp, 1);
            insertStringInNode(tmp, s);
        }
        //ako je levi overflow moguc
        else if (overflowLeftPossible(tmp, *this)) {
            overflow(tmp, -1);
            insertStringInNode(tmp, s);
        }
        //prelom <3
        else {
            //insertFracture(tmp, s);
            fracture(tmp, s);
        }
    }
}