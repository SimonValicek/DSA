#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100000
#define SLOVO 20

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

struct bunka* minimum(struct bunka* pom)	//tuto to skúsim zjednoduši
{
	struct bunka* nova=pom;
	while(nova->lava!=NULL)
		nova=nova->lava;
	return nova;	
}

struct bunka* vlozit(struct bunka* pom,char* slovo)
{
	if(pom==NULL)
		return(novy_prvok(slovo));
	if(strcmp(slovo,pom->slovo)==-1)
		pom->lava=vlozit(pom->lava,slovo);		
	else if(strcmp(slovo,pom->slovo)==1)
		pom->prava=vlozit(pom->prava,slovo);	
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
struct bunka* vlozit(struct bunka* pom,char* slovo) //pri ifoch boli chyby v rotáciách
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

struct bunka* odstran(struct bunka* pom, char* slovo)
{
	struct bunka* pomocna;
	if(pom==NULL)
		return pom;
	if(strcmp(slovo,pom->slovo)==1)
		pom->prava=odstran(pom->prava,slovo);
	else if(strcmp(slovo,pom->slovo)==-1)
		pom->lava=odstran(pom->lava,slovo);
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
			pom->prava=odstran(pom->prava,pomocna->slovo);
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

void vypis(struct bunka* pom)
{
	if(pom==NULL)
	{
		return;
	}
	printf("%s\n",pom->slovo);
	vypis(pom->lava);
	vypis(pom->prava);
}

bool hladaj(struct bunka* prva, char* retazec)
{
	if(prva==NULL)
		return false;
	if(strcmp(prva->slovo,retazec)==0)
		return true;
	if(strcmp(prva->slovo,retazec)==1)
		return hladaj(prva->lava,retazec);
	if(strcmp(prva->slovo,retazec)==-1)	
		return hladaj(prva->prava,retazec);
}

void vysledok_hladania(bool f)
{
	if(f==false)
		printf("Retazec sa nenasiel\n");
	else
		printf("Retazec sa nachadza v tabulke\n");	
}

int main()
{
	struct bunka* koren=NULL;
	char abeceda[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char nieco[SLOVO];
	FILE* fr;
	bool f;
	if((fr=fopen("retazce.txt","r"))!=NULL);
	for(int i=0;i<(MAX);i++)
	{	
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		koren=vlozit(koren,nieco);	
	}
	fclose(fr);
	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		f=hladaj(koren,nieco);
		vysledok_hladania(f);
	}
	fclose(fr);
	vypis(koren);
	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		koren=odstran(koren,nieco);
	}
	fclose(fr);
	for(int i=0;i<MAX;i++)
	{
		fgets(nieco, SLOVO, fr);	
		nieco[strlen(nieco)-1]=0;
		f=hladaj(koren,nieco);
		vysledok_hladania(f);
	}
	fclose(fr);
	return 0;
}
