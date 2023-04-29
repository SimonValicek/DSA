/*
�imon Val��ek
DSA
7.3.2022
prv� dom�ca �loha
in�piroval som sa internetom, ale nie je to ani z�aleka copypasta
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Zadefinujeme si polia pre star� a nov� p�smenk� z re�azca a pre star� a nov� indexy dan�ch p�smenok.
Plat�, �e prv� hodnota v poli p�smen zodpoved� prvej hodnote v poli indexov.
Polia "pole" a "index" zobrazuj� v�etky p�smen� abecedy, aj tie s indexom 0.
Polia "newpole" a "newindex" s� �� zoraden� a zobrazuj� len p�smen� s indexom 1 a viac.
Ni��ie v k�de s� funkcie, ktor� to zabezpe�uj�.*/

char pole[100], newpole[30];
int pole_index[100], newpole_index[30];


/*Vytvor�me si funkciu string, so vstupn�mi parametrami pole typu ukazovate� na char a index typu ukazovate� na int.
T�to funckia n�m zabezpe�uje vstup nami zadan�ho re�azca do programu a n�sledn� vyh�adanie jednotliv�ch p�smen a vyhodnotenie ich indexov.
N�vratov� hodnota tejto funkcie je po�et p�smen s nenulov�m indexom.
Funkcia obsahuje jeden uz�tvorkovan� riadok, ktor� zabezpe�uje v�pis.*/

int string(char* pole, int* index) {
    char str[1000];
    int i = 0;
    int j = 0;

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    for (char ch = 'a'; ; ch++) {
        int count = 0;
        for (int i = 0; str[i] != '\n'; ++i) {
            if (ch == str[i] || (ch - ('a' - 'A') == str[i]))
                ++count;
        }
        *(pole + i) = ch;
        *(index + i) = count;
        if (ch < 90 && ch>65)
            ch = ch - ('a' - 'A');
        //printf("Frequency of %c = %d\n", ch, count);                  
        j++;
        if (ch == 'z' || ch == 'Z') break;
        i++;
    }
    return j;
}

/*T�to funckia zabezpe�uje zoradenie nami zadan�ch p�smen z re�azca pod�a p�smen od t�ch s najmen��m indexom po tie s najv���m.*/

void bubbleSort(int* index, int size, char* pole) {

    for (int step = 0; step < size - 1; ++step) {
        for (int i = 0; i < size - step - 1; ++i) {
            if (index[i] > index[i + 1]) {

                int temp = index[i];
                index[i] = index[i + 1];
                index[i + 1] = temp;
                char temp1 = pole[i];
                pole[i] = pole[i + 1];
                pole[i + 1] = temp1;
            }
        }
    }
}


/*Pomocn� funckia na zistenie spr�vnosti vytvorenia pol� "pole" a "index".
V odovzdanom programe bude zrejme uz�tvorkovan�.*/

/*void printArray(int* index, int size, char* pole) {
    for (int i = 0; i < size; i++) {
        printf("%c  %d\n", pole[i], index[i]);
    }
    printf("\n");
}
*/

/*Funckia na vytvorenie nov�ch pol� "newpole" a "newindex", ktor u� bud� zoraden� a bez p�smen s nulov�m indexom.
N�vratov� hodnota funckie je ve�kos� nov�ch pol�, respekt�ve po�et p�smen s nenulov�m indexom.*/

int organising(int* index, char* pole, char* newpole, int* newindex, int size) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (index[i] != 0) {
            newindex[j] = index[i];
            newpole[j] = pole[i];
            j++;
        }
    }
    return j;
}

/*Pomocn� funkcia na vyp�sanie a overenie novovytvoren�ho po�a, v odovzdanom projekte bude zrejme uz�tvorkovan�.*/

/*void printnewArray(int* newindex, char* newpole, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c %d\n", newpole[i], newindex[i]);
    }
    putchar('\n');
}
*/

/*Vytvor�me si nov� typ node, ktor� obsahuje premenn� data(p�smeno v re�azci), premenn� index a dva pointre na prav�ho a �av�ho "childa"*/

struct node {
    char data;
    int index;
    struct node* left;
    struct node* right;
};

/*vytvor�me si skupinu listov huffmanovho stromu*/

struct min {
    int size;
    int info;
    struct node** array;
};

/*vytvor�me funckiu typu node* ktor� n�m bude vytv�ra� nov� nody*/

struct node* newNode(char data, int index){
    struct node* temporary = (struct node*)malloc(sizeof(struct node));
    temporary->data = data;
    temporary->index = index;
    temporary->left = NULL;
    temporary->right = NULL;
    return temporary;
}

/*vytvor�me si funkciu newMin, ktor� n�m bude zabezpe�ova� vytv�ranie kolekcie listov typu struct min*/

struct min* newMin(int index) {
    struct min* minimum = (struct min*)malloc(sizeof(struct min));
    minimum->size = 0;
    minimum->info = index;
    minimum->array = (struct node**)malloc(minimum->info * sizeof(struct node*));
    return minimum;
}

/*Funkcia na prehodenie porovnanie dvoch nodov, aby sme ich mali zoraden� pod�a ve�kosti.*/

void swapMinNodes(struct node** x, struct node** y) {
    struct node* temp = *x;
    *x = *y;
    *y = temp;
}

/*Funkcia na usporiadanie pod�a indexov.*/

void minNode(struct min* minimum, int index) {
    int least = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < minimum->size && minimum->array[left]->index < minimum->array[least]->index)
        least = left;
    if (right < minimum->size && minimum->array[right]->index < minimum->array[least]->index)
        least = right;
    if (least != index) {
        swapMinNodes(&minimum->array[least], &minimum->array[index]);
        minNode(minimum, least);
    }
}

/*zist�me �i je index 1*/

int isOne(struct min* minimum) {
    return(minimum->size == 1);
}

/*Funkcia na ukladanie nodov do pola */

struct node* extract(struct min* minimum) {
    struct node* temporary = minimum->array[0];
    minimum->array[0] = minimum->array[minimum->size - 1];
    --minimum->size;
    minNode(minimum, 0);
    return temporary;
}

/*Funkcia sl��i na vlo�enie nodov.*/

void insertNode(struct min* minimum, struct node* newnode) {
    ++minimum->size;
    int i = minimum->size - 1;
    while (i && newnode->index < minimum->array[(i - 1) / 2]->index) {
        minimum->array[i] = minimum->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minimum->array[i] = newnode;
}

/*funkcia na vytvorenie podstromu*/
void BuildMiniTree(struct min* minimum) {
    int x = minimum->size - 1;
    int i;
    for (i = (x - 1) / 2; i >= 0; i--) {
        minNode(minimum, i);
    }
}

/*Vytvor�me si funckiu na vyp�s.*/

void printArray2(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
}

/*funckia na zistenie �i node je list*/

int Leaf(struct node* root) {
    return!(root->left) && !(root->right);
}

/*funckia zabezpe�uje vytvorenie jedn�ho nodu so v�etk�m �o je s t�m spojen�*/

struct min* createNode(char data[], int index[], int size) {
    struct min* minimum = newMin(size);
    for (int i = 0; i < size; i++) {
        minimum->array[i] = newNode(data[i], index[i]);
    }
    minimum->size = size;
    BuildMiniTree(minimum);

    return minimum;
}

/*funckia vytvor� huffmanov strom*/

struct node* HuffmanTree(char data[], int index[], int size) {
    struct node* left, * right, * parent;
    struct min* minimum = createNode(data, index, size);
    while (!isOne(minimum)) {
        left = extract(minimum);
        right = extract(minimum);
        parent = newNode('$', left->index + right->index);
        parent->left = left;
        parent->right = right;
        insertNode(minimum, parent);
    }
    return extract(minimum);
}

/*Funckia sl��iaca na v�pis cel�ho Huffmanovho stromu.*/

void PrintHuffman(struct node* root, int arr[], int parent) {
    if (root->left) {
        arr[parent] = 0;
        PrintHuffman(root->left, arr, parent + 1);
    }
    if (root->right) {
        arr[parent] = 1;
        PrintHuffman(root->right, arr, parent + 1);
    }
    if (Leaf(root)) {
        printf("%c: ", root->data);
        printArray2(arr, parent);
    }
}


/*funckia zabezpe�uj�ca prepojenie vy��ie vyp�san�ch funkcii*/

void Code(char arr[], int arr1[], int size) {
    struct node* root = HuffmanTree(arr, arr1, size);
    int array[30], top = 0;
    PrintHuffman(root, array, top);
}

/*N�sledne m�me funkciu main, ktor� n�m volan�m ostatn�ch funkcii zabezpe�� chod programu.*/

int main()
{
    int a = string(pole, pole_index);
    bubbleSort(pole_index, a, pole);
    //printArray(pole_index, a, pole);
    int count = organising(pole_index, pole, newpole, newpole_index, a);
    //printnewArray(newpole_index, newpole, count);
    Code(newpole, newpole_index, count);
    return 0;
}
