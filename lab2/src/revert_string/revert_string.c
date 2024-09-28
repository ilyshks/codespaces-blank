#include <stdlib.h>
#include <string.h>
#include "revert_string.h"

void RevertString(char *str)
{
	char *result_str = malloc(sizeof(char) * (strlen(str) + 1));
	for (int i = 0; i < strlen(str); i++){
		result_str[i] = str[strlen(str) - i - 1];
	}
	result_str[strlen(str)] = '\0';
	strcpy(str, result_str);
	free(result_str);
}

