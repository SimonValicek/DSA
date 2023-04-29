#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 10000
#define SLOVO 22
#include<stdint.h>
#include<stdbool.h>
#define DELETED_NOTE (struct struktura_t1*) (0xFFFFFFFUL)
#include<time.h>



struct bunka{
	char* slovo;
	int vyska;
	struct bunka* prava;
	struct bunka* lava;
};

struct bunka* novy_prvok(char* slovo)
{
	struct bunka* nova=(struct bunka*)malloc(sizeof(struct bunka)*1);
	nova->slovo=(char*)malloc(SLOVO*sizeof(char));
	nova->vyska=1;
	nova->prava=NULL;
	nova->lava=NULL;
	strcpy(nova->slovo,slovo);
	return nova;
}

int vacsi(int a, int b)
{
	return (a>b)?a:b;
}

int vyska(struct bunka* pom)
{
	if(pom==NULL)
		return 0;
	return pom->vyska;

}

/*
int vyska(struct bunka* pom)
{
	if(pom!=NULL)
		return pom->vyska;
	return 0;
}
*/
/*
struct bunka* lava_rotacia(struct bunka* pom)
{
	struct bunka* nova=pom->prava;
	struct bunka* nova_pomocna=nova->lava;
	nova->lava=pom;
	pom->prava=nova_pomocna;

	pom->vyska = vacsi(vyska(pom->prava),vyska(pom->lava)) +1;
	nova->vyska = vacsi(vyska(pom->prava),vyska(pom->lava)) +1;

	return nova;
}
*/
/*
struct bunka* prava_rotacia(struct bunka* pom)
{
	struct bunka* nova=pom->lava;
	struct bunka* nova_pomocna=nova->prava;
	nova->prava=pom;
	pom->lava=nova_pomocna;

	pom->vyska = vacsi(vyska(pom->prava),vyska(pom->lava))+1;
	nova->vyska = vacsi(vyska(nova->prava),vyska(nova->lava))+1;

	return nova;
}
*/

struct bunka* prava_rotacia(struct bunka* pom)
{
	struct bunka* nova;
	struct bunka* nova_pomocna;
	nova=pom->lava;
	nova_pomocna=nova->prava;
	nova->prava=pom;
	pom->lava=nova_pomocna;

	pom->vyska = vacsi(vyska(pom->prava),vyska(pom->lava));
	nova->vyska = vacsi(vyska(pom->prava),vyska(pom->lava));

	return nova;
}



struct bunka* lava_rotacia(struct bunka* pom)
{
	struct bunka* nova;
	struct bunka* nova_pomocna;
	nova=pom->prava;
	nova_pomocna=nova->lava;
	nova->lava=pom;
	pom->prava=nova_pomocna;

	pom->vyska = vacsi(vyska(pom->prava),vyska(pom->lava));
	nova->vyska = vacsi(vyska(nova->prava),vyska(nova->lava));

	return nova;
}


int vybalancuj(struct bunka* pom)
{
	if(pom==NULL)
		return 0;
	return vyska(pom->lava)-vyska(pom->prava);
}

/*
int vybalancuj(struct bunka* pom)
{
	if(pom!=NULL)
		return vyska(pom->lava)-vyska(pom->prava);
	return 0;
}
*/

/*
char* vacsi_retazec(char* slovo1,char* slovo2)
{
	if(strcmp(slovo1,slovo2)==1)
		return slovo1;
	else if(strcmp(slovo1,slovo2)==-1)
		return slovo2;
	return NULL;
}
*/

struct bunka* minimum(struct bunka* pom)	//tuto to sk�sim zjednodu�i�
{
	struct bunka* nova=pom;
	while(nova->lava!=NULL)
		nova=nova->lava;
	return nova;
}

struct bunka* vlozit_strom(struct bunka* pom,char* slovo)
{
	if(pom==NULL)
		return(novy_prvok(slovo));
	if(strcmp(slovo,pom->slovo)==-1)
		pom->lava=vlozit_strom(pom->lava,slovo);
	else if(strcmp(slovo,pom->slovo)==1)
		pom->prava=vlozit_strom(pom->prava,slovo);
	return pom;

	pom->vyska=1+vacsi(vyska(pom->lava),vyska(pom->prava));
	int pomocna=vybalancuj(pom);
	if(pomocna>1&&(strcmp(slovo,pom->lava->slovo)==-1))
		return prava_rotacia(pom);
	if(pomocna<-1&&(strcmp(slovo,pom->prava->slovo)==1))
		return lava_rotacia(pom);
	if(pomocna>1&&(strcmp(slovo,pom->lava->slovo)==1))
	{
		pom->lava=lava_rotacia(pom->lava);
		return prava_rotacia(pom);
	}
	if(pomocna<-1&&(strcmp(slovo,pom->prava->slovo)==-1))
	{
		pom->prava=prava_rotacia(pom->prava);
		return lava_rotacia(pom);
	}
	return pom;
}

/*
struct bunka* vlozit(struct bunka* pom,char* slovo) //pri ifoch boli chyby v rot�ci�ch
{
	if(pom==NULL)
		return(novy_prvok(slovo));
	if(strcmp(slovo,pom->slovo)==-1)
		pom->prava=vlozit(pom->prava,slovo);
	else if(strcmp(slovo,pom->slovo)==1)
		pom->lava=vlozit(pom->lava,slovo);
	else
		return pom;
	pom->vyska=1+vacsi(vyska(pom->lava),vyska(pom->prava));
	int pomocna=vybalancuj(pom);
	int i=0;
	if(pomocna>1&&strcmp(slovo,pom->lava->slovo)==-1)	//case 1
		i=1;
	if(pomocna>1&&strcmp(slovo,pom->lava->slovo)==1)	//case 3
		i=2;
	if(pomocna<-1&&strcmp(slovo,pom->prava->slovo)==-1)	 //case 4
		i=3;
	if(pomocna<-1&&strcmp(slovo,pom->prava->slovo)==1)	//case 2
		i=4;
	switch(i)
	{
		case 1:
			return prava_rotacia(pom);
			break;
		case 2:
			pom->lava=lava_rotacia(pom->lava);
			return prava_rotacia(pom);
			break;
		case 3:
			pom->prava=prava_rotacia(pom->prava);
			return lava_rotacia(pom);
			break;
		case 4:
			return lava_rotacia(pom);
			break;
	}
	return pom;
}
*/

struct bunka* odstran_strom(struct bunka* pom, char* slovo)
{
	struct bunka* pomocna;
	if(pom==NULL)
		return pom;
	if(strcmp(slovo,pom->slovo)==1)
		pom->prava=odstran_strom(pom->prava,slovo);
	else if(strcmp(slovo,pom->slovo)==-1)
		pom->lava=odstran_strom(pom->lava,slovo);
	else
	{
		if(pom->prava==NULL&&pom->lava==NULL)
		{
			struct bunka* pomocna=pom->lava ? pom->lava : pom->prava;
			if(pomocna==NULL)
			{
				pomocna=pom;
				pom=NULL;
			}
			else
				pom=pomocna;
			free(pomocna);
		}
		else
		{
			struct bunka* pomocna=minimum(pom->prava);
			strcpy(pom->slovo,pomocna->slovo);
			pom->prava=odstran_strom(pom->prava,pomocna->slovo);
		}
	}
	if(pom==NULL)
		return pom;
	pom->vyska=1+vacsi(vyska(pom->prava),vyska(pom->lava));
	int pom_int=vybalancuj(pom);
	if(vybalancuj(pom->prava)>0&&pom_int<-1)
	{
		pom->prava=prava_rotacia(pom->prava);
		return lava_rotacia(pom);
	}
	if(vybalancuj(pom->prava)<=0&&pom_int<-1)
		return lava_rotacia(pom);
	if(vybalancuj(pom->lava)<0&&pom_int>1)
	{
		pom->lava=lava_rotacia(pom->lava);
		return prava_rotacia(pom);
	}
	if(vybalancuj(pom->lava)>=0&&pom_int>1)
		return prava_rotacia(pom);
	return pom;
}

/*
struct bunka* odstran(struct bunka* pom, char* slovo)
{
	struct bunka* pomocna;
	if(pom!=NULL)
	{
		if(strcmp(slovo,pom->slovo)==1)
			pom->prava=odstran(pom->prava,slovo);
		else if(strcmp(slovo,pom->slovo)==-1)
			pom->lava=odstran(pom->lava,slovo);
		else
		{
			if(pom->prava==NULL&&pom->lava==NULL)
			{

				pomocna=pom->lava ? pom->lava : pom->prava;
				if(pomocna!=NULL)
					*pom=*pomocna;
				else
				{
					pomocna=pom;
					pom=NULL;
				}
				free(pomocna);
			}
			else
			{
				pomocna=minimum(pom->prava);
				strcpy(pom->slovo,pomocna->slovo);
				pom->prava=odstran(pom->prava,pomocna->slovo);
			}
		}
	}
	else
		return pom;
	if(pom==NULL)
		return pom;
	pom->vyska=1+vacsi(vyska(pom->prava),vyska(pom->lava));
	int pom_int=vybalancuj(pom);
	int i=0;
	if(vybalancuj(pom->prava)>0&&pom_int<-1)
	{
		pom->prava=prava_rotacia(pom->prava);
		return lava_rotacia(pom);
	}
	if(vybalancuj(pom->prava)<=0&&pom_int<-1)
		return lava_rotacia(pom);
	if(vybalancuj(pom->lava)<0&&pom_int>1)
	{
		pom->lava=lava_rotacia(pom->lava);
		return prava_rotacia(pom);
	}
	if(vybalancuj(pom->lava)>=0&&pom_int>1)
		return prava_rotacia(pom);
	return pom;

}
*/

void vypis_strom(struct bunka* pom)
{
	if(pom==NULL)
	{
		return;
	}
	printf("%s\n",pom->slovo);
	vypis_strom(pom->lava);
	vypis_strom(pom->prava);
}

bool hladaj_strom(struct bunka* prva, char* retazec)
{
	if(prva==NULL)
		return false;
	if(strcmp(prva->slovo,retazec)==0)
		return true;
	if(strcmp(prva->slovo,retazec)==1)
		return hladaj_strom(prva->lava,retazec);
	if(strcmp(prva->slovo,retazec)==-1)
		return hladaj_strom(prva->prava,retazec);
}

void vysledok_hladania_strom(bool f)
{
	//if(f==false)
	//	printf("Retazec sa nenasiel\n");
//	else
	//	printf("Retazec sa nachadza v tabulke\n");
}

struct struktura_t1{
	char* slovo;
};

struct struktura_t1* nova_struktura(char* nieco)
{
	struct struktura_t1* pom=(struct struktura_t1*)malloc(sizeof(struct struktura_t1)*1);
	pom->slovo=(char*)malloc(SLOVO*(sizeof(char)));
	strcpy(pom->slovo,nieco);
//	printf("%s %s ",pom->slovo,nieco);
	return pom;
}

struct struktura_t1* tabulka_t1[MAX];

int hash_t1(char* slovo)
{
	int dlzka=strlen(slovo), hodnota=0;
	for(int i=0;i<dlzka;i++)
		hodnota+=slovo[i];
	hodnota%=MAX;
	return hodnota;
}

void vytvor_t1()
{
	for(int i=0;i<MAX;i++)
		tabulka_t1[i]=NULL;
}

bool pridaj_t1(struct struktura_t1* retazec)
{
	if(retazec==NULL)
		return false;
	int pozicia=hash_t1(retazec->slovo);
	for (int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka_t1[nahrada]==NULL)
		{
			tabulka_t1[nahrada]=retazec;
			return  true;
		}
	}
	return false;
}

struct struktura_t1* zisti_t1(char* slovo)
{
	int pozicia=hash_t1(slovo);
	for(int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka_t1[nahrada]==NULL)
			return false;
		if(tabulka_t1[nahrada]==DELETED_NOTE)
			continue;
		if(strcmp(tabulka_t1[nahrada]->slovo,slovo)==0)
			return  tabulka_t1[nahrada];
	}
	return NULL;
}

struct struktura_t1* odstran_t1(char* slovo)
{
	int pozicia=hash_t1(slovo);
	for(int i=0;i<MAX;i++)
	{
		int nahrada=(i+pozicia)%MAX;
		if(tabulka_t1[nahrada]==NULL)
			return false;
		if(tabulka_t1[nahrada]=DELETED_NOTE)
			continue;
		if(strcmp(tabulka_t1[nahrada]->slovo,slovo)==0)
		{
			struct struktura_t1* pomocna=tabulka_t1[nahrada];
			tabulka_t1[nahrada]=DELETED_NOTE;
			return pomocna;
		}

	}
	return NULL;
}

void vypis_t1()
{
	printf("Zaciatok\n");
	for(int i=0;i<MAX;i++)
	{
		if(tabulka_t1[i]==NULL)
			printf("%d\t---\n",i);
		else if(tabulka_t1[i]==DELETED_NOTE)
			printf("%d\t<vymazane>\n",i);
		else
			printf("%d\t%s\n",i,tabulka_t1[i]->slovo);
	}
	printf("Koniec\n");
}

struct struktura_t2{
	char *slovo;
	struct struktura_t2* dalsi;
};

struct tabulka_t2{
	struct struktura_t2** odkaz;
};

int hash_t2(char slovo[])
{
	int dlzka=strlen(slovo), hodnota=0;
	for(int i=0;i<dlzka;i++)
		hodnota+=slovo[i];
	hodnota=hodnota%MAX;
	return hodnota;
}

struct struktura_t2* vytvor_prvok_t2(char* slovo)
{
	struct struktura_t2* pom=(struct struktura_t2*)malloc(sizeof(struct struktura_t2)*1);
	pom->slovo=(char*)malloc(strlen(slovo)*(sizeof(char)+1));
	strcpy(pom->slovo,slovo);
	pom->dalsi=NULL;
	return pom;
}

struct tabulka_t2* vytvor()
{
	struct tabulka_t2* pom=(struct tabulka_t2*)malloc(sizeof(struct tabulka_t2)*1);
	pom->odkaz=(struct struktura_t2**)malloc(MAX*sizeof(struct struktura_t2*));
	for(int i=0;i<MAX;i++)
		pom->odkaz[i]=NULL;
	return pom;
}

void pridaj_t2(struct tabulka_t2* pom,char* slovo)
{
	int kluc=hash_t2(slovo);
	struct struktura_t2* prvok=pom->odkaz[kluc];
	struct struktura_t2* predchadzajuci=NULL;
	struct struktura_t2* pomocny=NULL;
	if(prvok==NULL)
	{
		pom->odkaz[kluc]=vytvor_prvok_t2(slovo);
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
	predchadzajuci->dalsi=vytvor_prvok_t2(slovo);
}

void odstran_t2(struct tabulka_t2* pom,char* slovo)
{
	int kluc=hash_t2(slovo);
	struct struktura_t2* prvok=pom->odkaz[kluc];
	for(int i=0;i<MAX;i++)
	{
		struct struktura_t2* predchadzajuci=pom->odkaz[i];
		if(strcmp(predchadzajuci->slovo,prvok->slovo)==0)
 		{
			struct struktura_t2* pomocna=pom->odkaz[i-1];
			pomocna->dalsi=pom->odkaz[i+1];
			free(prvok->slovo);
			free(pom->odkaz[i]);
			return;
		}
	}
	return;
}


bool hladaj_t2(struct tabulka_t2* pom, char* slovo)
{
	int kluc=hash_t2(slovo);
	struct struktura_t2* pom_prem=pom->odkaz[kluc];
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

void vypis_t2(struct tabulka_t2* zobrazenie)
{
	for(int i=0;i<MAX;i++)
	{
		struct struktura_t2* pom=zobrazenie->odkaz[i];
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

void vysledok_hladania_t2(bool x)
{
	//if(x==true)
	//	printf("Prvok sa nachadza v tabulke.\n");
	//else
	//	printf("Prvok sa nenachadza v tabulke\n");
}

int main()
{
	struct bunka* koren=NULL;
	char nieco[SLOVO];
	FILE* fr;
	bool f;
	clock_t start_1,end_1,total_1,start_2,end_2,total_2,start_3,end_3,total_3,start_4,end_4,total_4,start_5,end_5,total_5,start_6,end_6,total_6,start_7,end_7,total_7,start_8,end_8,total_8;
	if((fr=fopen("retazce.txt","r"))!=NULL);
	start_1=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		koren=vlozit_strom(koren,nieco);
	}
	end_1=clock();
	printf("Vkladanie prvkov do stromu sa vykonalo za %f sekund\n",(float)(end_1 - start_1) / CLOCKS_PER_SEC);

	fclose(fr);
	start_2=clock();
	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		f=hladaj_strom(koren,nieco);
		vysledok_hladania_strom(f);
	}
	end_2=clock();
	printf("Vyhladavanie prvkov v strome sa vykonalo za %f sekund\n",(float)(end_2 - start_2) / CLOCKS_PER_SEC);
	fclose(fr);
//	vypis_strom(koren);
	start_3=clock();
	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		koren=odstran_strom(koren,nieco);
	}
	end_3=clock();
	printf("Odstranovanie prvkov zo stromu sa vykonalo za %f sekund\n",(float)(end_3 - start_3) / CLOCKS_PER_SEC);
	fclose(fr);
/*	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		f=hladaj_strom(koren,nieco);
		vysledok_hladania_strom(f);
	}*/
	fclose(fr);
	vytvor_t1();
	struct struktura_t1* pom=NULL;
	if((fr=fopen("retazce.txt","r"))!=NULL);
	start_4=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		pom=nova_struktura(nieco);
		pridaj_t1(pom);
	}
	end_4=clock();
	printf("Vkladanie prvkov do prvej tabulky sa vykonalo za %f sekund\n",(float)(end_4 - start_4) / CLOCKS_PER_SEC);
//	vypis_t1();
	fclose(fr);
	start_5=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		zisti_t1(nieco);
	}
	end_5=clock();
	printf("Vyhladavanie prvkov v prvej tabulke sa vykonalo za %f sekund\n",(float)(end_5 - start_5) / CLOCKS_PER_SEC);
	fclose(fr);
	start_6=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		odstran_t1(nieco);
	}
	end_6=clock();
	printf("Odstranovanie prvkov z prvej tabulky sa vykonalo za %f sekund\n",(float)(end_6 - start_6) / CLOCKS_PER_SEC);
	fclose(fr);
//	vypis_t1();

	struct tabulka_t2* tab=vytvor();
	if((fr=fopen("retazce.txt","r"))!=NULL);
	start_7=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		pridaj_t2(tab,nieco);
	}
	end_7=clock();
	printf("Pridavanie prvkov do druhej tabulky sa vykonalo za %f sekund\n",(float)(end_7 - start_7) / CLOCKS_PER_SEC);
	fclose(fr);
	start_8=clock();
	for(int i=0;i<(MAX);i++)
	{
		fgets(nieco, SLOVO, fr);
		nieco[strlen(nieco)-1]=0;
		f=hladaj_t2(tab,nieco);
		vysledok_hladania_t2(f);
	}
	end_8=clock();
	printf("Vyhladavanie prvkov v druhej tabulke sa vykonalo za %f sekund\n",(float)(end_8 - start_8) / CLOCKS_PER_SEC);
	fclose(fr);
//	vypis_t2(tab);
//	pridaj_t2(tab,nieco);

	return 0;
}
