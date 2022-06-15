#include "str_contains.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "alloc.h"
#include "def/style.h"

char **str_matches(char *heystack, char **heystack_copy, char *needle) {
    if (heystack == NULL) return NULL;
    if (needle == NULL) return NULL;

    *heystack_copy = alloc(sizeof(heystack));
    strcpy(*heystack_copy, heystack);
    char *needle_copy = alloc(sizeof(needle));
    strcpy(needle_copy, needle);
    char **result = alloc(sizeof(char *) * (strlen(heystack) / strlen(needle)));

    for (int i = 0; i < strlen(*heystack_copy); i++) (*heystack_copy)[i] = (char) toupper((*heystack_copy)[i]);
    for (int i = 0; i < strlen(needle_copy); i++) needle_copy[i] = (char) toupper(needle_copy[i]);

    int index = 0;
    size_t char_index = 0;
    char *temp = strstr(*heystack_copy, needle_copy);
    while (temp != NULL) {
        char *copy = alloc(strlen(temp) + 1);
        strcpy(copy, temp);
        result[index] = copy;
        index++;
        char_index = temp - *heystack_copy;
        printf("found (%p - %p) %s at %zu\n", temp, *heystack_copy, temp, char_index);
        temp = strstr(temp + strlen(needle_copy), needle_copy);
    }

    free(needle_copy);

    return result;
}

bool str_contains(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    char **matches = str_matches(heystack, &heystack_copy, needle);
    if (matches == NULL) return false;

    int i;
    for (i = 0; matches[i] != NULL; i++);
    free(matches);
    free(heystack_copy);

    return i > 0;
}

char *highlight_needle(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    char **matches = str_matches(heystack, &heystack_copy, needle);
    int matches_count;
    for (matches_count = 0; matches[matches_count] != NULL; matches_count++);
    char *result = alloc(strlen(heystack) + matches_count * strlen(BLACK YELLOW_BG RESET) + 1);

    char *last_match = NULL;
    for (int i = 0; matches[i] != NULL; i++) {
        char *match = matches[i];
        int match_index = match - heystack_copy;
        printf("%p (%s) - %p (%s) = %d\n", match, match, heystack_copy, heystack_copy, match_index);
        if (last_match != NULL) {
            strncat(result, last_match + strlen(needle), match - last_match);
        }
        last_match = match;

        char *temp = alloc(strlen(match) + strlen(BLACK YELLOW_BG RESET) + 1);
        strcat(temp, BLACK YELLOW_BG);
        strncat(temp, match, strlen(needle));
        strcat(temp, RESET);
        strcat(temp, match + strlen(needle));

        strcat(result, temp);
        free(temp);
    }

    free(matches);
    free(heystack_copy);

    return result;
}
