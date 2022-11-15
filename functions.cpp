#include "functions.h"


int ListCtor_(List* list_p, int capacity)
{
    CHECKUS(list_p != NULL, 1);

    if (capacity < 1)
    {
        printf(KRED Kbright "Cannot make list with capacity %i\n" KNRM, capacity);
        return 1;
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

    list_p->data[0].prev = EMPTY_INDEX;
    for (int i = 1; i < capacity; i++)
    {
        list_p->data[i].var = 0;
        list_p->data[i].prev = EMPTY_INDEX;
        list_p->data[i].next = i + 1;
    }
    list_p->data[capacity - 1].next = 0;

    return 0;
}

int FindAdress(List* list_p, int position)
{
    if (position < 0)
    {
        //printf("Only positive integers are allowed for position number\n");
        printf("----------- FindAdress ERROR ------------\n");
        return 1;
    }
    if (list_p->size <= 0)
    {
        printf(KRED Kbright "FindAdress: error, list is empty!!!\n" KNRM);
        return 1;
    }

    int normal_position =  position % list_p->size;
    int next_adress = list_p->anchor_elem;

    for (int i = 0; i < normal_position; i++)
    {
        next_adress = list_p->data[next_adress].next;
    }
    return next_adress;
}

int Insert(List* list_p, int position, Elem_t var)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size >= 0, 1);

    if (position > list_p->size || position < 0) // error
    {
        printf(KRED Kbright "Cannot insert in position %i, there are only %i elements in list\n" KNRM, position, list_p->size);
        return 1;
    }
    else if (list_p->size == 0) // set anchor_elem to var
    {
        list_p->data[list_p->anchor_elem].var = var;
        list_p->data[list_p->anchor_elem].next = list_p->anchor_elem;
        list_p->data[list_p->anchor_elem].prev = list_p->anchor_elem;
        list_p->size = 1;
    }
    else
    {
        int free_adress = list_p->first_free;
        if (list_p->data[free_adress].next == 0)
        {
            IncreaseCapacity(list_p);
        }
        list_p->first_free = list_p->data[free_adress].next;

        int next_adress = FindAdress(list_p, position);
        int prev_adress = list_p->data[next_adress].prev;

        list_p->data[free_adress].var = var;
        list_p->data[free_adress].prev = prev_adress;
        list_p->data[free_adress].next = next_adress;

        list_p->data[prev_adress].next = free_adress;
        list_p->data[next_adress].prev = free_adress;

        if (position == 0)
        {
            list_p->anchor_elem = free_adress;
        }

        list_p->size += 1;
    }
    return 0;
}
/*
int PushFront(List* list_p, Elem_t var)
{
    CHECKUS(list_p != NULL, 1);
    return Insert(list_p, 0, var);
}
 */
int Dump(List* list_p)
{
    CHECKUS(list_p != NULL, 1);

    Elem_t x = 0;
    for (int i = 0; i < list_p->size; i++)
    {
        Copy(list_p, i, &x);
        printf(ELEM_FORMAT"\n", x);
    }

    printf("\nSize: %i\nCapacity: %i\nFirst free element adress: %i\nLast free element adress: %i\nAnchor element adress: %i\n\n",
    list_p->size, list_p->capacity, list_p->first_free, list_p->last_free, list_p->anchor_elem);

    //int cells_amount = min(list_p->capacity, list_p->size + 3);
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

int Erase(List* list_p, int position)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(list_p->size > 0, 1);

    if (position >= list_p->size || position < 0) // error
    {
        printf(KRED Kbright "Cannot erase in position %i, allowed positions 0 - %i\n" KNRM, position, list_p->size - 1);
        return 1;
    }
    else if (list_p->size == 1) // set anchor_elem to 0
    {
        list_p->data[list_p->anchor_elem].var = 0;
        list_p->data[list_p->anchor_elem].prev = EMPTY_INDEX;
        list_p->size -= 1;
    }
    else
    {
        int adress = FindAdress(list_p, position);
        int prev_adress = list_p->data[adress].prev;
        int next_adress = list_p->data[adress].next;

        if (position == 0)
        {
            list_p->anchor_elem = list_p->data[adress].next;
        }

        list_p->data[adress].var = 0;
        list_p->data[adress].prev = EMPTY_INDEX;
        list_p->data[adress].next = list_p->first_free;
        list_p->first_free = adress;

        list_p->data[prev_adress].next = next_adress;
        list_p->data[next_adress].prev = prev_adress;

        list_p->size -= 1;
    }
    return 0;
}

int Copy(List* list_p, int position, Elem_t* var_p)
{
    CHECKUS(list_p != NULL, 1);

    int adress = FindAdress(list_p, position);
    *var_p = list_p->data[adress].var;

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

/*
int IncreaseCapacity(List* list_p)
{
    CHECKUS(list_p != NULL, 1);
    CHECKUS(CAPACITY_COEF > 1, 1);

    int new_capacity = list_p->capacity * CAPACITY_COEF;
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
    new_data[counter].next = 0;

    for (int i = counter; i < new_capacity; i++)
    {
        new_data[counter].var = 0;
        new_data[counter].prev = EMPTY_INDEX;
        new_data[counter].next = i + 1;
    }
    new_data[new_capacity - 1].next = 0;

    list_p->first_free = counter;
    list_p->last_free = new_capacity - 1;
    list_p->capacity = new_capacity;

    free(list_p->data);
    list_p->data = new_data;

    return 0;
} */

int DecreaseCapacity(List* list_p)
{
    CHECKUS(list_p != NULL, 1);

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

    fprintf(gv_file, "digraph { subgraph { rank=same \n");

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
            fprintf(gv_file, "\nnode_%d:<next_out> -> node_%d:<next_in>;\n", counter, list_p->data[counter].next);
        }

        if ((list_p->data[counter].prev >= 0) && (counter != 0))
        {
            fprintf(gv_file, "\nnode_%d:<prev_out> -> node_%d:<prev_in>;\n", counter, list_p->data[counter].prev);
        }

        if ((list_p->data[counter].prev == EMPTY_INDEX) && (list_p->data[counter].next == 0))
        {
            fprintf(gv_file, "\nnode_%d:<next_out> -> node_%d:<next_in>;\n", counter, list_p->data[counter].next);
        }
    }

    fprintf(gv_file, "}\n}\n");

    fclose(gv_file);
    return;
}

void MakeGvList(void)
{
    system("dot -Tpng GvList.dot -o GvList.png");
}