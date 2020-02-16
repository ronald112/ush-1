#include "ush.h"

static void set_code(char *buff, int *code, int index);
static bool handle_key(t_prompt *prompt, int *code);
static void clear_prompt(t_prompt *prompt);
static void clear_view(t_prompt *prompt);

void mx_get_input(t_prompt *prompt, int *code) {
    clear_prompt(prompt);
    printf("%s%s", mx_str_prompt(), prompt->command);
    while (read(STDIN_FILENO, prompt->buff, 4)
           && prompt->index < ARG_MAX
           && handle_key(prompt, code)) {
        printf("%s%s", mx_str_prompt(), prompt->command);
        mx_handle_cursor(prompt);
        memset(prompt->buff, '\0', sizeof(prompt->buff));
    }
    mx_update_history(prompt);
}

static void set_code(char *buff, int *code, int index) {
    if (buff[0] == '\x03' && strlen(buff) == 1)
        *code = 130;
    if (buff[0] == '\x04' && !index && strlen(buff) == 1)
        *code = -1;
}

static bool handle_key(t_prompt *prompt, int *code) {
    set_code(prompt->buff, code, prompt->index);
    if (mx_match(prompt->buff, MX_NEW_LINE_CHARS))
        return false;
    if (prompt->buff[0] == '\x04' && !prompt->index)
        return false;
    clear_view(prompt);
    if (!mx_handle_history(prompt)) {
        mx_handle_print_char(prompt);
        strcpy(prompt->tmp_command, prompt->command);
    }
    return true;
}

static void clear_view(t_prompt *prompt) {
    mx_backspace(prompt->index + strlen(mx_str_prompt()));
    printf("%s%s", mx_str_prompt(), prompt->command);
    mx_backspace(prompt->index + strlen(mx_str_prompt()));
}

static void clear_prompt(t_prompt *prompt) {
    memset(prompt->command, '\0', sizeof(prompt->command));
    memset(prompt->tmp_command, '\0', sizeof(prompt->tmp_command));
    prompt->index = 0;
    prompt->cursor_index = 0;
    prompt->end = false;
}
