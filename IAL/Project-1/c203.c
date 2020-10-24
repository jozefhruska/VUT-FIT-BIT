
/* ******************************* c203.c *********************************** */
/*  P�edm�t: Algoritmy (IAL) - FIT VUT v Brn�                                 */
/*  �kol: c203 - Fronta znak� v poli                                          */
/*  Referen�n� implementace: Petr P�ikryl, 1994                               */
/*  P�epis do jazyka C: V�clav Topinka, z��� 2005                             */
/*  �pravy: Kamil Je��bek, z��� 2018                                          */
/* ************************************************************************** */
/*
** Implementujte frontu znak� v poli. P�esnou definici typ� naleznete
** v hlavi�kov�m souboru c203.h (ADT fronta je reprezentov�na strukturou tQueue,
** kter� obsahuje pole 'arr' pro ulo�en� hodnot ve front� a indexy f_index
** a b_index. V�echny implementovan� funkce mus� p�edpokl�dat velikost pole
** QUEUE_SIZE, i kdy� ve skute�nosti jsou rozm�ry statick�ho pole definov�ny
** MAX_QUEUE. Hodnota QUEUE_SIZE slou�� k simulaci fronty v r�zn� velk�m poli
** a nastavuje se v testovac�m skriptu c203-test.c p�ed testov�n�m
** implementovan�ch funkc�. Hodnota QUEUE_SIZE m��e nab�vat hodnot v rozsahu
** 1 a� MAX_QUEUE.
**
** Index f_index ukazuje v�dy na prvn� prvek ve front�. Index b_index
** ukazuje na prvn� voln� prvek ve front�. Pokud je fronta pr�zdn�, ukazuj�
** oba indexy na stejn� m�sto. Po inicializaci ukazuj� na prvn� prvek pole,
** obsahuj� tedy hodnotu 0. Z uveden�ch pravidel vypl�v�, �e v poli je v�dy
** minim�ln� jeden prvek nevyu�it�.
**
** P�i libovoln� operaci se ��dn� z index� (f_index i b_index) nesni�uje
** vyjma p��padu, kdy index p�es�hne hranici QUEUE_SIZE. V tom p��pad�
** se "posunuje" znovu na za��tek pole. Za t�mto ��elem budete deklarovat
** pomocnou funkci NextIndex, kter� pro kruhov� pohyb p�es indexy pole
** vyu��v� operaci "modulo".
**
** Implementujte n�sleduj�c� funkce:
**
**    nextIndex ..... pomocn� funkce - viz popis v��e
**    queueInit ..... inicializace fronty
**    queueEmpty .... test na pr�zdnost fronty
**    queueFull ..... test, zda je fronta zapln�na (vy�erp�na kapacita)
**    queueFront .... p�e�te hodnoty prvn�ho prvku z fronty
**    queueRemove ... odstran� prvn� prvek fronty
**    queueGet ...... p�e�te a odstran� prvn� prvek fronty
**    queueUp ....... za�azen� prvku na konec fronty
**
** Sv� �e�en� ��eln� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**
**/

#include "c203.h"

void queueError (int error_code) {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**
** TUTO FUNKCI, PROS�ME, NEUPRAVUJTE!
*/
	static const char* QERR_STRINGS[MAX_QERR+1] = {"Unknown error","Queue error: UP","Queue error: FRONT","Queue error: REMOVE","Queue error: GET","Queue error: INIT"};
	if ( error_code <= 0 || error_code > MAX_QERR )
		error_code = 0;
	printf ( "%s\n", QERR_STRINGS[error_code] );
	err_flag = 1;
}

void queueInit (tQueue* q) {
	if (q != NULL) {
		for (int i = 0; i < QUEUE_SIZE; i++) q->arr[i] = '*';

		q->b_index = 0;
		q->f_index = 0;
	} else queueError(QERR_INIT);
}

int nextIndex (int index) {
	return ((index + 1) % QUEUE_SIZE);
}

int queueEmpty (const tQueue* q) {
	return (q == NULL || q->b_index == q->f_index) ? 1 : 0;
}

int queueFull (const tQueue* q) {
	return (nextIndex(q->b_index) == q->f_index) ? 1 : 0;
}

void queueFront (const tQueue* q, char* c) {
	if (!queueEmpty(q)) {
		*c = q->arr[q->f_index];
	} else queueError(QERR_FRONT);
}

void queueRemove (tQueue* q) {
	if (!queueEmpty(q)) {
		q->f_index = nextIndex(q->f_index);
	} else queueError(QERR_REMOVE);
}

void queueGet (tQueue* q, char* c) {
	if (!queueEmpty(q)) {
		queueFront(q, c);
		queueRemove(q);
	} else queueError(QERR_GET);
}

void queueUp (tQueue* q, char c) {
	if (!queueFull(q)) {
		q->arr[q->b_index] = c;
		q->b_index = nextIndex(q->b_index);
	} else queueError(QERR_UP);
}
/* Konec p��kladu c203.c */
