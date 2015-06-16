# Makefile for ARM and PC
# Copyrith(C) 2004-2008 HHDigital Inc.

#OBJS = ./*.o

all	: ALL
clean : CLEAN
arm	: ARM
pc	: X86

3512:
	make -f Makefile.arm chip=3512
	
3515:
	make -f Makefile.arm chip=3515

3516:
	make -f Makefile.arm chip=3516

3518:
	make -f Makefile.arm chip=3518

ALL:
	echo make command error!Please Input:"make 3512 " or "make 3515" or "make 3516"

CLEAN:
	rm -f cpu ${OBJS}
	

X86:	
	make -f Makefile.pc 

	

