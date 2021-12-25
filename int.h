/*
base指的是32位的基地址，在这里分为了low,mid,high三个部分，这里分为3段主要是和80286时代的程序相兼容
limit是指段上限，表示这个段有多少字节
段上限分为高低，但是只有20位，这里看上去有3个字节但是limit_high的高四位用于保存段属性
limit_high的高四位和access_right这12位是段属性
*/
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1
#define ADR_IDT 0x0026f800
#define LIMIT_IDT 0x000007ff
#define ADR_GDT 0x00270000
#define LIMIT_GDT 0x0000ffff
#define ADR_BOTPAK 0x00280000
#define LIMIT_BOTPAK 0x0007ffff
#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_INTGATE32 0x008e
void load_gdtr(int limit, int addr);
void load_gdtr(int limit, int addr);
void asm_inthandler20(int *esp);
void asm_inthandler21(int *esp);
void asm_inthandler40(int *esp);
struct SEGMENT_DESCRIPTOR
{
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};
/*
offset_high&&offset_low,
中断或异常处理程序在其所在内存段中的偏移;
selector,处理程序所在内存段的段选择符;
dw_count,保留未用;
access_right,IDT描述符有效位,特权级(DPL),类型(TYPE)等属性
*/
struct GATE_DESCRIPTOR
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

/* GDT、IDT、descriptor table 关系处理 */
void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
	struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *)ADR_IDT;
	int i;

	/* GDT初始化 */
	for (i = 0; i <= LIMIT_GDT / 8; i++)
	{
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
	set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	load_gdtr(LIMIT_GDT, ADR_GDT);

	/* IDT初始化 */
	for (i = 0; i <= LIMIT_IDT / 8; i++)
	{
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);

	/* IDT设置*/
	set_gatedesc(idt + 0x20, (int)asm_inthandler20, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x21, (int)asm_inthandler21, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x40, (int)asm_inthandler40, 2 * 8, AR_INTGATE32);
	//set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
	//set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);

	return;
}
#define PORT_KEYDAT 0x0060
struct FIFO8 keyfifo;

void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *)0x8000;
	unsigned char data, s[4];
	/* 通知PIC IRQ-01 已经受理完毕 否则不处理下一次中断 */
	io_out8(PIC0_OCW2, 0x61);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);

	return;
}
void system_api_getch(void);
void inthandler40(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	int cs_base = *((int *)0xfe8);
	if (eax == 0x02)
	{
		printchar(edx);
	}
	else if (eax == 0x09)
	{
		char *str_ram = ebx + cs_base;
		print(str_ram);
	}
	else if (eax == 0x03)
	{
		intreturn(getch());
	}
	else if (eax == 0x04)
	{
		clear();
	}
	else if (eax == 0x06)
	{
		gotoxy(ebx, ecx);
	}
	else if (eax == 0x1f)
	{
		char *strin = edx+cs_base;
		strin = input_str_SM();
	}else if(eax == 0x20){
		char * str1 = ecx+cs_base;
		char * str2 = edx+cs_base;
		intreturn(strcmp(str1,str2));

	}else if(eax == 0x16F){
		sleep(ebx);
	}else if(eax == 0x16a){
		sleep_m(ebx);
	}

	return;
}
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff)
	{
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low = limit & 0xffff;
	sd->base_low = base & 0xffff;
	sd->base_mid = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low = offset & 0xffff;
	gd->selector = selector;
	gd->dw_count = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high = (offset >> 16) & 0xffff;
	return;
}

void init_pic(void)
/* PIC初始化 */
{
	io_out8(PIC0_IMR, 0xff); /* 禁止所有中断 */
	io_out8(PIC1_IMR, 0xff); /* 禁止所有中断 */

	io_out8(PIC0_ICW1, 0x11);	/* 边缘触发模式（edge trigger mode） */
	io_out8(PIC0_ICW2, 0x20);	/* IRQ0-7由INT20-27接收 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2相连 */
	io_out8(PIC0_ICW4, 0x01);	/* 无缓冲区模式 */

	io_out8(PIC1_ICW1, 0x11); /* 边缘触发模式（edge trigger mode） */
	io_out8(PIC1_ICW2, 0x28); /* IRQ8-15由INT28-2f接收 */
	io_out8(PIC1_ICW3, 2);	  /* PIC1由IRQ2连接 */
	io_out8(PIC1_ICW4, 0x01); /* 无缓冲区模式 */

	io_out8(PIC0_IMR, 0xfb); /* 11111011 PIC1以外全部禁止 */
	io_out8(PIC1_IMR, 0xff); /* 11111111 禁止所有中断 */
	return;
}

struct FIFO8
{
	unsigned char *buf;
	int p, q, size, free, flags;
};
/* FIFO */

#define FLAGS_OVERRUN 0x0001

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf)
/* 初始化FIFO缓冲区 */
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; /* 缓冲区大小 */
	fifo->flags = 0;
	fifo->p = 0; /* 下一个数据写入位置 */
	fifo->q = 0; /* 下一个数据读出位置 */
	return;
}

int fifo8_put(struct FIFO8 *fifo, unsigned char data)
/* 向FIFO传送数据并保存 */
{
	if (fifo->free == 0)
	{
		/* 没有空间了，溢出 */
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size)
	{
		fifo->p = 0;
	}
	fifo->free--;
	return 0;
}

int fifo8_get(struct FIFO8 *fifo)
/* 从FIFO取得一个数据 */
{
	int data;
	if (fifo->free == fifo->size)
	{
		/* 如果缓冲区为空则返回-1 */
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size)
	{
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

int fifo8_status(struct FIFO8 *fifo)
/* 报告一下积攒是数据量 */
{
	return fifo->size - fifo->free;
}
