## pf(f,a..)

[pf.h](https://github.com/kelas/pf/blob/master/pf.h) is a `printf()` shim intended for embedded systems, in about 40 sloc of C (plus a few handy macros).

what you get:

* format string parser recognizes `%[%0#-][09][.09*]dcups`
* `%d` and `%u` are also longs, `[lh]` prefixes are nop.
* works well on bare iron riscv, aarch, xtensa, 32/64.
* total absence of thread safety and floats.
* unsupported features are filtered out.
* freestanding elf ~9kb.

## tl;dr

minimal test:

```c
$ cd pf && cat m.c
#include"pf.h"
int
main(int argc, char **argv)
{
    printf("\n pf %d.%02d.%02d %s\n\n", PFV1, PFV2, PFV3, PFL);
    printf(" hey, %s\n\n", "guacamole!");
    printf(" %p //:~\n\n", 0x04);
    exit(0);
}
//:~
```

try it like so:

```bash
$ make m

 gcc -Os -DPF -DNOLC -nostdlib -ffreestanding -Werror -pedantic -om m.c s.S

 rwxrwxr-x 1 kelas kelas 9400 Feb 14 11:18 m

 pf 2021.02.14 (c) 2020 kparc / bsd

 hey, guacamole!

 0x04 //:~

$
```

## why?

generally speaking, there is no portable way to implement a drop-in nostdlib
replacement for `printf(3)`, since `va_arg` is an architecture-specific
built-in. in the unlikely case you want `printf` in a setting where you
absolutely must ditch stdlib, `pf` is a simple reality hack that provides
a bare minimum `printf(3)` at a price of:

* one syscall `write(2)`.
* one warning `int-conversion`, suppressed.

just like its real counterpart, `pf()` is extremely brittle. however, it
is a bit safer than `printf()` and performs some extra checks, e.g. stray
conversion specifiers are replaced with `(null)` instead of catching SEGV.

since `tcc` is the safest ISO C compiler available, you must define
the maximum number of arguments at compile time by defining `PFMX`, which
defaults to `8` and can't be more than `16`. calling `pf` with more than
`PFMX` positional arguments under `tcc` is a compile-time error.

for `clang` and `gcc` builds, the number of arguments is limited by the size of
your stack.

## test

`t.c` is like `m.c`, only more involved.

it can be built to your target architecture with `clang12`, `gcc10`, `tcc-mob`,
and supports both 32- and 64-bit. `makefile` targets are:

* `[tcl]` fat builds to default arch, `-O0 -g`
* `[tcl]32` fat builds to 32-bit
* `[tcl]64` nostdlib freestanding `-Os` (linux/x86_64 only, see `s.S`)
* `s64` dynamic library (not really useful)
* `r` reference output (use `printf` instead of `pf`)

```c
$ make m|t|c|l|t32|t64|c32|c64|l32|l64|s64|r

  pf 2021.02.14 Linux aarch64 (c) 2020 kparc / bsd

  (%)=(%) (kparc)=(kparc) (kparc)=(kparc) ()=()

   umx = (18446744073709551615)
   jmn = (-9223372036854775807)
   jmx = ( 9223372036854775807)
   imn = (         -2147483647)
   imx = (2147483647          )
  uimx = (4294967295          )

  pmx32 = (         0xffffffff)
  pmx64 = ( 0xffffffffffffffff)

  0pad0 = (00077777) (77777   )

  0|atw     |     atw|  3|3  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  1|nsl     |     nsl|  3|3  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  2|attila  |  attila|  6|6  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  3|icsa    |    icsa|  4|4  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  4|alex    |    alex|  4|4  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  5|ktye    |    ktye|  4|4  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|
  6|kelas   |   kelas|  5|5  |          0xdeadbeef|0x04 |  k|k  |%|kparc|%|

  (empty arglist) = (ok)

  PFCH=1 printf('~') = (~)

  PFMX=0 excess arguments (1234567891011) = (1234567891011) (unbound for gcc/clang builds)

  argc overflow (one)=(one) (two)=(two) (three)=((null)) (four)=((null))

  nesting+retval: s=(i uncover the soul-destroying abhorrence) p=(0xcafebabe)=(3405691582) c=(K) eot=(0x04) n=(108) //:~

$
```

## faq

> what's the idea?

`pf(f,a...)` is a variadic macro which is used to initialize an anonymous
array of uint64's (of maximal length `PFMX` in case of `tcc`) with the contents of `a...` on
stack, and count its elements. array pointer and its element count are both passed to
`txpf()`, which `txpf()` downcasts longs according to the format specification, and prints them.

> what's the catch?

since arguments of any type are upcast to `unsigned long long` and passed exclusively on stack,
`pf()` wins portability in return for much less efficient use of resources compared to ABI-specific
`va_arg` routines. use those if you can. too many positional arguments will smash your stack, therefore:

a) if you are using `tcc`, choose `VMX` wisely.

b) if you're absolutely sure your pointers are 32-bit and you don't need to print
longs, redeclare `union` to use `UI` instead of `UJ`. if you're not sure what
any of this means, `pf()` is not for you. caveat emptor.

c) protect your stack.

> will it work on my operating system?

yes, but if you're lucky to target some semblance of an operating system to write
home about, you should probably use local `printf(3)`.

> why two headers?

the code in `pf.h` is subject to terms of bsd-2-clause, whereas `_.h` is
placed in public domain by the [regents of kparc](https://github.com/kparc).
if you so desire, you are welcome to amalgamize `_.h` and `pf.h` into a single header,
at a small expense of not being able to easily receive updates from this repository
in the future.

> is there anything I can tweak except `PFMX`?

not really. there is one extra configuration parameter which breaks posix
compatibility and is therefore not documented. there are hardly any
user-serviceable parts in the code, so please let us now if you think
you have found a bug, or have a great feature suggestion in mind.

## more faq

> why the dummy `union{UJ}`?

to trick `gcc` into thinking that ptr-to-ULL coercion is not an error (`tcc`
and `clang` are aware of that).

> why `int-conversion` warning is suppressed?

it is only suppressed in `pf.h`, not in your code. ptr-to-ULL warning is
safe to be ignored, no lossy casts are taking place.

> why this software is written this way?

because this way the software is faster to write, easier to read, and safer to run.
your mileage may vary.

`//:~`
