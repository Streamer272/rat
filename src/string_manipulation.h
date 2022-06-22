#ifndef RAT_STRING_MANIPULATION_H
#define RAT_STRING_MANIPULATION_H

#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <sys/stat.h>

size_t **str_matches(char *heystack, char *needle, char **heystack_copy, int *matches_count);
bool str_contains(char *heystack, char *needle);
char *highlight_needle(char *heystack, char *needle, bool is_highlighted);
char *format_bytes(size_t bytes);
char *format_perms(mode_t perms);
char *format_time(time_t time);

#endif
