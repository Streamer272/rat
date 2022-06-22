#include "string_manipulation.h"
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>
#include "def/alloc.h"
#include "def/style.h"

size_t **str_matches(char *heystack, char *needle, char **heystack_copy, int *matches_count) {
    if (heystack == NULL) return NULL;
    if (needle == NULL) return NULL;
    if (strcmp(needle, "") == 0) return NULL;

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

char *highlight_needle(char *heystack, char *needle, bool is_highlighted) {
    if (heystack == NULL) return NULL;
    if (needle == NULL) return NULL;

    char *heystack_copy = NULL;
    int matches_count = 0;
    size_t **matches = str_matches(heystack, needle, &heystack_copy, &matches_count);
    char *color = join_strings(2, BLACK, YELLOW_BG);
    char *result = alloc(strlen(heystack) + matches_count * (strlen(color) + strlen(GREY_BG) + strlen(RESET)) + 1);

    size_t *last_match_index = NULL;
    for (int i = 0; i < matches_count; i++) {
        size_t *match_index = matches[i];
        char *match = heystack + *match_index;
        if (last_match_index != NULL)
            strncat(result, heystack + *last_match_index + strlen(needle), *match_index - *last_match_index - strlen(needle));
        else
            strncat(result, heystack, match - heystack);
        last_match_index = match_index;

        char *temp = alloc(strlen(needle) + strlen(color) + strlen(GREY_BG) + strlen(RESET) + 1);
        strcat(temp, color);
        strncat(temp, match, strlen(needle));
        strcat(temp, RESET);
        if (is_highlighted) strcat(temp, GREY_BG);

        strcat(result, temp);
        free(temp);
    }

    unsigned long rest_start = 0;
    if (last_match_index != NULL) rest_start = *last_match_index + strlen(needle);
    strcat(result, heystack + rest_start);

    free(color);
    for (int i = 0; matches[i] != NULL; i++) free(matches[i]);
    free(matches);
    free(heystack_copy);

    return result;
}

char *format_bytes(size_t bytes) {
    unsigned short unit = 0;
    long double value = bytes;

    while (value > 1024) {
        value /= 1024;
        unit++;
    }

    char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    char *result = alloc(sizeof(char) * (4 + strlen(units[unit]) + 1));
    sprintf(result, "%.0Lf%s", value, units[unit]);

    char *colored_result = colored(result, GREEN);
    free(result);
    return colored_result;
}

char *format_perms(mode_t perms) {
    char *usr_read = colored((perms & S_IRUSR) ? "r" : "-", YELLOW);
    char *usr_write = colored((perms & S_IWUSR) ? "w" : "-", RED);
    char *usr_exec = colored((perms & S_IXUSR) ? "x" : "-", GREEN);

    char *group_read = colored((perms & S_IRGRP) ? "r" : "-", YELLOW);
    char *group_write = colored((perms & S_IWGRP) ? "w" : "-", RED);
    char *group_exec = colored((perms & S_IXGRP) ? "x" : "-", GREEN);

    char *other_read = colored((perms & S_IROTH) ? "r" : "-", YELLOW);
    char *other_write = colored((perms & S_IWOTH) ? "w" : "-", RED);
    char *other_exec = colored((perms & S_IXOTH) ? "x" : "-", GREEN);

    char *result = join_strings(11, BOLD, usr_read, usr_write, usr_exec, RESET, group_read, group_write, group_exec, other_read, other_write, other_exec);

    free(usr_read);
    free(usr_write);
    free(usr_exec);

    free(group_read);
    free(group_write);
    free(group_exec);

    free(other_read);
    free(other_write);
    free(other_exec);

    return result;
}

char *format_time(time_t time) {
    char *result = alloc(256);
    struct tm *time_struct = localtime(&time);
    strftime(result, 256, "%dth %B %Y %H:%M:%S", time_struct);
    char *colored_result = colored(result, BLUE);
    free(result);
    return colored_result;
}
