#include "parser_test.h"
#include "../../parser/parser.h"
#include "../../lan/lan.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void parsertest_file_init()
{
    FILE* fptr;

    fptr = file_init("does_not_exist.txt");
    assert(fptr == NULL);

    fptr = fopen("parser_test.txt", "w");
    assert(fptr != NULL);
    fclose(fptr);

    fptr = file_init("parser_test.txt");
    assert(fptr != NULL);

    file_deinit(fptr);
    remove("parser_test.txt");
}

void parsertest_file_lan_create()
{
    FILE* fptr = fopen("parser_test.txt", "w");
    assert(fptr != NULL);

    fprintf(fptr, "2 1\n");
    fclose(fptr);

    fptr = fopen("parser_test.txt", "r");
    assert(fptr != NULL);

    Reseau* lan = NULL;
    assert(file_lan_create(fptr, &lan) == OK);

    file_deinit(fptr);
    lan_deinit(&lan);
    remove("parser_test.txt");
}

void parsertest_file_parse_invalid_file()
{
    Reseau* lan = lan_init(0, 0);

    assert(file_parse("does_not_exist.txt", lan) == POINTEUR_NULL);

    lan_deinit(&lan);
}

void parsertest_file_equipements_create_empty()
{
    FILE* fptr = fopen("parser_test.txt", "w");
    assert(fptr != NULL);
    fclose(fptr);

    fptr = fopen("parser_test.txt", "r");
    assert(fptr != NULL);

    Reseau* lan = lan_init(0, 0);

    assert(file_equipements_create(fptr, 0, lan) == OK);

    file_deinit(fptr);
    lan_deinit(&lan);
    remove("parser_test.txt");
}

void parsertest_file_connexions_create_empty()
{
    FILE* fptr = fopen("parser_test.txt", "w");
    assert(fptr != NULL);
    fclose(fptr);

    fptr = fopen("parser_test.txt", "r");
    assert(fptr != NULL);

    Reseau* lan = lan_init(0, 0);

    assert(file_connexions_create(fptr, 0, lan) == OK);

    file_deinit(fptr);
    lan_deinit(&lan);
    remove("parser_test.txt");
}
