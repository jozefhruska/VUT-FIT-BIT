//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JOZEF HRUŠKA <xhrusk2500@stud.fit.vutbr.cz>
// $Date:       $2018-03-05
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JOZEF HRUŠKA
 *
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
  this->rootElement = NULL;
}

PriorityQueue::~PriorityQueue()
{
  if (!this->isEmpty()) {
    Element_t *iElement = this->GetHead();
    while (iElement != NULL && iElement->pNext != NULL) {
      Element_t *tempElement = iElement;
      iElement = iElement->pNext;

      delete tempElement;
    }
  } else return;
}

void PriorityQueue::Insert(int value)
{
  if (!this->isEmpty()) {
    Element_t *iElement = this->GetHead();

    while (iElement != NULL) {
        if (iElement->value < value) {
            if (iElement->pNext == NULL) {
              Element_t *tempElement = this->addElement(value, NULL, iElement);
              iElement->pNext = tempElement;
              return;
            }

            iElement = iElement->pNext;
            continue;
        }

        Element_t *tempElement = this->addElement(value, iElement, iElement->pPrev);
        if (iElement->pPrev != NULL) iElement->pPrev->pNext = tempElement;
        else this->rootElement = tempElement;

        iElement->pPrev = tempElement;
        return;
    }
  } else this->rootElement = this->addElement(value);
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
  if (!this->isEmpty()) {
    Element_t *iElement = this->GetHead();
    while (iElement != NULL) {
      if (iElement->value == value) return iElement;
      else iElement = iElement->pNext;
    }
  } return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return rootElement;
}

PriorityQueue::Element_t *PriorityQueue::addElement(int value, Element_t *pNext, Element_t *pPrev)
{
    Element_t *element = new Element_t();
    element->value = value;
    element->pNext = pNext;
    element->pPrev = pPrev;
    return element;
}

bool PriorityQueue::Remove(int value) {
  Element_t *iElement = this->Find(value);
  if (iElement != NULL) {
    if (iElement->pPrev != NULL) iElement->pPrev->pNext = iElement->pNext;
    else this->rootElement = iElement->pNext;

    if (iElement->pNext != NULL) iElement->pNext->pPrev = iElement->pPrev;
    delete(iElement);
    return true;
  } return false;
}

bool PriorityQueue::isEmpty() {
  return (this->GetHead() == NULL);
}

/*** Konec souboru tdd_code.cpp ***/
