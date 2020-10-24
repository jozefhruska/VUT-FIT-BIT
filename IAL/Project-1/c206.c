
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
	if ((L = malloc(sizeof(tDLList)))) {
		L->Act = NULL;
		L->First = NULL;
		L->Last = NULL;
	}
}

void DLDisposeList (tDLList *L) {
	if (L != NULL && L->First != NULL) {
		while (L->First != NULL) {
			L->First = L->Act->rptr;
			free(L->Act);
			L->Act = L->First;
		}

		L->Last = NULL;
	}
}

void DLInsertFirst (tDLList *L, int val) {
	tDLElemPtr tempElem = L->First;
	if ((L->First = malloc(sizeof(struct tDLElem))) != NULL) {
		L->First->lptr = NULL;
		L->First->rptr = tempElem;

		if (tempElem == NULL) L->Last = L->First;
		else tempElem->lptr = L->First;
		L->First->data = val;
	} else DLError();
}

void DLInsertLast(tDLList *L, int val) {
	tDLElemPtr tempElem = L->Last;
	if ((L->Last = malloc(sizeof(struct tDLElem))) != NULL) {
		L->Last->lptr = tempElem;
		L->Last->rptr = NULL;

		if (tempElem == NULL) L->First = L->Last;
		else tempElem->rptr = L->Last;
		L->Last->data = val;
	} else DLError();
}

void DLFirst (tDLList *L) {
	L->Act = L->First;
}

void DLLast (tDLList *L) {
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
	if (L != NULL && L->First != NULL) {
		*val = L->First->data;
	} else DLError();
}

void DLCopyLast (tDLList *L, int *val) {
	if (L != NULL && L->Last != NULL) {
		*val = L->Last->data;
	} else DLError();
}

void DLDeleteFirst (tDLList *L) {
	if (L != NULL && L->First != NULL) {
		tDLElemPtr tempElem = L->First->rptr;
		free(L->First);
		L->First = tempElem;

		if (L->First != NULL) L->First->lptr = NULL;
	}
}

void DLDeleteLast (tDLList *L) {
	if (L != NULL && L->Last != NULL) {
		tDLElemPtr tempElem = L->Last->lptr;
		free(L->Last);
		L->Last = tempElem;

		if (L->Last != NULL) L->Last->rptr = NULL;
	}
}

void DLPostDelete (tDLList *L) {
	if (L->Act != NULL && L->Act != L->Last) {
		tDLElemPtr tempElem = L->Act->rptr;
		L->Act->rptr = tempElem->rptr;

		if (L->Last == tempElem) L->Last = L->Act;
		else tempElem->rptr->lptr = L->Act;
	}
}

void DLPreDelete (tDLList *L) {
	if (L->Act != NULL && L->Act != L->First) {
		tDLElemPtr tempElem = L->Act->lptr;
		L->Act->lptr = tempElem->lptr;

		if (L->First == tempElem) L->First = L->Act;
		else tempElem->lptr->rptr = L->Act;
	}
}

void DLPostInsert (tDLList *L, int val) {
	if (L->Act != NULL) {

		tDLElemPtr tempElem;
		if ((tempElem = malloc(sizeof(struct tDLElem)))) {
			if (L->Act == L->Last) L->Last = tempElem;
			else L->Act->rptr->lptr = tempElem;

			tempElem->rptr = L->Act->rptr;
			L->Act->rptr = tempElem;
			tempElem->lptr = L->Act;

			tempElem->data=val;
		} else DLError();
	}
}

void DLPreInsert (tDLList *L, int val) {
	if (L->Act != NULL) {

		tDLElemPtr tempElem;
		if ((tempElem = malloc(sizeof(struct tDLElem)))) {
			if (L->Act == L->First) L->First = tempElem;
			else L->Act->lptr->rptr = tempElem;

			tempElem->lptr = L->Act->lptr;
			L->Act->lptr = tempElem;
			tempElem->rptr = L->Act;

			tempElem->data=val;
		} else DLError();
	}
}

void DLCopy (tDLList *L, int *val) {
	if (L->Act != NULL) *val = L->Act->data;
	else DLError();
}

void DLActualize (tDLList *L, int val) {
	if (L->Act != NULL) L->Act->data = val;
	else DLError();
}

void DLSucc (tDLList *L) {
	if (L->Act != NULL) L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
	if (L->Act != NULL) L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
	return (L->Act != NULL) ? 1 : 0;
}

/* Konec c206.c*/
