#ifndef RAT_STYLE_H
#define RAT_STYLE_H

#define style(code) "\e[" code "m"

#define RESET style("0")
#define BOLD style("1")
#define UNDERLINE style("4")
#define GREEN style("32")
#define CYAN style("36")
#define GREY style("38;2;85;87;83")

#endif
