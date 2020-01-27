#include "ush.h"

static bool isvalid(char *c);
static void backspace(size_t *index, int times);
static void set_code(char *tmp, int *code);

void mx_get_input(char *buf, int *code) {
    char tmp[4] = "";
    size_t index = 0;

    buf[0] = '\0';  
    while (read(STDIN_FILENO, tmp, 5)
           && index < ARG_MAX && isvalid(tmp)) {
        if (tmp[0] == 0x7f && index > 0)
            backspace(&index, 1);
        if (tmp[0] == 0x15)
            backspace(&index, index);
        if (isprint(tmp[0]) && strlen(tmp) == 1) {
            buf[index++] = tmp[0];
            buf[index] = '\0';
            printf("%c", tmp[0]);
        }
        fflush(stdin);
        memset(tmp, '\0', sizeof(tmp));
    }
    set_code(tmp, code);
}

static void set_code(char *tmp, int *code) {
    if (tmp[0] == '\x03')
        *code = 130;
    if (tmp[0] == '\x04')
        *code = -1;
}

static void backspace(size_t *index, int times) {
    for (int i = 0; i < times; i++) {
        printf("\b \b");
        (*index)--;
    }
}

static bool isvalid(char *c) {
    if (mx_match(c, MX_NON_PRINTABLE)) {
        return false;
    }
    return true;
}
