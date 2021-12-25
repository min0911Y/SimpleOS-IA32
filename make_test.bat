DEL load.bin
del %1.obj
del %1.lst
del %1.hrb
del main.hrb
nasm load.asm -o load.bin
"../z_tools/cc1.exe" -o %1.gas %1.c
"../z_tools/GAS2NASK.exe" %1.gas %1.nas
"../z_tools/nask.exe" %1.nas %1.obj %1.lst
"../z_tools/OBJ2BIM.exe"  @../z_tools/haribote/haribote.rul  out:%1.bim stack:1024k map:%1.map  %1.obj %2 %3 %4 %5 %6
"../z_tools/BIM2HRB.exe" %1.bim main.hrb 0

copy /b load.bin+main.hrb %1.hrb
