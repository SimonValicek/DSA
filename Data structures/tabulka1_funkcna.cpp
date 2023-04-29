#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#define SLOVO 20
#define DELETED_NOTE (struct struktura*) (0xFFFFFFFUL)
#define MAX 1000

struct struktura{
	char* slovo;
};

struct struktura* nova_struktura(char* nieco)
{
	struct struktura* pom=(struct struktura*)malloc(sizeof(struct struktura)*1);
	pom->slovo=(char*)malloc(SLOVO*(sizeof(char)));
	strcpy(pom->slovo,nieco);
//	printf("%s %s ",pom->slovo,nieco);
	return pom;
}

struct struktura* tabulka[MAX];

int hash(char* slovo)
{
	int dlzka=strlen(slovo), hodnota=0;
	for(int i=0;i<dlzka;i++)
		hodnota+=slovo[i];	
	hodnota%=MAX;	
	return hodnota;	
}

void vytvor()
{
	for(int i=0;i<MAX;i++)
		tabulka[i]=NULL;
}

bool pridaj(struct struktura* retazec)
{
	if(retazec==NULL)
		return false;
	int pozicia=hash(retazec->slovo);
	for (int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka[nahrada]==NULL)
		{
			tabulka[nahrada]=retazec;
			return  true;
		}
	}
	return false;	
}

struct struktura* zisti(char* slovo)
{
	int pozicia=hash(slovo);
	for(int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka[nahrada]==NULL)
			return false;
		if(tabulka[nahrada]==DELETED_NOTE)
			continue;	
		if(strcmp(tabulka[nahrada]->slovo,slovo)==0)
			return  tabulka[nahrada];
	} 
	return NULL;
}

struct struktura* odstran(char* slovo)
{
	int pozicia=hash(slovo);
	for(int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka[nahrada]==NULL)
			return false;
		if(tabulka[nahrada]=DELETED_NOTE)
			continue;
		if(strcmp(tabulka[nahrada]->slovo,slovo)==0)
		{
			struct struktura* pomocna=tabulka[nahrada];
			tabulka[nahrada]=DELETED_NOTE;
			return pomocna;
		}
		
	} 
	return NULL;
}

void vypis()
{
	printf("Zaciatok\n");
	for(int i=0;i<MAX;i++)
	{
		if(tabulka[i]==NULL)
			printf("%d\t---\n",i);
		else if(tabulka[i]==DELETED_NOTE)
			printf("%d\t<vymazane>\n",i);
		else
			printf("%d\t%s\n",i,tabulka[i]->slovo);
	}
	printf("Koniec\n");
}



int main()
{
	vytvor();
	struct struktura* pom=NULL;
	char nieco[SLOVO];
	
	FILE* fr;
	bool f;
	if((fr=fopen("retazce.txt","r"))!=NULL);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		pom=nova_struktura(nieco);
		pridaj(pom);	
	}
	vypis();
	fclose(fr);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		zisti(nieco);
	}
	fclose(fr);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		odstran(nieco);
	}
	fclose(fr);
	vypis();
	
	
/*	
	gets(nieco);
	pom=nova_struktura(nieco);
	printf("%s %d\n",pom->slovo,hash(nieco));
	pridaj(pom);
	fflush(stdin);

	gets(nieco);
	pom=nova_struktura(nieco);
	printf("%s %d\n",pom->slovo,hash(nieco));
	pridaj(pom);
	fflush(stdin);
	
	gets(nieco);
	pom=nova_struktura(nieco);
	printf("%s %d\n",pom->slovo,hash(nieco));
	pridaj(pom);
	fflush(stdin);
	vypis();
	
	gets(nieco);
	pom=nova_struktura(nieco);
	printf("%s %d\n",pom->slovo,hash(nieco));
	odstran(pom->slovo);
	fflush(stdin);
	vypis();
*/
	return 0;
}
