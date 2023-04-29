#include <iostream>
#include <deque>
#include <algorithm>
#include <time.h>
#include <string>
#include <iostream>
#include <deque>
#include <algorithm>
#include <time.h>
#include <string>
#include <random>
#include <algorithm>
#include <fstream>

using namespace std;

int global_variable=0;
int skuska_raz_dva=0;
int nulovy_pointer_skuska=0;


int countPlus(string bfunction);
int countTimes(string bfunction);
int countOrder(string order);
bool checkIfOne(string bfunction, string order, int level);
string decimalToBinary(int n);
bool decide(int position);
string BDD_use(string order, string entries);
struct BDD* search_node(struct BDD* node, int position);
struct BDD* searchPosition(struct BDD* root,string str ,string position, int count, int size);
int findPosition(struct BDD* node, string str, int position, int level);
string findLetter(string order, int level);
string findHighPath(string bfunction, string order,int level);
string findLowPath(string bfunction, string order,int level);
struct BDD* reduceHigh(struct BDD* node, string str);
struct BDD* reduceLow(struct BDD* node, string str);
struct BDD* findBinary(struct BDD* root,int a);
int patrik();
string boolean_counter(string str);
string sum_up(string str);




struct BDD{
    string function;
    int rank;
    BDD* low;
    BDD* high;
    BDD* parent;
};

struct BDD *newBDDNode(string bfunction,int rank)
{
    BDD* node = new BDD;
    node->function= bfunction;
    node->rank=rank;
    node->low = NULL;
    node->high = NULL;
    node->parent=NULL;
    return node;
}

struct BDD* create(struct BDD* root, struct BDD* node,string bfunction, const string& order, string position ,int limit, int level, int size,int rank,int count){
    if (count!=0)
        return node;
    static BDD *p;
    BDD* temporary_low;
    BDD* temporary_high;

    BDD *retNode;
    if(bfunction=="0" && node != NULL){
        retNode= newBDDNode("0",rank);
        retNode->parent=p;
        //cout << "funkcia: " << retNode->function<<"(otvorena)"<< endl;
        return node;
    }

    if(node == NULL) {

        temporary_low= reduceLow(root,bfunction);
        temporary_high= reduceHigh(root,bfunction);
        if(temporary_low== NULL && temporary_high == NULL)
            nulovy_pointer_skuska++;
        else{
            if(temporary_low!=NULL){
                if(p->low==NULL){
                    p->low=create(root,temporary_low,bfunction,order,position,limit,level,size,rank,1);
                    // cout<<"1. scenar"<<endl;
                }
                else{
                    p->high=create(root,temporary_low,bfunction,order,position,limit,level,size,rank,1);
                    //cout<<"2. scenar"<<endl;
                }
            }
            else {
                if(p->low==NULL){
                    p->low=create(root,temporary_high,bfunction,order,position,limit,level,size,rank,1);
                    //cout<<"3. scenar"<<endl;
                }
                else{
                    p->high=create(root,temporary_high,bfunction,order,position,limit,level,size,rank,1);
                    //cout<<"4. scenar"<<endl;
                }
            }

        }

        // cout<<"cout: rank                    "<<rank<<endl;

        if(bfunction=="0"){
            retNode= newBDDNode("0",rank);
            //cout << "funckia: " << retNode->function<<"(zabudovana)" << endl;
            retNode->parent=p;
            retNode->low = nullptr;
            retNode->high= nullptr;
            return retNode;
        }

        bool f = checkIfOne(bfunction, order, level);
        if (f == true) {
            retNode = newBDDNode("1",rank);
            // cout << "funckia: " << retNode->function<<"(zabudovana)" << endl;
            retNode->parent = p;
            retNode->low= nullptr;
            retNode->high= nullptr;
            return retNode;
        } else {
            retNode = newBDDNode(bfunction,rank);
            //cout << "funckia: " << retNode->function << endl;
            retNode->parent = p;
            return retNode;
        }
    }

    bool f=checkIfOne(bfunction,order,level);
    if(f==true){
        //cout<<"bfunction pri otvorenej jednotke: "<<bfunction<<endl;
        retNode= newBDDNode("1",rank);
        //cout << "funckia: " << retNode->function <<"(otvorena)"<< endl;
        retNode->parent=p;
        return node;
    }

    string help=position;
    string delimiter = ".";
    size_t pos = 0;
    int i = 0;
    string array[size];
    while ((pos = help.find(delimiter)) != string::npos) {
        array[i] = help.substr(0, pos);
        help.erase(0, pos + delimiter.length());
        i++;
    }
    array[i] = help;
    for(int j=0;j<size;j++)
        //cout<<array[j]<<" ";

        while (limit !=size){
            if (array[limit] == "0") {
                p = node;
                level++;
                limit++;
                //cout<<"level "<<level<<endl;
                string str = findLowPath(node->function, order, level);
                node->low = create(root,node->low, str, order, position, limit, level,size,rank,0);
            }
            else if (array[limit] == "1") {
                p = node;
                level++;
                limit++;
                // cout<<"level "<<level<<endl;
                string str = findHighPath(node->function, order, level);
                node->high = create(root,node->high, str, order, position, limit, level,size,rank,0);
            } else
                cout << "Niekde nastala chyba pri zostavovani stromu." << endl;
            level=-1;
            return node;
        }
}

int countPlus(string bfunction)  {
    int a;
    long long b=count(bfunction.begin(),bfunction.end(),'+');
    a=b;
    return a;
}

int countTimes(string bfunction) {
    int a;
    long long b=count(bfunction.begin(),bfunction.end(),'.');
    a=b;
    return a;
}

int countOrder(string order){
    int i = count(order.begin(),order.end(),'/');
    if (i>0)
        return i+1;
    else if (i==0)
        return 1;
    else{
        // cout<<"Nespravne zadane poradie"<<endl;
        return 0;
    }
}

string findLetter(string order, int level){
    string delimiter = "/";
    size_t pos=0;
    int i=0;
    int j=0;
    int count= countOrder(order);
    if (level>count)
        level=count;
    if(count==0)
        return "";
    else if (count==1)
        return order;
    else{
        string array[count];
        while((pos=order.find(delimiter))!=string::npos){
            array[i]=order.substr(0,pos);
            order.erase(0,pos+delimiter.length());
            i++;
        }
        array[i]=order;
        switch(level){
            case 0:j=0; break;
            case 1:j=1;break;
            case 2:j=2;break;
            case 3:j=3;break;
            case 4:j=4;break;
            case 5:j=5;break;
            case 6:j=6;break;
            case 7:j=7;break;
            case 8:j=8;break;
            case 9:j=9;break;
            case 10:j=10;break;
            case 11:j=11;break;
            case 12:j=12;break;
            case 13:j=13;break;
        }
        return array[j];
    }
}
/*
struct BDD* search_node(struct BDD* node, int position){
    int count=0;
    for (int i=1;i<position;i*2)
        count++;
    string binary_position= decimalToBinary(position);
    string delimiter = ".";
    size_t pos = 0;
    int i = 0;
    string array[count];
    while ((pos = binary_position.find(delimiter)) != string::npos) {
        array[i] = binary_position.substr(0, pos);
        binary_position.erase(0, pos + delimiter.length());
        i++;
    }

    for (int i=0;i<count-1;i++){
        if(array[i]=="0")
            search_node(node->low,position);
        if(array[i]=="1")
            search_node(node->high,position);
    }
    return node;
}

bool decide(int position){
    if(position%2==0)
        return false;
    if(position%2==1)
        return true;
}
*/
/*int findPosition(struct BDD* node, string str, int position,int level) {
    BDD *root = node;
    BDD *check;
    if (node == NULL)
        return -1;
    int size = 0;
    int j=1;
    string binary_position;
    for (int a = 0; a != (level); a++) {
        for (int j = 0; j != (level + 1) * 2; j++) {
            binary_position = decimalToBinary(position);
            cout<<"position: "<<position<<"          binary position: "<<binary_position<<endl;
            check = searchPosition(root, str,binary_position, -1, level);
            cout<<"check....."<<check->function<<"     string: "<<str<<endl;
            if (check->function == str)
                return position;
            position++;
        }
    }
    return -1;
}

struct BDD* searchPosition(struct BDD* temp, string str ,string position, int count, int size) {
    //cout<<"temp->function: "<<temp->function<<"   string:"<<str<<edl;
    count++;
    cout<<"count: "<<count<<"     size: "<<size<<endl;
    if (temp == NULL)
        return temp;
    if(count==size)
        return temp;

    string help = position;
    string delimiter = ".";
    size_t pos = 0;
    int i = 0;
    string array[size];
    while ((pos = help.find(delimiter)) != string::npos) {
        array[i] = help.substr(0, pos);
        help.erase(0, pos + delimiter.length());
        i++;
    }
    array[i] = help;


        if (array[count] == "0") {
            temp->low=searchPosition(temp->low, str,position, count, size);
        } else if (array[count] == "1") {
            temp->high=searchPosition(temp->high,str, position, count, size);
        } else
            cout<<"niekde nastala chyba !!!!!!!!!!!!!!!!!!"<<endl;
            return temp;
}
*/


bool checkIfOne(string bfunction, string order, int level){
    //cout<<"------------>funkcia<-----------"<<bfunction<<endl;
    //cout<<"toto sem vstupuje: "<<bfunction<<endl;
    //cout<<"level: "<<level<<endl;
    if(level<0)
        return false;
    //cout<<"level: "<<level<<endl;
    string limiter = "+";
    size_t main_pos = 0;
    int i = 0;
    int plus = countPlus(bfunction)+1;
    string array[plus];
    while ((main_pos = bfunction.find(limiter)) != string::npos) {
        array[i]= bfunction.substr(0, main_pos);
        bfunction.erase(0, main_pos + limiter.length());
        i++;
    }
    array[i] = bfunction;

    for(int g=0;g!=i+1;g++){
        string delimiter = "/";
        size_t pos=0;
        int w=0;
        int count= countOrder(order);
        if (level>count)
            level=count;
/*  else if (count==1)
        return order;*/
        else{
            string arrayPos[count];
            while((pos=order.find(delimiter))!=string::npos){
                arrayPos[w]=order.substr(0,pos);
                order.erase(0,pos+delimiter.length());
                w++;
            }
            arrayPos[w]=order;

            string arrayNeg[count];
            for(int k=0;k!=count;k++){
                arrayNeg[k]=arrayPos[k]+"!";
                //  cout<<arrayNeg[k]<<" "<<arrayPos[k]<<endl;
            }

            //potiaľto ok
            string divider = ".";
            size_t position=0;
            int j=0;
            //toto je diskutabilne
            int times= countTimes(array[g]);
            string arrayNew[times+1];
            while((position=array[g].find(divider))!=string::npos){
                arrayNew[j]=array[g].substr(0,position);
                array[g].erase(0,position+divider.length());
                j++;
            }
            arrayNew[j]=array[g];

            int c=0;
            for(int a=0;a!=times+1;a++){
                for(int b=0;b!=level+1;b++){
                    //cout<<"arrNew: "<<arrayNew[a]<<" arrPos: "<<arrayPos[b]<<" arrNeg: "<<arrayNeg[b]<<endl;
                    if(arrayNew[a]==arrayPos[b] || arrayNew[a]==arrayNeg[b]) {
                        //cout<<"tieto sa rovnaju: "<<arrayNew[a]<<" "<<arrayPos[b]<<" "<<arrayNeg[b]<<endl;
                        c++;
                        if(c==times+1) {
                            //cout << "skoncilo sa to uspesne!!!" << endl;
                            return true;
                        }
                        continue;
                    }
                }
            }
            //cout<<"jojojoj, velky neuspech!!!"<<endl;
            return false;
        }
    }
/*        switch(level){
            case 0:j=0; break;
            case 1:j=1;break;
            case 2:j=2;break;
            case 3:j=3;break;
            case 4:j=4;break;
            case 5:j=5;break;
            case 6:j=6;break;
            case 7:j=7;break;
            case 8:j=8;break;
            case 9:j=9;break;
            case 10:j=10;break;
            case 11:j=11;break;
            case 12:j=12;break;
            case 13:j=13;break;
        }*/

}

string findHighPath(string bfunction, string order,int level) {//token a tokenNew môžme odstrániť a nahradiť array[i] a arrayNew[j]
    string delimiter = "+";
    size_t pos = 0;
    int i = 0;
    int plus = countPlus(bfunction)+1;
    string array[plus];
    string temp = findLetter(order,level) + "!";
    // cout<<"HIGH - letter: "<<temp<<endl;
    // cout<<"___________"<<endl;
    // cout<<bfunction<<endl;
    while ((pos = bfunction.find(delimiter)) != string::npos) {
        array[i]= bfunction.substr(0, pos);
        bfunction.erase(0, pos + delimiter.length());
        i++;
    }
    array[i] = bfunction;
    string returnBack = "";

    for (int j = 0; j != i+1; j++) {
        here:
        if(j==i+1)
            break;
        int times = countTimes(array[j]);
        if (times ==0 && array[j] != temp) {
            if (returnBack != "")
                returnBack = returnBack + "+";
            returnBack = returnBack + array[j];
            j++;
            goto here;
        }
        else if(times==0 && array[j] == temp){
            j++;
            goto here;
        }
        else {
            if(times!=0)
                times++;
            string arrayNew[times];
            string divider = ".";
            size_t position=0;
            int k = 0;
            while ((position = array[j].find(divider)) != string::npos) {
                arrayNew[k] = array[j].substr(0, position);
                array[j].erase(0, position + divider.length());
                k++;
            }
            arrayNew[k] = array[j];
            for (int l = 0; l != k + 1; l++) {
                if (arrayNew[l] == temp) {
                    j++;
                    goto here;
                }
            }
            if(returnBack != ""){
                returnBack=returnBack + "+";
                for (int l=0;l!=k+1;l++){
                    if(l==0)
                        returnBack=returnBack+arrayNew[l];
                    else {
                        returnBack = returnBack + "." + arrayNew[l];
                    }
                }
            }
            else {
                for (int l = 0; l != k+1; l++) {
                    if (l == 0)
                        returnBack = returnBack + arrayNew[l];
                    else
                        returnBack = returnBack + "." + arrayNew[l];
                }
            }
        }
    }
    if(returnBack==""){
        returnBack="0";
    }
    return returnBack;
}

string findLowPath(string bfunction, string order,int level) {//token a tokenNew môžme odstrániť a nahradiť array[i] a arrayNew[j]
    string delimiter = "+";
    size_t pos = 0;
    int i = 0;
    int plus = countPlus(bfunction)+1;
    string array[plus];
    string temp = findLetter(order,level);
    // cout<<"LOW - letter: "<<temp<<endl;
    // cout<<"___________"<<endl;
    // cout<<bfunction<<endl;

    while ((pos = bfunction.find(delimiter)) != string::npos) {
        array[i]= bfunction.substr(0, pos);
        bfunction.erase(0, pos + delimiter.length());
        i++;
    }
    array[i] = bfunction;
    string returnBack = "";

    for (int j = 0; j != i+1; j++) {
        here:
        /*bool f= false;
        f=checkIfOne(array[j],order,level);
        if(f== true){
            returnBack="1";
            return returnBack;
        }*/
        if(j==i+1)
            break;
        int times = countTimes(array[j]);
        if (times ==0 && array[j] != temp) {
            if (returnBack != "")
                returnBack = returnBack + "+";
            returnBack = returnBack + array[j];
            j++;
            goto here;
        }
        else if(times==0 && array[j] == temp){
            j++;
            goto here;
        }
        else {
            if(times!=0)
                times++;
            string arrayNew[times];
            string divider = ".";
            size_t position=0;
            int k = 0;
            while ((position = array[j].find(divider)) != string::npos) {
                arrayNew[k] = array[j].substr(0, position);
                array[j].erase(0, position + divider.length());
                k++;
            }
            arrayNew[k] = array[j];
            for (int l = 0; l != k + 1; l++) {
                if (arrayNew[l] == temp) {
                    j++;
                    goto here;
                }
            }
            if(returnBack != ""){
                returnBack=returnBack + "+";
                for (int l=0;l!=k+1;l++){
                    if(l==0)
                        returnBack=returnBack+arrayNew[l];
                    else {
                        returnBack = returnBack + "." + arrayNew[l];
                    }
                }
            }
            else {
                for (int l = 0; l != k+1; l++) {
                    if (l == 0)
                        returnBack = returnBack + arrayNew[l];
                    else
                        returnBack = returnBack + "." + arrayNew[l];
                }
            }
        }
    }
    if(returnBack=="")
        returnBack="0";
    return returnBack;
}

string decimalToBinary(int n){
    int a[13],i;
    string num;
    for(i=0;n>=2;i++)
    {
        a[i]=n%2;
        n=n/2;
    }
    for(i=i-1;i>=0;i--)
    {
        num= num + to_string(a[i]);
        if(i>=1)
            num = num + ".";
    }
    return num;
}


void print(struct BDD* node){
    if(node == NULL)
        return;
    // cout<<"funkcia: "<<node<<"   rank:"<<node->rank<<endl;
    skuska_raz_dva++;
    print(node->low);
    print(node->high);
}

struct BDD* reduceHigh(struct BDD* node, string str){
    if(node==NULL){
        return NULL;
    }
    if(node->function==str) {
        // cout<<"node: "<<node<<endl;
        // cout<<"tu sa rovná (funckia + string + rank) : "<<node->function<<" + "<<str<<" + "<<node->rank<<endl;
        return node;
    }
    return reduceHigh(node->high,str);
}

struct BDD* reduceLow(struct BDD* node, string str){
    if(node==NULL){
        return NULL;
    }
    if(node->function==str) {
        //  cout<<"node: "<<node<<endl;
        // cout<<"tu sa rovná (funckia + string + rank) : "<<node->function<<" + "<<str<<" + "<<node->rank<<endl;
        return node;
    }
    return reduceLow(node->low,str);
}

string BDD_use(struct BDD* root, string entries){
    static BDD* q;
    BDD* temp=root;
    // cout<<"root root: "<<root<<endl;
    int count= countTimes(entries);
    string help=entries;
    string delimiter = ".";
    size_t pos = 0;
    int i = 0;
    string array[count+1];
    while ((pos = help.find(delimiter)) != string::npos) {
        array[i] = help.substr(0, pos);
        help.erase(0, pos + delimiter.length());
        i++;
    }
    array[i] = help;
    int x;

    for(int j=0;j<count+1;j++) {
        // cout << array[j] << " ";
        if(array[j]=="0"){
            x=0;
        }
        if(array[j]=="1"){
            x=1;
        }
        q=temp;
        temp= findBinary(temp,x);
        if(temp->function=="0" || temp->function=="1")
            return temp->function;
        // cout<<"temp:  "<<temp<<endl;
        // cout<<"temp-function: "<<temp->function<<endl;
    }
    return temp->function;
}

struct BDD* findBinary(struct BDD* root,int a){
    if(a!=0 && a!=1)
        return root;
    // cout<<"root(FB): "<<root<<endl;
    if(root==NULL)
        return NULL;
    if(a==0)
        return findBinary(root->low,a+2);
    if(a==1)
        return findBinary(root->high,a+2);
}

bool countBoolean(string str, string order, string value){
    //  cout<<str<<endl;
    //cout<<order<<endl;

    string delimiter_value = ".";
    size_t pos_value = 0;
    int count_value;
    int k=0;
    count_value = countTimes(value) +1;
    string valueArray[count_value];
    while ((pos_value = value.find(delimiter_value)) != string::npos) {
        valueArray[k]= value.substr(0, pos_value);
        value.erase(0, pos_value + delimiter_value.length());
        k++;
    }
    valueArray[k] = value;

    /* for (int i=0;i<count_value;i++){
         cout<<valueArray[i]<<" ";
     }
     cout<<endl;
 */

    string delimiter_order = "/";
    size_t pos_order = 0;
    int count_order;
    int i = 0;
    count_order= countOrder(order);
    string lettersPos[count_order];

    while ((pos_order = order.find(delimiter_order)) != string::npos) {
        lettersPos[i]= order.substr(0, pos_order);
        order.erase(0, pos_order + delimiter_order.length());
        i++;
    }
    lettersPos[i] = order;


    string lettersNeg[count_order];
    for(int i=0;i<count_order;i++){
        lettersNeg[i]=lettersPos[i]+"!";
        //    cout<<"pozitivne pismeno: "<<lettersPos[i];
        //  cout<<"     negativne pismeno: "<<lettersNeg[i]<<endl;
    }


    string delimiter_plus = "+";
    size_t pos_plus = 0;
    int count_plus;
    int j = 0;
    count_plus= countPlus(str)+1;
    string arrayPlus[count_plus];

    while((pos_plus=str.find(delimiter_plus))!= string::npos){
        arrayPlus[j]=str.substr(0,pos_plus);
        str.erase(0,pos_plus+delimiter_plus.length());
        j++;
    }
    arrayPlus[j] = str;

    /*for (int i=0;i<count_plus;i++){
        cout<<arrayPlus[i]<<" ";
    }
    cout<<endl;*/

    //cout<<"count value: "<<count_value<<endl;
    //cout<<"count plus: "<<count_plus<<endl;
    //cout<<"count order: "<<count_order<<endl;


    for(int i=0;i<count_plus;i++){
        string delimiter_times= ".";
        size_t pos_times = 0;
        int count_times;
        int g=0;
        count_times= countTimes(arrayPlus[i])+1;
        string arrayTimes[count_times];
        // cout<<"count times: "<<count_times<<endl;
        // cout<<"count value: "<<count_value<<endl;
        //cout<<"count plus: "<<count_plus<<endl;
        //cout<<"count order: "<<count_order<<endl;
        while((pos_times=arrayPlus[i].find(delimiter_times))!= string::npos){
            arrayTimes[g]=arrayPlus[i].substr(0,pos_times);
            arrayPlus[i].erase(0,pos_times+delimiter_times.length());
            g++;
        }
        arrayTimes[g] = arrayPlus[i];

        /*for (int p=0;p<count_times;p++){
            cout<<arrayTimes[p]<<" ";
        }
        cout<<endl;*/
        int positive_counter=0;
        for(int i=0;i<count_times;i++) {
            for (int h = 0; h < count_value; h++) {
                if (valueArray[h] == "0" && lettersNeg[h] == arrayTimes[i])
                    positive_counter++;
                if (valueArray[h] == "1" && lettersPos[h] == arrayTimes[i])
                    positive_counter++;
                if (positive_counter == count_times)
                    return true;
            }
        }
    }
    return false;
}

int patrik(){
    static default_random_engine gen(123456789);
    static uniform_int_distribution dist(0,25);
    int i;
    i=dist(gen);
    return i;
}

string RFG(int num) {
    int r;
    int p;
    int q;
    string str;
    srand (time(NULL));
    char letters[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    for(int i=0;i<num;i++){
        r=patrik();
        p=patrik();
        q=patrik();

        if(p%2)
            str=str+letters[r];
        else
            str=str+letters[r]+"!";
        if(i==num-1)
            return str;
        if(q%2)
            str=str+"+";
        else
            str=str+".";
    }
}

string sum_up(string str){
    string temp;
    int count_size=0;
    int slash=0;
    char letters[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    for(int i=0;i<26;i++){
        long long b=count(str.begin(),str.end(),letters[i]);
        if(b>0)
            count_size++;
    }
    for(int i=0;i<26;i++){
        long long b=count(str.begin(),str.end(),letters[i]);
        if(b>0){
            temp=temp+letters[i];
            if(slash<count_size-1){
                temp=temp+"/";
                slash++;
            }
        }
    }
    return temp;
}

string boolean_counter(string str){
    string temp;
    long long a = count(str.begin(),str.end(),'/');
    int i=a;
    char value[2] = {'0','1'};
    int r;
    for(int j=0;j<i+1;j++){
        r=patrik();

        temp=temp+value[r%2];
        if(j!=i)
            temp=temp+".";
    }
    return temp;
}

int main(){
    string str;
    string temp;
    string b_value;
    int nezhoda=0;
    fstream newfile;
    fstream functions;
    string fileArray[100];

    functions.open("functions.txt",ios::in);

    for(int w=0;w<100;w++){
        if(functions.is_open()){
        string fileTemp;
        getline(functions,fileTemp);
        fileArray[w]=fileTemp;
        }
    }
    functions.close();

    for(int g=0;g<100;g++){

    BDD* root =NULL;
    int position=2;
    int temp_position;
    string binary_position;
    string temp_binary;
    BDD* temporary;
    int i=0;
    int k=2;
    int p=0;
    int j=1;
    bool f;
    int level=-1;




    newfile.open("boolean.txt", ios::in);
    str=fileArray[g];
    temp = sum_up(str);
    root = create(root, root, str, temp, "1", 0, 0, 0, 1, 0);

    while (i + 1) {
        for (int f = 0; f != k; f++) {
            binary_position = decimalToBinary(position);
            temp_binary = binary_position;
            create(root, root, str, temp, binary_position, p, level, j, position, 0);
            position++;
        }
        j++;
        global_variable++;
        k *= 2;
        i++;
        if (i == countOrder(temp))
            break;
    }
    for(int q=0;q<8192;q++) {
        string tp;
        if(newfile.is_open())
            getline(newfile, tp);  //read data from file object and put it into string.
        else
            return -8;
        cout <<q << "       " <<str << "      " << temp << "      " << tp << endl;
        f = countBoolean(str, temp, tp);
        if (f == true)
            cout << "result(funkcia): " << "1" << endl;
        else
            cout << "result(funkcia): " << "0" << endl;
        print(root);
        string result = BDD_use(root, tp);
        if(result=="0"&&f==false)
            cout<<"zhoda"<<endl;
        if(result=="1"&&f==true)
            cout<<"zhoda"<<endl;
        if(result=="1"&&f==false)
            nezhoda++;
        if(result=="0"&&f==true)
            nezhoda++;
        cout << "result(diagram): " << result << endl;
        cout <<"pocet premennych: " <<global_variable << endl;
        cout <<"pocet uzlov: "<<skuska_raz_dva << endl;
        cout<<"nezhoda: "<<nezhoda<< endl;
        global_variable=0;
        skuska_raz_dva=0;
    }
    getchar();

    newfile.close();
    }
    return 0;
}