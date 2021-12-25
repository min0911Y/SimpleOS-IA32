#include "console.h"
void asm_func(void);
void HariMain(void)
{
	char keybuf[32];
	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC的初始化已经完成，于是开放CPU的中断 */
	init_pit();
	io_out8(PIC0_IMR, 0xf8); /* 开放PIC1和键盘中断(11111001) */
	fifo8_init(&keyfifo, 32, keybuf);
	/*   *初始化中断  */
	clear();
	//Word_END_gameMain();
	shell();
	clear();
	print("\n\n\n\n\n\n\n\n\n\n\n                       Now you can safe to shutdown!\n");
	for (;;)
	{
	}
}

