#include "system.h"
int map[19][19];
int gx = 0, gy = 0;
int next = 0;
char qi[2] = {'@', 'O'};
void gobangMain()
{
    int i, j;
    const int this_color = get_this_color();
    set_color(0x0c);
    clear();
    Print_Flashing_text("Game Loader!!!!!!",5,45);
    set_color(0x3f);
    // init
    for (i = 0; i < 19; i++)
    {
        for (j = 0; j < 19; j++)
        {
            map[i][j] = 0;
        }
    }
    gx = 0;
    gy = 0;
    next = 0;
    // init finish
    // 此时 map这个二维数组的所有数据已经被清零

    print("--");
    clear();
    for(i=0;i<80;i++){
        for(j=0;j<25;j++){
            print("-");
        }
    }
    gotoxy(30,10);
    print("gobang game v0.1");
    gotoxy(30,11);
    print("   by min0911");
    while (1)
    {
        gotoxy(gx, gy);
        printchar('>');
        view_map();
        char ch = getch();
        if (ch == 100)
        {
            if (gx < 18)
            { //检测是否撞墙
                gx++;
            }
        }
        else if (ch == 115)
        {
            if (gy < 18)
            { //检测是否撞墙
                gy++;
            }
        }
        else if (ch == 119)
        {
            if (gy > 0)
            { //检测是否撞墙
                gy--;
            }
        }
        else if (ch == 97)
        {
            if (gx > 0)
            { //检测是否撞墙
                gx--;
            }
        }
        else if (ch == '\n')
        {
            if (map[gy][gx] != 1 && map[gy][gx] != 2)
            {
                map[gy][gx] = next + 1;
                if (next == 0)
                {
                    next = 1;
                }
                else if (next == 1)
                {
                    next = 0;
                }
            }
        }
        if (who_is_win() == 1)
        {
            clear();
            print("Black won");
            break;
        }
        else if (who_is_win() == 2)
        {
            clear();
            print("white won");
            break;
        }
    }
    set_color(this_color);
    print("\n");
}
void view_map()
{ //打印 地图（map）

    int i, j;
    gotoxy(0, 0);
    for (i = 0; i < 19; i++)
    {
        for (j = 0; j < 19; j++)
        {
            if (j != gx && i != gy)
            {
                if (map[i][j] == 0)
                {
                    print(" ");
                }
                else if (map[i][j] == 1)
                {
                    print("@");
                }
                else if (map[i][j] == 2)
                {
                    print("O");
                }
            }
            else if (j == gx && i != gy)
            {
                if (map[i][j] == 0)
                {
                    print(" ");
                }
                else if (map[i][j] == 1)
                {
                    print("@");
                }
                else if (map[i][j] == 2)
                {
                    print("O");
                }
            }
            else if (j != gx && i == gy)
            {
                if (map[i][j] == 0)
                {
                    print(" ");
                }
                else if (map[i][j] == 1)
                {
                    print("@");
                }
                else if (map[i][j] == 2)
                {
                    print("O");
                }
            }
            else if (j == gx && i == gy)
            {
                printchar('>');
            }
        }
        print("\n");
    }
}
/*
            if(map[i][j] == 0){
                print(" ");
            }else if(map[i][j] == 1){
                print("@ ");
            }else if(map[i][j] == 2){
                print("O ");
            }
*/
int who_is_win()
{ // result = 0 === no win no loss result = 1 pleayer win result = 2 === ai win
    int result = 0;
    int i, j, k;
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i + k][j + k] == 1)
                {
                    player++;
                }
                else if (map[i + k][j + k] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i + k][j - k] == 1)
                {
                    player++;
                }
                else if (map[i + k][j - k] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i][j - k] == 1)
                {
                    player++;
                }
                else if (map[i][j - k] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i][j + k] == 1)
                {
                    player++;
                }
                else if (map[i][j + k] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i + k][j] == 1)
                {
                    player++;
                }
                else if (map[i + k][j] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    for (i = 0; i < 19; ++i)
    {
        for (j = 0; j < 19; ++j)
        {
            int player = 0;
            int ai = 0;
            for (k = 0; k < 5; k++)
            {
                if (map[i - k][j] == 1)
                {
                    player++;
                }
                else if (map[i - k][j] == 2)
                {
                    ai++;
                }
            }
            if (player == 5)
            {
                result = 1;
                break;
            }
            else if (ai == 5)
            {
                result = 2;
                break;
            }
        }
    }
    return result;
}