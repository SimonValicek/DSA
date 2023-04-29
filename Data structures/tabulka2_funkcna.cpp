#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define SLOVO 15
#define MAX 100


struct struktura{
	char *slovo;
	struct struktura* dalsi;
};

struct tabulka{
	struct struktura** odkaz;
};

int hash(char slovo[])
{
	int dlzka=strlen(slovo), hodnota=0;
	for(int i=0;i<dlzka;i++)
		hodnota+=slovo[i];
	hodnota=hodnota%MAX;	
	return hodnota;	
}

struct struktura* vytvor_prvok(char* slovo)
{
	struct struktura* pom=(struct struktura*)malloc(sizeof(struct struktura)*1);
	pom->slovo=(char*)malloc(strlen(slovo)*(sizeof(char)+1));	
	strcpy(pom->slovo,slovo);
	pom->dalsi=NULL;
	return pom;
}

struct tabulka* vytvor()
{
	struct tabulka* pom=(struct tabulka*)malloc(sizeof(struct tabulka)*1);
	pom->odkaz=(struct struktura**)malloc(MAX*sizeof(struct struktura*));
	for(int i=0;i<MAX;i++)
		pom->odkaz[i]=NULL;
	return pom;
}

void pridaj(struct tabulka* pom,char* slovo)
{
	int kluc=hash(slovo);
	struct struktura* prvok=pom->odkaz[kluc];
	struct struktura* predchadzajuci=NULL;
	struct struktura* pomocny=NULL;
	if(prvok==NULL)
	{
		pom->odkaz[kluc]=vytvor_prvok(slovo);
		return;
	}		
	while(prvok!=NULL)
	{
		if(strcmp(prvok->slovo,slovo)==0)
		{
			free(prvok->slovo);
			prvok->slovo=(char*)malloc(strlen(slovo)*(sizeof(char)+1));
			strcpy(prvok->slovo,slovo);	
			return;
		}
		predchadzajuci=prvok;
		prvok=predchadzajuci->dalsi;
	}
	predchadzajuci->dalsi=vytvor_prvok(slovo);
}

void odstran(struct tabulka* pom,char* slovo)
{
	int kluc=hash(slovo);
	struct struktura* prvok=pom->odkaz[kluc];
	for(int i=0;i<MAX;i++)
	{
		struct struktura* predchadzajuci=pom->odkaz[i];
		if(strcmp(predchadzajuci->slovo,prvok->slovo)==0)
 		{
			struct struktura* pomocna=pom->odkaz[i-1];
			pomocna->dalsi=pom->odkaz[i+1];
			free(prvok->slovo);
			free(pom->odkaz[i]);
		}
	}	
}


bool hladaj(struct tabulka* pom, char* slovo)
{
	int kluc=hash(slovo);
	struct struktura* pom_prem=pom->odkaz[kluc];
	if(pom_prem==NULL)
		return false;
	while(pom_prem!=NULL)
	{
		if(strcmp(pom_prem->slovo,slovo)==0)
			return true;
		pom_prem=pom_prem->dalsi;	
	}		
	return false;
}

void vypis(struct tabulka* zobrazenie)
{
	for(int i=0;i<MAX;i++)
	{
		struct struktura* pom=zobrazenie->odkaz[i];
		if(pom==NULL)
		{	
			printf("poradie[%d]: ",i);
			printf("-----------\n");
			continue;
		}
		printf("poradie[%d]:",i);	
		for(;;)
		{
			printf("%s ",pom->slovo);
			if(pom->dalsi==NULL)
				break;
			pom=pom->dalsi;	
		}	
		printf("\n");
	}	
}

void vysledok_hladania(bool x)
{
	if(x==true)
		printf("Prvok sa nachadza v tabulke.\n");
	else
		printf("Prvok sa nenachadza v tabulke\n");
}

int main()
{
	struct tabulka* tab=vytvor();
	char nieco[SLOVO];
	FILE* fr;
	bool f;
	if((fr=fopen("retazce.txt","r"))!=NULL);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		pridaj(tab,nieco);	
	}
	fclose(fr);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		f=hladaj(tab,nieco);
		vysledok_hladania(f);
	}
	fclose(fr);
	vypis(tab);
	pridaj(tab,nieco);	

	
	return 0;
}

/*
char generator_pismen(int cislo)
{
	char x;
	switch(cislo)
	{
		case 0:
			x='a';
			break;
		case 1:
			x='A';
			break;
		case 3:
			x='b';
			break;
		case 4:
			x='B';
			break;
		case 5:
			x='c';
			break;
		case 6:
			x='C';
			break;
		case 7:
			x='d';
			break;
		case 8:
			x='D';
			break;
		case 9:
			x='e';
			break;
		case 10:
			x='E';
			break;
		case 11:
			x='f';
			break;
		case 12:
			x='F';
			break;
		case 13:
			x='g';
			break;
		case 14:
			x='G';
			break;
		case 15:
			x='h';
			break;
		case 16:
			x='H';
			break;
		case 17:
			x='i';
			break;
		case 18:
			x='I';
			break;
		case 19:
			x='j';
			break;
		case 20:
			x='J';
			break;
		case 21:
			x='k';
			break;
		case 22:
			x='K';
			break;
		case 23:
			x='l';
			break;
		case 24:
			x='L';
			break;
		case 25:
			x='m';
			break;
		case 26:
			x='M';
			break;
		case 27:
			x='n';
			break;
		case 28:
			x='N';
			break;
		case 29:
			x='o';
			break;
		case 30:
			x='O';
			break;
		case 31:
			x='p';
			break;
		case 32:
			x='P';
			break;
		case 33:
			x='q';
			break;
		case 34:
			x='Q';
			break;
		case 35:
			x='r';
			break;
		case 36:
			x='R';
			break;
		case 37:
			x='s';
			break;
		case 38:
			x='S';
			break;
		case 39:
			x='t';
			break;
		case 40:
			x='T';
			break;
		case 41:
			x='u';
			break;
		case 42:
			x='V';
			break;
		case 43:
			x='w';
			break;
		case 44:
			x='W';
			break;
		case 45:
			x='x';
			break;
		case 46:
			x='X';
			break;
		case 47:
			x='y';
			break;
		case 48:
			x='Y';
			break;
		case 49:
			x='z';
			break;
		case 50:
			x='Z';
			break;
		case 51:
			x='#';
			break;
		case 52:
			x='@';
			break;
		case 53:
			x='!';
			break;
		case 54:
			x='?';
			break;
		case 55:
			x='_';
			break;
		case 56:
			x='-';
			break;
		case 57:
			x='+';
			break;
		case 58:
			x='*';
			break;			
		case 59:
			x='0';
			break;
		case 60:
			x='1';
			break;
		case 61:
			x='2';
			break;
		case 62:
			x='3';
			break;
		case 63:
			x='4';
			break;
		case 64:
			x='5';
			break;
		case 65:
			x='6';
			break;
		case 66:
			x='7';
			break;
		case 67:
			x='8';
			break;
		case 68:
			x='9';
			break;	
	}
	return x;
}

void generator_slov(char* retazec)
{
	srand(time(0));
	int f=0;
	for(int i=0;i<15;i++)
	{
		retazec[i]=generator_pismen((rand())%68);	
	}	
}
*/


