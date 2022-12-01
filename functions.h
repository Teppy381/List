#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "colors.h"

#define CHECKUS(condition, return_num)                                                          \
do {                                                                                            \
    if (!(condition))                                                                           \
    {                                                                                           \
        printf("\n" KWHT Kbright "%s: " KNRM "In function ‘" KWHT Kbright "%s" KNRM "’:\n"      \
        KWHT Kbright "%s:%i: " KNRM "Assertion ‘" KRED Kbright "%s" KNRM "’ failed\n",          \
        __FILE__, __PRETTY_FUNCTION__, __FILE__, __LINE__, #condition);                         \
        return return_num;                                                                      \
    }                                                                                           \
} while (0)

typedef int Elem_t;
#define ELEM_FORMAT "%i"
#define ELEM_FORMAT_INDENT(num) "%-"#num"i"

#define CAPACITY_COEF 2 // > 1
#define EMPTY_INDEX -666

#define BACKGROUND_COLOR "272822"
#define LAST_FREE_COLOR  "466AA8"
#define FIRST_FREE_COLOR "AF5D7B"
#define ANCHOR_COLOR     "86B42B"
#define DATA_COLOR       "ACBFA2"
#define EMPTY_COLOR      "808080"


struct ListElement
{
    Elem_t var;
    int prev = 0;
    int next = 0;
};

struct List
{
    ListElement* data;
    int size = 0;
    int capacity = 0;
    int first_free = 0;
    int last_free = 0;
    int anchor_elem = 0;
    int is_sorted = 0;
};


int ListCtor(List* list_p, int capacity);

int FindAddress(List* list_p, int position);

int InsertByAddress(List* list_p, int address, Elem_t var);

int InsertByPosition(List* list_p, int position, Elem_t var);

int Dump(List* list_p);

int EraseByAddress(List* list_p, int address);

int EraseByPosition(List* list_p, int position);

int CopyByAddress(List* list_p, int address, Elem_t* var_p);

int CopyByPosition(List* list_p, int position, Elem_t* var_p);

int IncreaseCapacity(List* list_p);

int SortAndResize(List* list_p, int new_capacity);

int ListDtor(List* list_p);

void MakeGvList(List* list_p);


#endif
