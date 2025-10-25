#include "patterns.h"
#include <stdio.h>

static void print_spaces(FILE *out, int n) {
    for (int i = 0; i < n; i++) fputc(' ', out);
}

static void print_chars(FILE *out, char ch, int n) {
    for (int i = 0; i < n; i++) fputc(ch, out);
}

static void print_numbers(FILE *out, int start, int n) {
    for (int i = 0; i < n; i++) fputc('1' + (start + i - 1) % 9, out);
}

static void print_alpha(FILE *out, char start, int n) {
    for (int i = 0; i < n; i++) fputc(start + i, out);
}

// Pyramid
static void print_pyramid(const PatternOptions *opt) {
    for (int i = 1; i <= opt->rows; i++) {
        print_spaces(opt->out, opt->rows - i);
        if (opt->mode == PRINT_NUMBER) print_numbers(opt->out, 1, 2*i - 1);
        else if (opt->mode == PRINT_ALPHA) print_alpha(opt->out, 'a', 2*i - 1);
        else print_chars(opt->out, opt->fill, 2*i - 1);
        fputc('\n', opt->out);
    }
}

// Inverted Pyramid
static void print_inverted(const PatternOptions *opt) {
    for (int i = opt->rows; i >= 1; i--) {
        print_spaces(opt->out, opt->rows - i);
        if (opt->mode == PRINT_NUMBER) print_numbers(opt->out, 1, 2*i - 1);
        else if (opt->mode == PRINT_ALPHA) print_alpha(opt->out, 'a', 2*i - 1);
        else print_chars(opt->out, opt->fill, 2*i - 1);
        fputc('\n', opt->out);
    }
}

// Number Pyramid
static void print_number_pyramid(const PatternOptions *opt) {
    for (int i = 1; i <= opt->rows; i++) {
        print_spaces(opt->out, opt->rows - i);
        for (int j = 1; j <= i; j++) {
            if (opt->mode == PRINT_NUMBER) fprintf(opt->out, "%d", j);
            else if (opt->mode == PRINT_ALPHA) fprintf(opt->out, "%c", 'a' + j - 1);
            else fputc(opt->fill, opt->out);
        }
        for (int j = i - 1; j >= 1; j--) {
            if (opt->mode == PRINT_NUMBER) fprintf(opt->out, "%d", j);
            else if (opt->mode == PRINT_ALPHA) fprintf(opt->out, "%c", 'a' + j - 1);
            else fputc(opt->fill, opt->out);
        }
        fputc('\n', opt->out);
    }
}

// Pascal Triangle
static void print_pascal(const PatternOptions *opt) {
    for (int i = 0; i < opt->rows; i++) {
        print_spaces(opt->out, opt->rows - i - 1);
        long long val = 1;
        for (int j = 0; j <= i; j++) {
            fprintf(opt->out, "%lld ", val);
            val = val * (i - j) / (j + 1);
        }
        fputc('\n', opt->out);
    }
}

// Diamond
static void print_diamond(const PatternOptions *opt) {
    for (int i = 1; i <= opt->rows; i++) {
        print_spaces(opt->out, opt->rows - i);
        if (opt->mode == PRINT_NUMBER) print_numbers(opt->out, 1, 2*i - 1);
        else if (opt->mode == PRINT_ALPHA) print_alpha(opt->out, 'a', 2*i - 1);
        else print_chars(opt->out, opt->fill, 2*i - 1);
        fputc('\n', opt->out);
    }
    for (int i = opt->rows - 1; i >= 1; i--) {
        print_spaces(opt->out, opt->rows - i);
        if (opt->mode == PRINT_NUMBER) print_numbers(opt->out, 1, 2*i - 1);
        else if (opt->mode == PRINT_ALPHA) print_alpha(opt->out, 'a', 2*i - 1);
        else print_chars(opt->out, opt->fill, 2*i - 1);
        fputc('\n', opt->out);
    }
}

// Hollow Diamond (always char)
static void print_hollow_diamond(const PatternOptions *opt) {
    char ch = opt->fill;
    for (int i = 1; i <= opt->rows; i++) {
        print_spaces(opt->out, opt->rows - i);
        if (i == 1) fputc(ch, opt->out);
        else {
            fputc(ch, opt->out);
            print_spaces(opt->out, 2*i - 3);
            fputc(ch, opt->out);
        }
        fputc('\n', opt->out);
    }
    for (int i = opt->rows - 1; i >= 1; i--) {
        print_spaces(opt->out, opt->rows - i);
        if (i == 1) fputc(ch, opt->out);
        else {
            fputc(ch, opt->out);
            print_spaces(opt->out, 2*i - 3);
            fputc(ch, opt->out);
        }
        fputc('\n', opt->out);
    }
}

// Butterfly
static void print_butterfly(const PatternOptions *opt) {
    for (int i = 1; i <= opt->rows; i++) {
        if (opt->mode == PRINT_NUMBER) {
            print_numbers(opt->out, 1, i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_numbers(opt->out, 1, i);
        } else if (opt->mode == PRINT_ALPHA) {
            print_alpha(opt->out, 'a', i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_alpha(opt->out, 'a', i);
        } else {
            print_chars(opt->out, opt->fill, i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_chars(opt->out, opt->fill, i);
        }
        fputc('\n', opt->out);
    }
    for (int i = opt->rows; i >= 1; i--) {
        if (opt->mode == PRINT_NUMBER) {
            print_numbers(opt->out, 1, i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_numbers(opt->out, 1, i);
        } else if (opt->mode == PRINT_ALPHA) {
            print_alpha(opt->out, 'a', i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_alpha(opt->out, 'a', i);
        } else {
            print_chars(opt->out, opt->fill, i);
            print_spaces(opt->out, 2*(opt->rows - i));
            print_chars(opt->out, opt->fill, i);
        }
        fputc('\n', opt->out);
    }
}

int print_pattern(const PatternOptions *opt) {
    if (!opt || !opt->out || opt->rows <= 0) return -1;
    switch (opt->type) {
        case P_PYRAMID: print_pyramid(opt); break;
        case P_INVERTED: print_inverted(opt); break;
        case P_NUMBER_PYRAMID: print_number_pyramid(opt); break;
        case P_PASCAL: print_pascal(opt); break;
        case P_DIAMOND: print_diamond(opt); break;
        case P_HOLLOW_DIAMOND: print_hollow_diamond(opt); break;
        case P_BUTTERFLY: print_butterfly(opt); break;
        default: return -2;
    }
    return 0;
}
