
#include "cmos.h"
#include "system.h"
int io_in(int addr);
int io_out(int addr);
int get_eax(void);
int get_ebx(void);
int get_ecx(void);
int get_edx(void);
int get_edi1(void);
int get_esi(void);
int16 get_cs(void);
int get_ebp(void);
int get_eip(void);
int get_esp(void);
int get_cpu(void);
int pusha(void);
int popa(void);
void ASM_call(int16 xy);
const char tips[] = "SimpleOS-"; //提示是不可更改的
const char tips_1[] = " # ";
int x = 0;
int y = 0;
int cons_x = 0;
int cons_y = 0;
int times = 1;
int caps_lock = 0;
char *name_1;
char user[8];

char *string;
const char *NULL_S;
const char *error_tips = "USER wrong!!!!! Please wait 1sec\n";
void shell(void)
{

	int i, j, k, x, y;
	char s[30];
	char *p, *q;
	char *user1_1;
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
	struct TASK *task = task_now();
	int *fat = (int *)memman_alloc_4k(memman, 4 * 2880);
	if(fopen("VIEW") == 0){
		print("VIEW CONFIG ERROR\n");
		for(;;);
	}
	if(fopen("USER") == 0){
		print("USER not found, please to windows create name:(e.g. USER.txt)!\n");
		for(;;);
	}
	if(fopen("PASSWORD") == 0){
		print("PASSWORD not found, please to windows create name:(e.g. PASSWORD.txt)!\n");
		for(;;);
	}
	if (strcmp("true", fopen("VIEW")) == 0)
	{
		system_init();
		clear();

		print("------login:\n");
	start1:
		print("----User:");
		string = input_str_SM();
		if (strcmp(string, fopen("USER")) != 0)
		{
			print(error_tips);
			putchar('\0'); //我也不知道这串有什么用 反正不加就出bug
			sleep(1);
			goto start1;
		}
		print("----Password:");
		string = input_password();
		if (strcmp(string, fopen("PASSWORD")) != 0)
		{
			print("PASSWORD wrong!!!!! Please wait 1sec\n");
			putchar('\0');
			sleep(1);
			goto start1;
		}
	}
	else
	{
		user1_1 = fopen("USER");
		if (getlength(user1_1) > 7)
		{
			user[0] = user1_1[0];
			user[1] = user1_1[1];
			user[2] = user1_1[2];
			user[3] = user1_1[3];
			user[4] = user1_1[4];
			user[5] = user1_1[5];
			user[6] = user1_1[6];
		}
		else
		{
			for (i = 0; i < getlength(user1_1); i++)
			{
				user[i] = user1_1[i];
			}
		}
	}
	print("SimpleOS 32bits 0.0.0.1 beta\n");
	print("this system made by min0911 of DogheadOS Stduio\n");
	print("system.start.msg::\"hello, world    hello, 32bits world    helloC\"\n");
	char *bmp;
	while (1)
	{
		/*
		 * 输入命令
		 *
		 * 检测命令
		*/
		char *inputstr;
		print(tips);
		print(user);
		print(tips_1);
		inputstr = input_str_SM();
		command_run(inputstr);
	}
}
void command_run(str inputstr)
{
	int i, j, k, x, y;
	char s[30];
	char *p, *q;
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
	struct TASK *task = task_now();
	int *fat = (int *)memman_alloc_4k(memman, 4 * 2880);
	if (strcmp(inputstr, "ls") == 0)
	{
		cmd_dir();
	}
	else if (strcmp(inputstr, "clear") == 0)
	{
		clear();
	}
	else if (strncmp(inputstr, "cat ", 4) == 0)
	{
		type_deal(inputstr);
	}
	else if (strcmp(inputstr, "test") == 0)
	{
		print("hello, world\n"); //打印一个字符串
		print(itoa(my_atoi("50")));
		print("\n");
	}
	else if (strcmp(inputstr, "calc") == 0)
	{
		calcdeal();
	}
	else if (strcmp(inputstr, "shutdown") == 0)
	{
		return;
	}
	else if (strcmp(inputstr, "res") == 0)
	{
		view_res();
	}
	else if (strncmp(inputstr, "color ", 6) == 0)
	{
		string = inputstr;
		color_deal();
	}
	else if (strcmp(inputstr, "0xfff000") == 0)
	{
		color_egg_deal();
	}
	else if (strcmp(inputstr, "time") == 0)
	{
		print_date();
		print("\n");
	}
	else if (strcmp(inputstr, "timer") == 0)
	{
		clear();
		cons_x = 150;
		cons_y = 150;
		while (1)
		{
			print_date();
			printchar(' ');
			printchar(' ');
			printchar(' ');
			printchar(' ');
			x = 0;
			y = 0;
		}
	}
	else if (strcmp(inputstr, "wordend") == 0)
	{
		Word_END_gameMain();
	}
	else if (strcmp(inputstr, "gobang") == 0)
	{
		gobangMain();
	}

	else if (strcmp(inputstr, "hex") == 0)
	{
		hex_view_main();
	}
	else if(strcmp(inputstr,"code") == 0){
		macineMAIN();
	}
	else
	{
		if (cmd_app(fat, inputstr) == 0)
		{
			print("bad command!\n");
		}
	}
}
void color_deal()
{
	char hex_s[3] = {string[6], string[7], '\0'};
	int colo = htoi(hex_s);
	if (colo == -1)
	{
		print("Please enter the correct hexadecimal number\n");
		return;
	}
	set_color(colo);
}
void color_egg_deal()
{
	int r1, i;
	for (i = 0; i < 1000; i++)
	{
		r1 = RAND() % 10;
		if (r1 == 2)
		{
			print("\n");
		}
		else if (r1 < 5)
		{
			print("1");
		}
		else
		{
			print("0");
		}
		sleep_m(3);
	}
	print("\n");
}
void printdeal(str st)
{
	int i;
	for (i = 6; i < getlength(st); i++)
	{
		printchar(st[i]);
	}
	print("\n");
}
void calcdeal()
{
	print("1.add\n2.sub\n3.mul\n4.div\nplease switch:");
	str in1 = input_str_SM();
	int num1;
	int num2;
	if (strcmp(in1, "1") == 0)
	{
		print("please enter first Addend:");
		in1 = input_str_SM();
		num1 = atoi(in1);
		print("please enter second Addend:");
		in1 = input_str_SM();
		num2 = atoi(in1);
		print("result=");
		print(itoa(num1 + num2));
		print("\n");
	}
	else if (strcmp(in1, "2") == 0)
	{
		print("please enter first Minute:");
		in1 = input_str_SM();
		num1 = atoi(in1);
		print("please enter second Minus:");
		in1 = input_str_SM();
		num2 = atoi(in1);
		print("result=");
		print(itoa(num1 - num2));
		print("\n");
	}
	else if (strcmp(in1, "3") == 0)
	{
		print("please enter first multiplier:");
		in1 = input_str_SM();
		num1 = atoi(in1);
		print("please enter second multiplier:");
		in1 = input_str_SM();
		num2 = atoi(in1);
		print("result=");
		print(itoa(num1 * num2));
		print("\n");
	}
	else if (strcmp(in1, "4") == 0)
	{
		print("please enter first Dividend:");
		in1 = input_str_SM();
		num1 = atoi(in1);
		print("please enter second divisor:");
		in1 = input_str_SM();
		num2 = atoi(in1);
		print("result=");
		print(itoa(num1 / num2));
		print("\n");
	}
	else
	{
		print("error[404]\n");
	}
}

void type_deal(str name)
{
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	int i, j, k, c, x, t = 0;
	char s[30] = {0};
	char *p;
	char *BMP = "A";

	strcpy(name_1, name);
	for (i = 0; i < 224; i++)
	{
	RE:
		if (finfo[i].name[0] == 0x00)
		{
			break;
		}
		if (finfo[i].name[0] != 0xe5)
		{
			if ((finfo[i].type & 0x18) == 0)
			{

				for (j = 0; j < 8; j++)
				{
					s[j] = finfo[i].name[j];
				}
				s[9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				//print(s);
				int flag = 1;
				for (x = 0; x <= 8; ++x)
				{

					if (s[x] != 0x20)
					{
						if (s[x] == name[x + 4])
						{
							//putchar(name[x]);
						}
						else
						{
							flag = 0;
						}
					}
					else
					{
						break;
					}
				}
				//goto done
				if (flag == 1)
				{
					c = finfo[i].clustno;
					goto done;
				}
				//*NA = 0;
			}
		}
	}
	goto NO;
done:
	p = (char *)(c * 512 + 0x003e00 + 0x8000);

	print(p);
	print("\n");
	return;
NO:
	//print("NO\n");

	print("Not found this file(you can\'t add EXT in you input name)\n");
}

void cmd_dir()
{
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	int i, j, k;
	char s[30] = {0};
	print("FILENAME   EXT   LENGTH\n");
	for (i = 0; i < 224; i++)
	{
		if (finfo[i].name[0] == 0x00)
		{
			break;
		}
		if (finfo[i].name[0] != 0xe5)
		{
			if ((finfo[i].type & 0x18) == 0)
			{

				for (j = 0; j < 8; j++)
				{
					s[j] = finfo[i].name[j];
				}
				s[9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];

				for (k = 0; k < 12; ++k)
				{
					if (k == 9)
					{
						print("   ");
					}
					if (s[k] == '\n')
					{
						print("   ");
					}
					else
					{
						putchar(s[k]);
					}
				}
				print("    ");
				print(itoa(finfo[i].size));
				print("\n");
			}
		}
	}
	print("\n");
	//&s = 0;
	return;
}
char *fopen(const char *name)
{
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	int i, j, k, c, x, t = 0;
	char s[30] = {0};
	char *p;
	char *BMP = "A";

	strcpy(name_1, name);
	for (i = 0; i < 224; i++)
	{
	RE:
		if (finfo[i].name[0] == 0x00)
		{
			break;
		}
		if (finfo[i].name[0] != 0xe5)
		{
			if ((finfo[i].type & 0x18) == 0)
			{

				for (j = 0; j < 8; j++)
				{
					s[j] = finfo[i].name[j];
				}
				s[9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				//print(s);
				int flag = 1;
				for (x = 0; x <= 8; ++x)
				{

					if (s[x] != 0x20)
					{
						if (s[x] == name[x])
						{
							//putchar(name[x]);
						}
						else
						{
							flag = 0;
						}
					}
					else
					{
						break;
					}
				}
				//goto done
				if (flag == 1)
				{
					c = finfo[i].clustno;
					goto done;
				}
				//*NA = 0;
			}
		}
	}
	goto NO;
done:
	p = (char *)(c * 512 + 0x003e00 + 0x8000);

	return p;
NO:
	//print("NO\n");

	return 0;
}

int fopen_getlen(const char *name)
{
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	int i, j, k, c, x, t = 0;
	char s[30] = {0};
	char *p;
	char *BMP = "A";

	strcpy(name_1, name);
	for (i = 0; i < 224; i++)
	{
	RE:
		if (finfo[i].name[0] == 0x00)
		{
			break;
		}
		if (finfo[i].name[0] != 0xe5)
		{
			if ((finfo[i].type & 0x18) == 0)
			{

				for (j = 0; j < 8; j++)
				{
					s[j] = finfo[i].name[j];
				}
				s[9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				//print(s);
				int flag = 1;
				for (x = 0; x <= 8; ++x)
				{

					if (s[x] != 0x20)
					{
						if (s[x] == name[x])
						{
							//putchar(name[x]);
						}
						else
						{
							flag = 0;
						}
					}
					else
					{
						break;
					}
				}
				//goto done
				if (flag == 1)
				{
					c = finfo[i].size;
					goto done;
				}
				//*NA = 0;
			}
		}
	}
	goto NO;
done:
	//p = (char *)(c * 512 + 0x003e00 + 0x8000);

	return c;
NO:
	//print("NO\n");

	return -1;
}

void cmd_type(const char *name)
{
	struct FILEINFO *finfo = (struct FILEINFO *)(0x8000 + 0x002600);
	int i, j, k, c, x, t = 0;
	char s[30] = {0};
	char *p;
	char *BMP = "A";

	strcpy(name_1, name);
	for (i = 0; i < 224; i++)
	{
	RE:
		if (finfo[i].name[0] == 0x00)
		{
			break;
		}
		if (finfo[i].name[0] != 0xe5)
		{
			if ((finfo[i].type & 0x18) == 0)
			{

				for (j = 0; j < 8; j++)
				{
					s[j] = finfo[i].name[j];
				}
				s[9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				//print(s);
				int flag = 1;
				for (x = 0; x <= 8; ++x)
				{

					if (s[x] != 0x20)
					{
						if (s[x] == name[x])
						{
							//putchar(name[x]);
						}
						else
						{
							flag = 0;
						}
					}
					else
					{
						break;
					}
				}
				//goto done
				if (flag == 1)
				{
					c = finfo[i].clustno;
					goto done;
				}
				//*NA = 0;
			}
		}
	}
	goto NO;
done:
	p = (char *)(c * 512 + 0x003e00 + 0x8000);
	print(p);
	print("\n");
	return;
NO:
	print("Not found this file\n");
	return;
}
int str_c(const char *a, const char *b)
{
	return strcmp(a, b);
}
/*****/

char *itoa(int a)
{
	char *res;
	Myitoa(res, a);
	res = revstr(res, getlength(res));
	return res;
}
int my_atoi(const char *in)
{
	int tmp = 0;
	int ret = 0;
	int sign = 0; //符号标记
	if (in[0] == '-')
	{ //是否为负数
		sign = -1;
	}

	while (*in != '\0') //遍历字符串
	{
		if (*in >= '0' && *in <= '9') //ASCII码在此区间内
		{
			tmp = *in - '0';
			ret = 10 * ret + tmp;
			in++;
			if (ret > 214748364)
			{ //考虑溢出情况
				print("error: data overflow...\n");
				break;
			}
		}
		else
		{ //忽略其他字符
			in++;
			continue;
			//break;                  //检测到非数字的，可直接退出
		}
	}

	if (sign < 0)
		return 0 - ret;
	else
		return ret;
}
void Myitoa(char *str, int n)
{
	int i, j = 0;
	if (n == 0)
	{
		*str = '0';
		j++;
		*str++;
	}
	while (n != 0)
	{
		*str = n % 10 + '0';
		n /= 10;
		j++;
		str++;
	}

	*str = '\0';
}

int scanToAcill(int scan)
{ //RETURN -1 = NOME
	if (scan == 3218974)
	{
		return 'a';
	}
}
char *revstr(char *str, int len)
{

	char *start = str;
	char *end = str + len - 1;
	char ch;

	if (str != 0)
	{
		while (start < end)
		{
			ch = *start;
			*start++ = *end;
			*end-- = ch;
		}
	}
	return str;
}
char *constring(char *s, char *t)
{
	int i, j;
	char *q;
	for (i = 0; s[i] != '\0'; i++)
	{
		q[i] = s[i];
	}
	for (j = 0; t[j] != '\0'; j++)
	{
		q[i + j] = t[j];
	}
	q[i + j] = '\0';
	return q;
}
void putstr(const char *str, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (y == 24 && x >= 160)
		{
			screen_ne();
		}
		if (str[i] == 0x0d)
		{
			continue;
		}
		putchar(str[i]);
	}
}
void print(const char *str)
{
	putstr(str, getlength(str));
}
int getlength(const char *str)
{
	int i;
	for (i = 0;; ++i)
	{
		if (str[i] == '\0')
		{
			return i;
		}
	}
}
char *input_password()
{
	char *str;
	int ch;
	char *bmp = "A";
	int bmp_x = x;
	//int times = 0;

	char *str2;
	*str = 0;
fl:
	while (1)
	{
		if (y == 24 && x >= 160)
		{
			screen_ne();
		}
		ch = input_char_inSM();
		if (ch == -1)
		{
			continue;
		}
		if (caps_lock == 1)
		{
			if (keytable[ch] == '\b')
			{
				if (x == bmp_x)
				{
					continue;
				}
				str[getlength(str) - 1] = '\0';
			}
			if (keytable[ch] != '\b' && keytable[ch] != 97 && keytable1[ch] != 10)
			{
				bmp[0] = keytable[ch];
				str = cat(str, bmp);
			}
			if (keytable[ch] == 97)
			{
				caps_lock = 0;
				goto fl;
				continue;
			}
			if (keytable[ch] != '\b' && keytable[ch] != '\n')
			{
				putchar('*');
			}
			else
			{
				putchar(keytable[ch]);
			}
			//times++;
		}
		if (caps_lock == 0)
		{
			if (keytable1[ch] == '\b')
			{
				if (x == bmp_x)
				{
					continue;
				}
				str[getlength(str) - 1] = '\0';
			}
			if (keytable1[ch] != '\b' && keytable1[ch] != 70 && keytable1[ch] != 10)
			{
				bmp[0] = keytable1[ch];
				str = cat(str, bmp);
			}
			if (keytable1[ch] == 70)
			{
				caps_lock = 1;
				goto fl;
			}
			if (keytable1[ch] != '\b' && keytable1[ch] != '\n')
			{
				putchar('*');
			}
			else
			{
				putchar(keytable1[ch]);
			}
			//times++;
		}
		if (keytable1[ch] == 10)
		{
			break;
		}
	}
	times = 0;
	return str;
}
char *input_str_SM()
{
	char *str;
	int ch;
	char *bmp = "A";
	int bmp_x = x;
	//int times = 0;

	char *str2;
	*str = 0;
fl:
	while (1)
	{

		if (y == 24 && x >= 160)
		{
			screen_ne();
		}
		ch = input_char_inSM();
		if (ch == -1)
		{
			continue;
		}
		if (caps_lock == 1)
		{
			if (keytable[ch] == '\b')
			{
				if (x == bmp_x)
				{
					continue;
				}
				str[getlength(str) - 1] = '\0';
			}
			if (keytable[ch] != '\b' && keytable[ch] != 97 && keytable1[ch] != 10)
			{
				bmp[0] = keytable[ch];
				str = cat(str, bmp);
			}
			if (keytable[ch] == 97)
			{
				caps_lock = 0;
				goto fl;
				continue;
			}
			putchar(keytable[ch]);
			//times++;
		}
		if (caps_lock == 0)
		{
			if (keytable1[ch] == '\b')
			{
				if (x == bmp_x)
				{
					continue;
				}
				str[getlength(str) - 1] = '\0';
			}
			if (keytable1[ch] != '\b' && keytable1[ch] != 70 && keytable1[ch] != 10)
			{
				bmp[0] = keytable1[ch];
				str = cat(str, bmp);
			}
			if (keytable1[ch] == 70)
			{
				caps_lock = 1;
				goto fl;
			}
			putchar(keytable1[ch]);
			//times++;
		}
		if (keytable1[ch] == 10)
		{
			break;
		}
	}
	times = 0;
	return str;
}
char *cat(char *a, char *b)
{

	char *p = a, *q = b;

	while (*p++)
		;

	p--;

	while (*p++ = *q++)
		;

	*p = '\0';

	return a;
}
int input_char_inSM()
{
	int i;
	while (1)
	{
		if (fifo8_status(&keyfifo) == 0)
		{				 //键盘没有被按下或弹起
			io_stihlt(); //硬件系统挂起，直到发生了某个中断
		}
		else
		{
			i = fifo8_get(&keyfifo); //获取按键编码
			io_sti();
			if (i < 0x80)
			{
				break;
			}
		}
	}
	return i;
}
void putchar(char ch)
{

	if (x == 160)
	{
		y++;
		cons_y++;
		x = 0;
		cons_x = 0;
		Move_Cursor(cons_x, cons_y);
	}

	if (ch == '\n')
	{
		if (y == 24)
		{
			screen_ne();
			return;
		}
		y++;
		//*(char *)(0xb8000 + cons_y * 160 + cons_x) = ' ';
		cons_y++;
		x = 0;

		cons_x = 0;
		Move_Cursor(cons_x, cons_y);
		return;
	}
	else if (ch == '\0')
	{
		return;
	}
	else if (ch == '\b')
	{
		if (x == 0)
		{
			return;
		}
		*(char *)(0xb8000 + y * 160 + x - 2) = ' '; /*显示位置为第23行*/
		x -= 2;
		cons_x -= 2;
		//*(char *)(0xb8000 + cons_y * 160 + cons_x + 4) = ' ';
		cons_x += 1;
		Move_Cursor(cons_x, cons_y);
		return;
	}
	//*(char *)(0xb8000 + cons_y * 160 + cons_x) = ' ';
	cons_x += 1;
	Move_Cursor(cons_x, cons_y);
	*(char *)(0xb8000 + y * 160 + x) = ch; /*显示位置为第23行*/
	x += 2;
}
void clear()
{
	int i;
	int j;
	for (i = 0; i < 160; i += 2)
	{
		for (j = 0; j < 25; j++)
		{
			*(char *)(0xb8000 + j * 160 + i) = ' ';
		}
	}
	x = 0;
	y = 0;
	cons_x = 0;
	cons_y = 0;
	Move_Cursor(cons_x, cons_y);
}
int p2cWaitForKbRead()
{
	int in = 0;
	in = io_in(0x64);
	if (in == 0x02)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void Move_Cursor(int16 x, int16 y)
{
	int res = y * 80 + x;
	ASM_call(res);
}
void screen_ne() /*向下滚动一行*/
{
	char *p = 0xb8000 + 0 * 160 + 0;
	int i, j;
	/*清空第一行*/
	for (i = 0; i < 160; i += 2)
	{
		*(char *)(0xb8000 + 0 * 160 + i) = 0;
	}
	//clear();
	for (i = 0; i < 25; i++)
	{
		for (j = 0; j < 80; j++)
		{
			//*(char *)(0xb8000 + i * 160 + j) = *(char *)(0xb8000 + i+1 * 160 + j);
			char ch = *(char *)(0xb8000 + i * 160 + j * 2);
			outputchar_b800(j, i - 1, ch);
		}
	}

	for (j = 0; j < 160; j += 2)
	{
		*(char *)(0xb8000 + 24 * 160 + j) = 0;
	}

	cons_x = 0;
	x = 0;
	Move_Cursor(cons_x, cons_y);
}
void outputchar_b800(int x, int y, char ch)
{
	x *= 2;
	*(char *)(0xb8000 + y * 160 + x) = ch;
}
void sleep(int sec)
{
	init_pit();
	while (1)
	{
		print("\0");
		if (timerctl.count == sec * 100)
		{

			break;
		}
	}
	times = 1;
}
void sleep_m(int sec)
{
	init_pit();
	while (1)
	{
		print("\0");
		if (timerctl.count == sec)
		{

			break;
		}
	}
	putchar('\0');
	//times = 1;
}
void f(int x, char *p)
{
	int div;
	int k = 0;

	int i;
	if (x == 0)
	{
		*p = '0';
	}
	if (x < 0)
	{
		x = -x;
		*p = '-';
		p++;
	}
	for (i = 0; x != 0; i++)
	{
		div = x % 16;
		x = x / 16;
		if ((div - 10) < 0)
			*(p + i) = div + '0';
		else
			*(p + i) = 'A' + div - 10;
		div = 0;
		k++;
	}
	//reverse
	for (i = 0; i < k / 2; i++)
	{
		char temp = p[i];
		p[i] = p[k - 1 - i];
		p[k - 1 - i] = temp;
	}
}
void printchar(char ch)
{
	char ch1[2] = {ch, '\0'};
	print(ch1);
}
void print_32bit_hex(int a)
{
	int i, r = 0;
	char s[20];
	sprintf(s, "%x", a);
	for (i = 0; i < 8 - getlength(s); i++)
	{

		print("0");
	}
	for (i = 0; i < getlength(s); i++)
	{
		printchar(s[i]);
	}
}
void view_res()
{
	print("RES:\n");
	print("EAX=");
	print_32bit_hex(get_eax());
	print("\nECX=");
	print_32bit_hex(get_ecx());
	print("\nEDX=");
	print_32bit_hex(get_edx());
	print("\nESI=");
	print_32bit_hex(get_esi());
	print("\nEDI=");
	print_32bit_hex(get_edi());
	print("\nEBP=");
	print_32bit_hex(get_ebp());
	print("\nEIP=");
	print_32bit_hex(get_eip());
	print("\nESP=");
	print_32bit_hex(get_esp());
	print("\nCS=");
	print_32bit_hex(get_cs());
	print("\n");
}
void intreturn(int ret);
void system_init()
{
	int i;
	clear();
	Print_Flashing_text("Loading . . .", 3, 45);
	print(itoa(get_year()));
	print("/");
	print(itoa(get_mon_hex()));
	print("/");
	print(itoa(get_day_of_month()));
	print("/");
	get_day_eng();
	print(" ");
	print(itoa(get_hour_hex()));
	print(":");
	print(itoa(get_min_hex()));
	print(":");
	print(itoa(get_sec_hex()));
	print("\n");
	print("RES:\n");
	print("EAX=");
	print_32bit_hex(get_eax());
	print("\nECX=");
	print_32bit_hex(get_ecx());
	print("\nEDX=");
	print_32bit_hex(get_edx());
	print("\nESI=");
	print_32bit_hex(get_esi());
	print("\nEDI=");
	print_32bit_hex(get_edi());
	print("\nEBP=");
	print_32bit_hex(get_ebp());
	print("\nEIP=");
	print_32bit_hex(get_eip());
	print("\nESP=");
	print_32bit_hex(get_esp());
	print("\nCS=");
	print_32bit_hex(get_cs());
	print("\nit\'s run in 32 bits mode\nSystem.test.func:\noutput:hello, world\nINT---");
	sleep(2);
	print("OK!\n");
	print("RANDNUM(1,100)-----");
	print(itoa(RAND() % 100 + 1));
	print("\n");
	sleep(1);
	print("test switch color--green\n");
	set_color(0x0a);
	sleep(1);
	set_color(0x07);
	print("(DogPower Kernel 32bits v0.0.0.100 build 097)\n");
	sleep(1);
	char *user1_1;
	user1_1 = fopen("USER");
	if (getlength(user1_1) > 7)
	{
		user[0] = user1_1[0];
		user[1] = user1_1[1];
		user[2] = user1_1[2];
		user[3] = user1_1[3];
		user[4] = user1_1[4];
		user[5] = user1_1[5];
		user[6] = user1_1[6];
	}
	else
	{
		for (i = 0; i < getlength(user1_1); i++)
		{
			user[i] = user1_1[i];
		}
	}
}
void set_color(int8 color)
{
	int i, j;
	for (i = 1; i < 160; i++)
	{
		for (j = 0; j < 25; j++)
		{
			if (i % 2 == 1)
			{
				*(char *)(0xb8000 + j * 160 + i) = color;
			}
		}
	}
}
int8 get_this_color()
{
	return *(char *)(0xb8000 + 0 * 160 + 1);
}
int htoi(char s[])
{
	int n = 0;
	int i = 0;
	while (s[i] != '\0' && s[i] != '\n')
	{
		if (s[i] == '0')
		{
			if (s[i + 1] == 'x' || s[i + 1] == 'X')
				i += 2;
		}
		if (s[i] >= '0' && s[i] <= '9')
		{
			n = n * 16 + (s[i] - '0');
		}
		else if (s[i] >= 'a' && s[i] <= 'f')
		{
			n = n * 16 + (s[i] - 'a') + 10;
		}
		else if (s[i] >= 'A' && s[i] <= 'F')
		{
			n = n * 16 + (s[i] - 'A') + 10;
		}
		else
			return -1;
		++i;
	}
	return n;
}
void get_day_eng()
{
	if (get_day_of_week() == 0)
	{
		print("Sun");
		return;
	}
	if (get_day_of_week() == 1)
	{
		print("Mon");
		return;
	}
	if (get_day_of_week() == 2)
	{
		print("Tue");
		return;
	}
	if (get_day_of_week() == 3)
	{
		print("Wed");
		return;
	}
	if (get_day_of_week() == 4)
	{
		print("Thu");
		return;
	}
	if (get_day_of_week() == 5)
	{
		print("Fri");
		return;
	}
	if (get_day_of_week() == 6)
	{
		print("Sat");
		return;
	}
}
void print_date()
{
	print(itoa(get_year()));
	print("/");
	print(itoa(get_mon_hex()));
	print("/");
	print(itoa(get_day_of_month()));
	print("/");
	get_day_eng();
	print(" ");
	print(itoa(get_hour_hex()));
	print(":");
	print(itoa(get_min_hex()));
	print(":");
	print(itoa(get_sec_hex()));
}
void gotoxy(int a, int b)
{
	x = a * 2;
	y = b;
	cons_x = a;
	cons_y = b;
	Move_Cursor(a, b);
}
char getch()
{
	char ch = input_char_inSM();
	if (caps_lock == 0)
	{
		if (keytable1[ch] != 70)
		{
			return keytable1[ch];
		}
		else
		{
			caps_lock = 1;
			return 0;
		}
	}
	else
	{
		if (keytable[ch] != 97)
		{
			return keytable[ch];
		}
		else
		{
			caps_lock = 0;
			return 0;
		}
	}
}
void Print_Flashing_text(const char *text, int times, int time)
{
	int len = getlength(text);
	int now_x = x, now_y = y;
	int i, j, k, t;
	t = times;
	while (t != 0)
	{
		print(text);
		gotoxy(now_x, now_y);
		sleep_m(time);
		for (i = 0; i < len; i++)
		{
			print(" ");
		}
		gotoxy(now_x, now_y);
		sleep_m(time);
		t--;
	}
}
int cmd_app(int *fat, char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
	struct FILEINFO *finfo;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
	char name[18], *p, *q;
	struct TASK *task = task_now();
	int i;

	/*根据命令行生成文件名*/
	for (i = 0; i < 13; i++)
	{
		if (cmdline[i] <= ' ')
		{
			break;
		}
		name[i] = cmdline[i];
	}
	name[i] = 0; /*暂且将文件名的后面置为0*/

	/*寻找文件 */
	finfo = file_search(name, (struct FILEINFO *)(ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0 && name[i - 1] != '.')
	{
		/*由于找不到文件，故在文件名后面加上“.hrb”后重新寻找*/
		name[i] = '.';
		name[i + 1] = 'H';
		name[i + 2] = 'R';
		name[i + 3] = 'B';
		name[i + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *)(ADR_DISKIMG + 0x002600), 224);
	}

	if (finfo != 0)
	{
		/*找到文件的情况*/
		p = (char *)memman_alloc_4k(memman, finfo->size);
		q = (char *)memman_alloc_4k(memman, 64 * 1024);
		*((int *)0xfe8) = (int)p;
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *)(ADR_DISKIMG + 0x003e00));
		set_segmdesc(gdt + 1003, finfo->size - 1, (int)p, AR_CODE32_ER);
		set_segmdesc(gdt + 1004, 64 * 1024 - 1, (int)q, AR_DATA32_RW);
		start_app(0, 1003 * 8, 64 * 1024, 1004 * 8, &(task->tss.esp0));
		memman_free_4k(memman, (int)p, finfo->size);
		memman_free_4k(memman, (int)q, 64 * 1024);
		return 1;
	}
	/*没有找到文件的情况*/
	return 0;
}
int get_x()
{
	return x;
}
int get_y()
{
	return y;
}