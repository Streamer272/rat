#ifndef RAT_STYLE_H
#define RAT_STYLE_H

#define style(code) "\e[" code "m"

#define RESET style("0")
#define BOLD style("1")
#define ITALIC style("3")
#define UNDERLINE style("4")
#define RED style("31")
#define GREEN style("32")
#define CYAN style("36")
#define GREY style("38;2;153;153;153")
#define HIGHLIGHT_BG style("48;2;69;69;69")

#define NONE ""

#endif
