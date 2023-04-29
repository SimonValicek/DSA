/*
Šimon Valíèek
DSA
7.3.2022
prvá domáca úloha
inšpiroval som sa internetom, ale nie je to ani zïaleka copypasta
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Zadefinujeme si polia pre staré a nové písmenká z reazca a pre staré a nové indexy danıch písmenok.
Platí, e prvá hodnota v poli písmen zodpovedá prvej hodnote v poli indexov.
Polia "pole" a "index" zobrazujú všetky písmená abecedy, aj tie s indexom 0.
Polia "newpole" a "newindex" sú ú zoradené a zobrazujú len písmená s indexom 1 a viac.
Nišie v kóde sú funkcie, ktoré to zabezpeèujú.*/

char pole[100], newpole[30];
int pole_index[100], newpole_index[30];


/*Vytvoríme si funkciu string, so vstupnımi parametrami pole typu ukazovate¾ na char a index typu ukazovate¾ na int.
Táto funckia nám zabezpeèuje vstup nami zadaného reazca do programu a následné vyh¾adanie jednotlivıch písmen a vyhodnotenie ich indexov.
Návratová hodnota tejto funkcie je poèet písmen s nenulovım indexom.
Funkcia obsahuje jeden uzátvorkovanı riadok, ktorı zabezpeèuje vıpis.*/

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

/*Táto funckia zabezpeèuje zoradenie nami zadanıch písmen z reazca pod¾a písmen od tıch s najmenším indexom po tie s najväèším.*/

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


/*Pomocná funckia na zistenie správnosti vytvorenia polí "pole" a "index".
V odovzdanom programe bude zrejme uzátvorkovaná.*/

/*void printArray(int* index, int size, char* pole) {
    for (int i = 0; i < size; i++) {
        printf("%c  %d\n", pole[i], index[i]);
    }
    printf("\n");
}
*/

/*Funckia na vytvorenie novıch polí "newpole" a "newindex", ktor u budú zoradené a bez písmen s nulovım indexom.
Návratová hodnota funckie je ve¾kos novıch polí, respektíve poèet písmen s nenulovım indexom.*/

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

/*Pomocná funkcia na vypísanie a overenie novovytvoreného po¾a, v odovzdanom projekte bude zrejme uzátvorkovaná.*/

/*void printnewArray(int* newindex, char* newpole, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c %d\n", newpole[i], newindex[i]);
    }
    putchar('\n');
}
*/

/*Vytvoríme si novı typ node, ktorı obsahuje premennú data(písmeno v reazci), premennú index a dva pointre na pravého a ¾avého "childa"*/

struct node {
    char data;
    int index;
    struct node* left;
    struct node* right;
};

/*vytvoríme si skupinu listov huffmanovho stromu*/

struct min {
    int size;
    int info;
    struct node** array;
};

/*vytvoríme funckiu typu node* ktorá nám bude vytvára nové nody*/

struct node* newNode(char data, int index){
    struct node* temporary = (struct node*)malloc(sizeof(struct node));
    temporary->data = data;
    temporary->index = index;
    temporary->left = NULL;
    temporary->right = NULL;
    return temporary;
}

/*vytvoríme si funkciu newMin, ktorá nám bude zabezpeèova vytváranie kolekcie listov typu struct min*/

struct min* newMin(int index) {
    struct min* minimum = (struct min*)malloc(sizeof(struct min));
    minimum->size = 0;
    minimum->info = index;
    minimum->array = (struct node**)malloc(minimum->info * sizeof(struct node*));
    return minimum;
}

/*Funkcia na prehodenie porovnanie dvoch nodov, aby sme ich mali zoradené pod¾a ve¾kosti.*/

void swapMinNodes(struct node** x, struct node** y) {
    struct node* temp = *x;
    *x = *y;
    *y = temp;
}

/*Funkcia na usporiadanie pod¾a indexov.*/

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

/*zistíme èi je index 1*/

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

/*Funkcia slúi na vloenie nodov.*/

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

/*Vytvoríme si funckiu na vypís.*/

void printArray2(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
}

/*funckia na zistenie èi node je list*/

int Leaf(struct node* root) {
    return!(root->left) && !(root->right);
}

/*funckia zabezpeèuje vytvorenie jedného nodu so všetkım èo je s tım spojené*/

struct min* createNode(char data[], int index[], int size) {
    struct min* minimum = newMin(size);
    for (int i = 0; i < size; i++) {
        minimum->array[i] = newNode(data[i], index[i]);
    }
    minimum->size = size;
    BuildMiniTree(minimum);

    return minimum;
}

/*funckia vytvorí huffmanov strom*/

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

/*Funckia slúiaca na vıpis celého Huffmanovho stromu.*/

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


/*funckia zabezpeèujúca prepojenie vyššie vypísanıch funkcii*/

void Code(char arr[], int arr1[], int size) {
    struct node* root = HuffmanTree(arr, arr1, size);
    int array[30], top = 0;
    PrintHuffman(root, array, top);
}

/*Následne máme funkciu main, ktorá nám volaním ostatnıch funkcii zabezpeèí chod programu.*/

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
