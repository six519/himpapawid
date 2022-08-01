#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

void print_error(const char *str, const char *err)
{
	printf(str, err);
	exit(0);
}

int generate_random_number(int min, int max)
{
	return rand()%(max-min) + min;
}