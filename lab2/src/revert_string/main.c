#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "revert_string.c"

int main(int argc, char *argv[])
{
	if (argc != 2) // check if there is only one argument
	{
		printf("Usage: %s string_to_revert\n", argv[0]); // print name
		return -1;
	}

	char *reverted_str = malloc(sizeof(char) * (strlen(argv[1]) + 1));
	strcpy(reverted_str, argv[1]);

	RevertString(reverted_str);

	printf("Reverted: %s\n", reverted_str);
	free(reverted_str);
	return 0;
}

