#include "system.h"
struct SEGMENT_DESCRIPTOR
{
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};
void macineMAIN()
{
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)0x00270000;
    char *p = memman_alloc_4k(memman, 512);
    *((int *)0xfe8) = (int)p;
    print("mem=");
    print_32bit_hex((int)p);
    print(":0\n");
    char hex[3];
    int HEX[512];
    int k = 0, i = 0, j = 0;
INPUR_START:
    k = 0;
    print("INPUT CODE:");
    while (k < 512)
    {
    one:
        hex[0] = getch();
        if (hex[0] == 10)
        {
            break;
        }
        if (hex[0] == '\b')
        {
            if (k > 0)
            {
                k--;
                HEX[k] = 0;
                print("\b\b");
            }
            goto twe;
        }
        if (hex[0] == 'Q' || hex[0] == 'q')
        {
            print("\n");
            goto INPUR_START;
        }
        printchar(hex[0]);
    twe:
        hex[1] = getch();
        if (hex[1] == 10)
        {
            hex[1] = '0';
            hex[2] = 0;
            HEX[k] = HEX2INT(hex);
            k++;
            break;
        }
        if (hex[1] == '\b')
        {

            HEX[k] = 0;
            print("\b");

            goto one;
        }
        if (hex[1] == 'Q' || hex[1] == 'q')
        {
            print("\n");
            goto INPUR_START;
        }
        printchar(hex[1]);
        hex[2] = 0;
        HEX[k] = HEX2INT(hex);
        k++;
        print(" ");
    }
    RESET:
    print("\n");
    print(">");
    char input_string[30];
    for(i=0;i<30;i++){
        char ch = getch();
        if(ch==10){
            input_string[i]='\0';
            break;
        }
        if(ch=='\b'){
            if(i!=0){
                input_string[i]='\0';
                i--;
                print("\b");

            }
            
        }
        else{
            input_string[i]=ch;
            printchar(ch);
        }
    }
    print("\n");
    if (strcmp(input_string, "r") == 0)
    {
        print("Loading...\n");
        for (i = 0; i < k; i++)
        {

            p[i] = HEX[i];
        }
        print("Loaded!\n");
        set_segmdesc(gdt + 1003, 512, (int)p, 0x409a);
        farcall(0, 1003 * 8);
        print("\n");
        print("program end\n");
        goto RESET;
    }else if(strcmp(input_string, "q") == 0){
        
    }else if(strcmp(input_string, "s") == 0 ){
        print("\n");
        goto INPUR_START;
    }
    else{
        print("bad command!!!!!!!!!\n");
        goto RESET;
    }
    memman_free_4k(memman, (int)p, 512);
}

int HEX2INT(char *p)
{
    char *str;
    long i = strtol(p, &str, 16);
    return (int)i;
}