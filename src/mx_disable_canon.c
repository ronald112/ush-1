#include "ush.h"

void mx_disable_canon() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, mx_get_tty());
}
