#ifndef RAT_STYLE_H
#define RAT_STYLE_H

#define style(code) "\e[" code "m"

extern char *RESET;
extern char *BOLD;
extern char *ITALIC;
extern char *UNDERLINE;
extern char *BLACK;
extern char *RED;
extern char *GREEN;
extern char *CYAN;
extern char *GREY;
extern char *GREY_BG;
extern char *YELLOW_BG;

#define NONE ""

void enable_colors();
void disable_colors();
char *join_strings(int count, ...);
char *colored(char *text, char *color);

#endif
