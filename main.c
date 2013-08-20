#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "expander.h"
#include "lcd.h"

int main()
{
    exp_init();
    btn_printf(btn_blk());
}