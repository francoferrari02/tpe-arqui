#include "include/sys_calls.h"
#include <stdint.h>
#include "include/userLib.h"
#include "include/shell.h"
#include "include/colores.h"

char * v = (char*)0xB8000 + 79 * 2;

static const Color WHITE = {255,255,255};
static const Color BLACK = {0,0,0};
static const Color LIGHT_BLUE = {255, 180,180};

int main() {

	shell();


	return 0;
} 