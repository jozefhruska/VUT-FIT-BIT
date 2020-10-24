
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
		printf ("*ERROR* The program has performed an illegal operation.\n");
		errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
	if ((L = malloc(sizeof(tList)))) {
		L->Act = NULL;
		L->First = NULL;
	} else Error();
}

void DisposeList (tList *L) {
	if (L != NULL) {
		while(L->First != NULL) {
			tElemPtr tempElem = L->First->ptr;
			free(L->First);
			L->First = tempElem;
		}

		L->Act = NULL;
	}
}

void InsertFirst (tList *L, int val) {
	if (L != NULL && val) {
		tElemPtr tempElem = L->First;
		if ((L->First = malloc(sizeof(struct tElem)))) {
			L->First->data = val;
			L->First->ptr = tempElem;
		} else Error();
	}
}

void First (tList *L) {
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
	if (L != NULL && L->First != NULL) {
		*val = L->First->data;
	} else Error();
}

void DeleteFirst (tList *L) {
	if (L->First != NULL && L->First != NULL) {
		if (L->Act == L->First) L->Act = NULL;

		tElemPtr tempElem = L->First;
		free(L->First);
		L->First = tempElem->ptr;
	}
}

void PostDelete (tList *L) {
	if (L->Act != NULL && L->Act->ptr != NULL) {
		tElemPtr tempElem = L->Act->ptr->ptr;
		free(L->Act->ptr);
		L->Act->ptr = tempElem;
	}
}

void PostInsert (tList *L, int val) {
	if (L != NULL && L->Act != NULL && val) {
			tElemPtr tempElem = L->Act->ptr;
			if ((L->Act->ptr = malloc(sizeof(struct tElem)))) {
				L->Act->ptr->data = val;
				L->Act->ptr->ptr = tempElem;
			} else Error();
	}
}

void Copy (tList *L, int *val) {
	if (L->Act != NULL) *val = L->Act->data;
	else Error();
}

void Actualize (tList *L, int val) {
	if (L->Act != NULL) L->Act->data = val;
	else Error();
}

void Succ (tList *L) {
	if (L->Act != NULL) L->Act = L->Act->ptr;
}

int Active (tList *L) {
	return (L->Act != NULL) ? 1 : 0;
}

/* Konec c201.c */
