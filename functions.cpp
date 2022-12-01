#include "functions.h"


int ListCtor(List* list_p, int capacity)
{
    CHECKUS(list_p != NULL, 1);

    if (capacity < 1)
    {
        printf(KRED Kbright "Cannot make list with capacity %i\n" KNRM, capacity);
        exit(1);
    }
    else if (capacity == 1)
    {
        capacity = 2;
    }

    list_p->data = (ListElement*) calloc(capacity, sizeof(*list_p->data));
    list_p->capacity = capacity;
    list_p->first_free = 1;
    list_p->last_free = capacity - 1;
    list_p->size = 0;
    list_p->anchor_elem = 0;

    list_p->data[0].prev = 0;
    list_p->data[0].next = 0;
    list_p->data[0].var  = 0;
    for (int i = 1; i < capacity; i++)
    {
        list_p->data[i].var = 0;
        list_p->data[i].prev = EMPTY_INDEX;
        list_p->data[i].next = i + 1;
    }
    list_p->data[capacity - 1].next = 0;

    return 0;
}

int FindAddress(List* list_p, int position)
{
    if (list_p->size < 0)
    {
        printf(KRED Kbright "FindAddress: Error, list is empty!\n" KNRM);
        exit(1);
    }
    if (position < 0 || position > list_p->size)
    {
        printf(KRED Kbright "FindAddress: Error, incorrect position (%i)!\n" KNRM, position);
        exit(1);
    }

    int next_adress = list_p->anchor_elem;

    for (int i = 0; i < position; i++)
    {
        next_adress = list_p->data[next_adress].next;
    }
    return next_adress;
}

int InsertByAddress(List* list_p, int address, Elem_t var)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size >= 0, 1);

    if (address < 0 || address > list_p->capacity || list_p->data[address].prev == EMPTY_INDEX)
    {
        printf(KRED Kbright "InsertByAddress: Error, incorrect address!\n" KNRM);
        return 1;
    }

    int free_adress = list_p->first_free;
    if (list_p->data[free_adress].next == 0)
    {
        IncreaseCapacity(list_p);
    }
    list_p->first_free = list_p->data[free_adress].next;

    int next_adress = address;
    int prev_adress = list_p->data[next_adress].prev;

    list_p->data[free_adress].var = var;
    list_p->data[free_adress].prev = prev_adress;
    list_p->data[free_adress].next = next_adress;

    list_p->data[prev_adress].next = free_adress;
    list_p->data[next_adress].prev = free_adress;

    if (next_adress == list_p->anchor_elem)
    {
        list_p->anchor_elem = free_adress;
    }

    list_p->size += 1;
    list_p->is_sorted = 0;

    return 0;
}

int InsertByPosition(List* list_p, int position, Elem_t var)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size >= 0, 1);

    if (position > list_p->size || position < 0) // error
    {
        printf(KRED Kbright "Cannot insert in position %i, there are only %i elements in list\n" KNRM, position, list_p->size);
        return 1;
    }

    if (list_p->is_sorted) // position = address
    {
        InsertByAddress(list_p, position, var);
    }
    else
    {
        InsertByAddress(list_p, FindAddress(list_p, position), var);
    }

    return 0;
}

int Dump(List* list_p)
{
    CHECKUS(list_p != NULL, 1);

    Elem_t x = 0;
    for (int i = 0; i < list_p->size; i++)
    {
        CopyByPosition(list_p, i, &x);
        printf(ELEM_FORMAT"\n", x);
    }

    printf("\nSize: %i\nCapacity: %i\nFirst free element adress: %i\nLast free element adress: %i\nAnchor element adress: %i\n\n",
    list_p->size, list_p->capacity, list_p->first_free, list_p->last_free, list_p->anchor_elem);

    for (int i = 0; i < list_p->capacity; i++)
    {
        if (i == list_p->first_free)
        {
            printf(KRED "Var%i: " ELEM_FORMAT_INDENT(7) " Next: %-5i Prev: %-5i <-- first free element\n" KNRM,
            i, list_p->data[i].var, list_p->data[i].next, list_p->data[i].prev);
        }
        else if (i == list_p->last_free)
        {
            printf(KBLU "Var%i: " ELEM_FORMAT_INDENT(7) " Next: %-5i Prev: %-5i <-- last free element\n" KNRM,
            i, list_p->data[i].var, list_p->data[i].next, list_p->data[i].prev);
        }
        else if (i == list_p->anchor_elem)
        {
            printf(KGRN "Var%i: " ELEM_FORMAT_INDENT(7) " Next: %-5i Prev: %-5i <-- anchor element\n" KNRM,
            i, list_p->data[i].var, list_p->data[i].next, list_p->data[i].prev);
        }
        else if (list_p->data[i].prev == EMPTY_INDEX)
        {
            printf(KBLK "Var%i: " ELEM_FORMAT_INDENT(7) " Next: %-5i Prev: %-5i\n" KNRM,
            i, list_p->data[i].var, list_p->data[i].next, list_p->data[i].prev);
        }
        else
        {
            printf("Var%i: " ELEM_FORMAT_INDENT(7) " Next: %-5i Prev: %-5i\n",
            i, list_p->data[i].var, list_p->data[i].next, list_p->data[i].prev);
        }
    }
    return 0;
}

int EraseByAddress(List* list_p, int address)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size > 0, 1);

    if (address < 0 || address > list_p->capacity || list_p->data[address].prev == EMPTY_INDEX)
    {
        printf(KRED Kbright "InsertByAddress: Error, incorrect address!\n" KNRM);
        return 1;
    }

    int prev_address = list_p->data[address].prev;
    int next_address = list_p->data[address].next;

    list_p->data[address].var = 0;
    list_p->data[address].prev = EMPTY_INDEX;
    list_p->data[address].next = list_p->first_free;
    list_p->first_free = address;

    list_p->data[prev_address].next = next_address;
    list_p->data[next_address].prev = prev_address;

    if (address == list_p->anchor_elem)
    {
        list_p->anchor_elem = next_address;
    }

    list_p->size -= 1;
    list_p->is_sorted = 0;
    return 0;
}

int EraseByPosition(List* list_p, int position)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size > 0, 1);

    if (position >= list_p->size || position < 0) // error
    {
        printf(KRED Kbright "Cannot erase in position %i, allowed positions 0 - %i\n" KNRM, position, list_p->size - 1);
        return 1;
    }

    if (list_p->is_sorted) // position = address
    {
        EraseByAddress(list_p, position);
    }
    else
    {
        EraseByAddress(list_p, FindAddress(list_p, position));
    }

    return 0;
}

int CopyByAddress(List* list_p, int address, Elem_t* var_p)
{
    CHECKUS(list_p != NULL, 1);

    if (list_p->data[address].prev == EMPTY_INDEX)
    {
        printf(KRED Kbright "Cannot copy from address %i, its empty!" KNRM, address);
    }

    *var_p = list_p->data[address].var;

    return 0;
}

int CopyByPosition(List* list_p, int position, Elem_t* var_p)
{
    CHECKUS(list_p != NULL, 1);

    CopyByAddress(list_p, FindAddress(list_p, position), var_p);

    return 0;
}

int IncreaseCapacity(List* list_p)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(CAPACITY_COEF > 1, 1);

    int new_capacity = list_p->capacity * CAPACITY_COEF;
    list_p->data = (ListElement*) realloc(list_p->data, new_capacity * sizeof(*list_p->data));

    for (int i = list_p->capacity; i < new_capacity; i++)
    {
        list_p->data[i].var = 0;
        list_p->data[i].prev = EMPTY_INDEX;
        list_p->data[i].next = i + 1;
    }
    list_p->data[new_capacity - 1].next = 0;

    list_p->data[list_p->last_free].next = list_p->capacity;

    if (list_p->first_free == 0 && list_p->data[0].prev != EMPTY_INDEX)
    {
        list_p->first_free = list_p->capacity;
    }

    list_p->last_free = new_capacity - 1;
    list_p->capacity = new_capacity;
    return 0;
}


int SortAndResize(List* list_p, int new_capacity)
{
    CHECKUS(list_p != NULL, 1);

    if (new_capacity <= list_p->size)
    {
        printf(KRED Kbright "Error: Cannot resize because new capacity (%i) is smaller or equal than list size (%i)" KNRM,
                new_capacity, list_p->size);
    }

    ListElement* new_data = (ListElement*) calloc(new_capacity, sizeof(*new_data));

    int next_adress = list_p->anchor_elem;
    int counter = 0;
    do {
        new_data[counter].var = list_p->data[next_adress].var;
        new_data[counter].prev = counter - 1;
        new_data[counter].next = counter + 1;
        next_adress = list_p->data[next_adress].next;
        counter++;
    } while (next_adress != list_p->anchor_elem);

    new_data[0].prev = counter;
    new_data[counter - 1].next = 0;

    for (int i = counter; i < new_capacity; i++)
    {
        new_data[i].var = 0;
        new_data[i].prev = EMPTY_INDEX;
        new_data[i].next = i + 1;
    }
    new_data[new_capacity - 1].next = 0;

    list_p->first_free = counter;
    list_p->last_free = new_capacity - 1;
    list_p->capacity = new_capacity;

    free(list_p->data);
    list_p->data = new_data;
    list_p->is_sorted = 1;

    return 0;
}


int ListDtor(List* list_p)
{
    CHECKUS(list_p != NULL, 1);

    free(list_p->data);
    list_p->capacity = 0;
    list_p->first_free = 0;
    list_p->size = 0;
    return 0;
}

void MakeGvList(List* list_p)
{
    FILE* gv_file = fopen("GvList.dot", "w");
    CHECKUS(gv_file != NULL, (void) 0);

    fprintf(gv_file, "digraph {\n    fontname=\"Comic Sans MS\"\n    bgcolor=\"#" BACKGROUND_COLOR "\"\n    pad=0.4\n    node [style=rounded]\n    subgraph {\n        rank=same");

    for (int counter = 0; counter < list_p->capacity; counter++)
    {
        fprintf(gv_file,
                R"(
                    node_%d
                    [
                        style="filled",
                        fillcolor="#)", counter);

        if (counter == list_p->first_free)
        {
            fprintf(gv_file, FIRST_FREE_COLOR);
        }
        else if (counter == list_p->last_free)
        {
            fprintf(gv_file, LAST_FREE_COLOR);
        }
        else if (counter == list_p->anchor_elem)
        {
            fprintf(gv_file, ANCHOR_COLOR);
        }
        else if (list_p->data[counter].prev == EMPTY_INDEX)
        {
            fprintf(gv_file, EMPTY_COLOR);
        }
        else
        {
            fprintf(gv_file, DATA_COLOR);
        }


        fprintf(gv_file,
                R"("
                        shape=plaintext,
                        label=
                        <
                            <table border="0" cellborder="1" cellspacing="0">
                                <tr>
                                    <td colspan="2">Node %d</td>
                                </tr>
                                <tr>)", counter);

        fprintf (gv_file, R"(
                <td>   data:  </td>
                <td>   %i    </td>
            )", list_p->data[counter].var);

        fprintf(gv_file, R"(
                            </tr>
                            <tr>
                                <td  port="prev_out" > prev: </td>
                                <td  port="prev_in" > %d    </td>
                            </tr>
                            <tr>
                                <td port="next_in" > next: </td>
                                <td port="next_out"> %d    </td>
                            </tr>
                        </table>
                    >
                ];
            )", list_p->data[counter].prev, list_p->data[counter].next);

    }

    for (int counter = 0; counter < list_p->capacity; counter++)
    {
        if (list_p->data[counter].next > 0)
        {
            fprintf(gv_file, "\nnode_%d:<next_out> -> node_%d:<next_in> [color=grey90, style=bold];\n", counter, list_p->data[counter].next);
        }

        if ((list_p->data[counter].prev >= 0) && (counter != 0))
        {
            fprintf(gv_file, "\nnode_%d:<prev_out> -> node_%d:<prev_in> [color=grey90, style=bold];\n", counter, list_p->data[counter].prev);
        }

        if ((list_p->data[counter].prev == EMPTY_INDEX) && (list_p->data[counter].next == 0))
        {
            fprintf(gv_file, "\nnode_%d:<next_out> -> node_%d:<next_in> [color=grey90, style=bold];\n", counter, list_p->data[counter].next);
        }
    }


    fprintf (gv_file, "}\n");

    fprintf (gv_file, "<first free element> [shape=rectangle, fontcolor = \"#" FIRST_FREE_COLOR "\", color = \"#" FIRST_FREE_COLOR
    "\"]; <first free element> -> node_%d [color=grey90, style=bold];\n", list_p->first_free);                                // first free element
    fprintf (gv_file, "<last free element> [shape=rectangle, fontcolor = \"#" LAST_FREE_COLOR "\", color = \"#" LAST_FREE_COLOR
    "\"]; <last free element> -> node_%d [color=grey90, style=bold];\n", list_p->last_free);                                  // last free element
    fprintf (gv_file, "<anchor element> [shape=rectangle, fontcolor = \"#" ANCHOR_COLOR "\", color = \"#" ANCHOR_COLOR
    "\"]; <anchor element> -> node_%d [color=grey90, style=bold];\n", list_p->anchor_elem);                                   // anchor element

    fprintf (gv_file, "}\n");

    fclose(gv_file);
    return;
}

