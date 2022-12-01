#include "functions.h"
#include "assert.h"

int main ()
{
    printf("\n");

    List my_list = {};
    ListCtor(&my_list, 1);

    InsertByPosition(&my_list, 0, 320);
    InsertByPosition(&my_list, 0, 7400);

    InsertByAddress(&my_list, 1, 1387);

    // InsertByPosition(&my_list, 0, 5400);
    // InsertByPosition(&my_list, 0, 60);

    //EraseByPosition(&my_list, 0);
    //EraseByPosition(&my_list, 2);

    //IncreaseCapacity(&my_list);

    MakeGvList(&my_list);

    Dump(&my_list);

    ListDtor(&my_list);
    return 0;
}
