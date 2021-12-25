OBJS_Kernelc = Kernelc.obj gobang.obj text_game.obj mem.obj file.obj fifo.obj timer.obj task.obj view_hex.obj Chinese.obj macinecode.obj naskfunc.obj hankaku.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) clean
	$(MAKE) img
	qemu-system-i386 -fda SimpleOS.img
# 镜像文件生成

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

Kernelc.bim : $(OBJS_Kernelc) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:Kernelc.bim stack:3136k map:Kernelc.map \
		$(OBJS_Kernelc)
# 3MB+64KB=3136KB

Kernelc.hrb : Kernelc.bim Makefile
	$(BIM2HRB) Kernelc.bim Kernelc.hrb 0

haribote.sys : asmhead.bin Kernelc.hrb Makefile
	copy /B asmhead.bin+Kernelc.hrb haribote.sys
haribote.img : ipl10.bin haribote.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		copy from:HELP.txt to:@: \
		copy from:readme.txt to:@: \
		copy from:key.txt to:@: \
		copy from:em.txt to:@: \
		copy from:user.ini to:@: \
		copy from:password.txt to:@: \
		copy from:text.txt to:@: \
		copy from:VIEW.INI to:@: \
		imgout:SimpleOS.img
		

# 其他指令

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# 运行程序

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(COPY) DogheadOS.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) Kernelc.map
	-$(DEL) Kernelc.bim
	-$(DEL) Kernelc.hrb
	-$(DEL) haribote.sys

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img