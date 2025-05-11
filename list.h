#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//doubly linked list

template <typename T>
struct Node
{ //structure for a list's node; it has the node's info and 2 pointers
    T info;
    Node <T> *next; //pointer to the next node
    Node <T> *prev; //pointer to the previous node
};

template <typename T>
class LinkedList
{
    public:
        Node <T> *pfirst; //pointer to the first node
        Node <T> *plast; //pointer to the last node

    void addFirst(T x)
    {
        //add info x on the first position of the list

        Node <T> *paux = new Node<T>;

        paux->info = x; // ou (*paux).info = x;
        paux->prev = NULL;
        paux->next = pfirst;

        if (pfirst != NULL)
            pfirst->prev = paux;

        pfirst = paux; //we link the old "first" with the new one

        if (plast==NULL)
            plast=pfirst;
    }

    void addLast(T x)
    {
        //add info x on the last position of the list

        Node<T> *paux = new Node <T>;

        paux->info = x;
        paux->prev = plast;
        paux->next = NULL;

        if (plast != NULL)
            plast->next = paux;

        plast = paux;

        if (pfirst == NULL)
            pfirst = plast;
    }

    T getInfo (Node<T>* p)
    {
		//return info from a specific node
        return p->info;
    }

    void removeFirst()
    {
        //removes the first element of the list
        Node<T>* paux;

        if (pfirst != NULL)
        {
            paux = pfirst->next;

            if (pfirst == plast)
                plast = NULL;

            delete pfirst; //delete the first node of the list

            pfirst = paux;  //the following node becomes the new first

            if (pfirst != NULL)
                pfirst->prev = NULL;
        }
        else
            cout<<"The list is empty"<<endl;
    }

    void removeLast()
    {
        //removes the last element of the list
        Node <T> *paux;

        if (plast != NULL)
        {
            paux = plast->prev;

            if (pfirst == plast)
                pfirst = NULL;

            delete plast; //delete the last node of the list

            plast = paux; //the previous node becomes the new last
            if (plast != NULL)
                plast->next = NULL;
        }
        else
            cout<<"The list is empty"<<endl;
    }

    Node <T>* getFirst () {
        //return the first node of the list
        Node <T> *paux;
        paux = pfirst;
        return paux; 
    }
/*
    Node <T>* getNext (Node <T>* p) {
        //return the next node of the list
        Node <T> *paux;
        paux = p->next;
        return paux; 
    }
*/    
    int getSize ()
    {
        //return the size of the list
        Node <T> *paux;
        int size = 0;
        paux = pfirst;
        while (paux != NULL)
        {
            size++;
            paux = paux->next;
        }
        return size;
    }

    Node <T>* findFirstOccurrence(T x)
    {
        //search for the first node which has the info x
        Node <T> *paux;

        paux = pfirst;

        while (paux != NULL)
        {
            if (paux->info == x)
                return paux;
            paux = paux->next;
        }
        return NULL;
    }

    Node <T>* findLastOccurrence(T x)
    { //search for the last node which has the info x
        Node <T> *paux;

        paux = plast;
        while (paux != NULL) {
            if (paux->info == x)
                return paux;
            paux = paux->prev;
        }

        return NULL;
    }

    void removeFirstOccurrence(T x)
    {
        //remove the first node which has the info x
        Node <T> *px;

        px = findFirstOccurrence(x); //px must be removed

        if (px != NULL)
        {//we must re-link the pointers
            if (px->prev != NULL)
                px->prev->next = px->next;

            if (px->next != NULL)
                px->next->prev = px->prev;

            if (px->prev == NULL) // if px == pfirst
                pfirst = px->next;

            if (px->next == NULL) // if px == plast
                plast = px->prev;

            delete px; //now we can delete it
        }
    }

    void removeLastOccurrence(T x)
    {
        //remove the last node which has the info x
        Node <T> *px;

        px = findLastOccurrence(x);

        if (px != NULL)
        {
            if (px->prev != NULL)
                px->prev->next = px->next;

            if (px->next != NULL)
                px->next->prev = px->prev;

            if (px->prev == NULL) // if px == pfirst
                pfirst = px->next;

            if (px->next == NULL) // if px == plast
                plast = px->prev;

            delete px;
        }
    }

    int isEmpty()
    { //check if the list is empty
        return (pfirst == NULL);
    }

    LinkedList()
    { //constructor
        pfirst = plast = NULL;
    }

};

