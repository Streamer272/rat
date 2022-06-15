#include "str_contains.h"
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "def/alloc.h"
#include "def/style.h"

size_t **str_matches(char *heystack, char *needle, char **heystack_copy, int *matches_count) {
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

    if (matches_count != NULL) *matches_count = index;
    return result;
}

bool str_contains(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    int matches_count = 0;
    size_t **matches = str_matches(heystack, needle, &heystack_copy, &matches_count);
    if (matches == NULL) return false;

    int i;
    for (i = 0; i < matches_count; i++) free(matches[i]);
    free(matches);
    free(heystack_copy);

    return i > 0;
}

// TODO: fix filter fucks up line highlighting
char *highlight_needle(char *heystack, char *needle) {
    char *heystack_copy = NULL;
    int matches_count = 0;
    size_t **matches = str_matches(heystack, needle, &heystack_copy, &matches_count);
    char *color = join_strings(2, BLACK, YELLOW_BG);
    char *result = alloc(strlen(heystack) + matches_count * (strlen(color) + strlen(RESET)) + 1);

    size_t *last_match_index = NULL;
    for (int i = 0; i < matches_count; i++) {
        size_t *match_index = matches[i];
        char *match = heystack + *match_index;
        if (last_match_index != NULL)
            strncat(result, heystack + *last_match_index + strlen(needle), *match_index - *last_match_index - strlen(needle));
        else
            strncat(result, heystack, match - heystack);
        last_match_index = match_index;

        char *temp = alloc(strlen(needle) + strlen(color) + strlen(RESET) + 1);
        strcat(temp, color);
        strncat(temp, match, strlen(needle));
        strcat(temp, RESET);

        strcat(result, temp);
        free(temp);
    }

    strcat(result, heystack + *last_match_index + strlen(needle));

    free(color);
    for (int i = 0; matches[i] != NULL; i++) free(matches[i]);
    free(matches);
    free(heystack_copy);

    return result;
}
