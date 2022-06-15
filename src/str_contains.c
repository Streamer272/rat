#include "str_contains.h"
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "alloc.h"
#include "def/style.h"

size_t **str_matches(char *heystack, char **heystack_copy, char *needle) {
    if (heystack == NULL) return NULL;
    if (needle == NULL) return NULL;

    *heystack_copy = alloc(sizeof(heystack));
    strcpy(*heystack_copy, heystack);
    char *needle_copy = alloc(sizeof(needle));
    strcpy(needle_copy, needle);
    size_t **result = alloc(sizeof(char *) * (strlen(heystack) / strlen(needle)));

    for (int i = 0; i < strlen(*heystack_copy); i++) (*heystack_copy)[i] = (char) toupper((*heystack_copy)[i]);
    for (int i = 0; i < strlen(needle_copy); i++) needle_copy[i] = (char) toupper(needle_copy[i]);

    int index = 0;
    size_t char_index;
    char *temp = strstr(*heystack_copy, needle_copy);
    while (temp != NULL) {
        size_t *memory = alloc(sizeof(size_t));
        char_index = temp - *heystack_copy;
        *memory = char_index;
        result[index] = memory;

        index++;
        temp = strstr(temp + strlen(needle_copy), needle_copy);
    }

    free(needle_copy);

    return result;
}

bool str_contains(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    size_t **matches = str_matches(heystack, &heystack_copy, needle);
    if (matches == NULL) return false;

    int i;
    for (i = 0; matches[i] != NULL; i++) free(matches[i]);
    free(matches);
    free(heystack_copy);

    return i > 0;
}

char *highlight_needle(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    size_t **matches = str_matches(heystack, &heystack_copy, needle);
    int matches_count;
    for (matches_count = 0; matches[matches_count] != NULL; matches_count++);
    char *result = alloc(strlen(heystack) + matches_count * strlen(BLACK YELLOW_BG RESET) + 1);

    size_t *last_match_index = NULL;
    for (int i = 0; matches[i] != NULL; i++) {
        size_t *match_index = matches[i];
        char *match = heystack + *match_index;
        if (last_match_index != NULL)
            strncat(result, heystack + *last_match_index + strlen(needle), *match_index - *last_match_index - strlen(needle));
        else
            strncat(result, heystack, match - heystack);
        last_match_index = match_index;

        char *temp = alloc(strlen(needle) + strlen(BLACK YELLOW_BG RESET) + 1);
        strcat(temp, BLACK YELLOW_BG);
        strncat(temp, match, strlen(needle));
        strcat(temp, RESET);

        strcat(result, temp);
        free(temp);
    }

    strcat(result, heystack + *last_match_index + strlen(needle));

    for (int i = 0; matches[i] != NULL; i++) free(matches[i]);
    free(matches);
    free(heystack_copy);

    return result;
}