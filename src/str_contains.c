#include "str_contains.h"
#include <string.h>
#include <ctype.h>
#include "alloc.h"

bool str_contains(char *heystack, char *needle) {
    if (heystack == NULL) return NULL;
    if (needle == NULL) return NULL;

    char *heystack_copy = alloc(sizeof(heystack));
    strcpy(heystack_copy, heystack);
    char *needle_copy = alloc(sizeof(needle));
    strcpy(needle_copy, needle);

    for (int i = 0; i < strlen(heystack_copy); i++) {
        heystack_copy[i] = (char) toupper(heystack_copy[i]);
    }
    for (int i = 0; i < strlen(needle_copy); i++) {
        needle_copy[i] = (char) toupper(needle_copy[i]);
    }

    return strstr(heystack_copy, needle_copy) != NULL;
}
