W=-fno-common -fno-asynchronous-unwind-tables -fno-stack-protector
O=t
S=t.c
A=`uname -ms`
F=-DPF -DPFA="\"$A\""
L=clang
G=gcc
T=tcc

m: c
	@$G -Os $W -DNOLC -DPF -Werror -pedantic -nostdlib -ffreestanding -om m.c s.S
	@echo
	@strip m
	@ls -la m
	@./m

mm: c
	@$L  $F -omm m.c -Os -DNOPF -DPFA="\"$A\""
	@#$G -Os $W -DNOLC -DPF -Werror -pedantic -nostdlib -ffreestanding -om m.c s.S
	@#echo
	@#strip m
	@#ls -la m
	@./mm

t:
	@$T $F -w -o$O $S -O0 -g && ./$O
t32:
	@$T $F -m32 -w -o$O $S -Os && ./$O
t64:
	@$T $F -m64 -DNOLC -nostdlib -w -o$O $S s.S -Os && ./$O

g:
	@$G $F $W -o$O $S -O0 -g && ./$O
g32:
	@$G $F -m32 $W -o$O $S -Os && ./$O
g64:
	@$G $F -m64 -DNOLC $W -nostdlib -ffreestanding -o$O $S s.S -Os && ./$O

l:
	@$L $F -o$O $W $S -O0 -g && ./$O
l32:
	@$L $F -m32 -o$O $W $S -Os && ./$O
l64:
	@$L $F -m64 -DNOLC -nostdlib -ffreestanding -o$O $S s.S -Os -g && ./$O

s64:
	@$L $F -DLIB -Wno-pointer-sign $W -shared -fPIC -nostdlib -rdynamic $S -Os -o lib$O.so

r:
	@$G -Wno-format $F -o$O $S -Os -DNOPF -DPFA="\"$A\"" && ./$O

arm64:
	@/opt/clang/bin/clang $F -DNOLC -o$O $W $S -Os && ./$O

c:
	@rm -f m pf

.PHONY: c all

#/:~
