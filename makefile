W=-fno-common -fno-asynchronous-unwind-tables -fno-stack-protector
O=t
S=t.c
A=`uname -ms`
F=-DPF
L=clang
G=gcc
T=tcc

all: c l

m: c
	@$G -Os $W -DPF -Werror -pedantic -nostdlib -ffreestanding -om m.c s.S
	@echo
	@strip m
	@ls -la m
	@./m

#tcc
t:
	@$T $F -w -o$O $S -O0 -g && ./$O
t32:
	@$T $F -m32 -w -o$O $S -Os && ./$O
t64:
	@$T $F -m64 -DNOLC -nostdlib -w -o$O $S s.S -Os && ./$O

#gcc
g:
	@$G $F $W -o$O $S -O0 -g && ./$O
g32:
	@$G $F -m32 $W -o$O $S -Os && ./$O
g64:
	@$G $F -m64 -DNOLC $W -nostdlib -ffreestanding -o$O $S s.S -Os && ./$O

#clang
l:
	@$L $F -o$O $W $S -O0 -g && ./$O
l32:
	@$L $F -m32 -o$O $W $S -Os && ./$O
l64:
	@$L $F -m64 -DNOLC -nostdlib -ffreestanding -o$O $S s.S -Os -g && ./$O

#lib
s64:
	@$L $F -DLIB -Wno-pointer-sign $W -shared -fPIC -nostdlib -rdynamic $S -Os -o lib$O.so

#local printf(f)
r:
	@$G -Wno-format $F -o$O $S -Os -DNOPF && ./$O # should segv

#clean
c:
	@rm -f m t l pf

.PHONY: c all

#/:~
