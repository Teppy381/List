#include "functions.h"
#include "assert.h"

int main ()
{
    printf("\n");

    List my_list = {};
    ListCtor(&my_list, 1);

    Insert(&my_list, 0, 320);
    Insert(&my_list, 0, 7400);
    Insert(&my_list, 0, 5400);
    Insert(&my_list, 0, 60);

    //Erase(&my_list, 0);
    //Erase(&my_list, 2);

    //IncreaseCapacity(&my_list);

    MakeGvList(&my_list);

    Dump(&my_list);

    ListDtor(&my_list);
    return 0;
}
