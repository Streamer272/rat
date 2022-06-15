#ifndef RAT_STR_CONTAINS_H
#define RAT_STR_CONTAINS_H

#include <stdbool.h>

char **str_matches(char *heystack, char **heystack_copy, char *needle);
bool str_contains(char *heystack, char *needle);
char *highlight_needle(char *heystack, char *needle);

#endif
