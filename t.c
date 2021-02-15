//! nolibc printf(3), copyright (c) 2020 regents of kparc, bsd-2-clause

#include <string.h> //!< strlen
#include <stdlib.h> //!< exit

//! configure pf for smoke test:

#ifdef __TINYC__
#define PFMX 10     //!< override default maxargs for tcc
#else
#define PFMX  0     //!< unlimited for gcc/clang
#endif
#define PFCH  1     //!< treat f < 128 as a char instead of segv (breaks posix compat)

#include "pf.h"     //!< include pf()

int
main(int argc, char**argv)
{

  printf("\n  pf %d.%02d.%02d %s %s\n", PFV1, PFV2, PFV3, PFA, PFL);

  const char *t_prc = "kparcxxxx";

  printf("\n  (%%)=(%%) (kparc)=(%.*s) (kparc)=(%.5s) ()=(%.s)\n", 5, t_prc, t_prc, t_prc);

  const unsigned long long umx = 18446744073709551615ULL;
  const long long jmx = 9223372036854775807LL;
  const unsigned int uimx = 4294967295U;
  const int imx = 2147483647;

  printf("\n"
    "   umx = (%20llu)\n"
    "   jmn = (%20lld)\n"
    "   jmx = (%20lld)\n"
    "   imn = (%20d)\n"
    "   imx = (%-20d)\n"
    "  uimx = (%-20u)\n",
    umx, -jmx, jmx, -imx, imx, uimx);

  printf("\n  pmx32 = (%19p)\n  pmx64 = (%19p)\n",uimx, umx);

  printf("\n  0pad0 = (%08d) (%-08d)\n", 77777, 77777);

  char *ti;
  char *t[] = {"atw", "nsl", "attila", "icsa", "alex", "ktye", "kelas"};

  int i = -1;
  int len;

  printf("\n");

  while(++i < 7)
  {
    ti = t[i];
    int len = strlen(ti);
    printf("%3d|%-8s|%8s|%3d|%-4d|%20p|%-5p|%3c|%-3c|%%|%s|%%|\n", i, ti, ti, len, len, 0xdeadbeef, 0x04, 'k', 'k', "kparc");
  }

  printf("\n  (empty arglist) = (ok)\n");

  //! feature flag tests:

  //! PFCH
  #ifndef NOPF
  printf("\n  PFCH=1 printf('~') = (");
  printf('~');
  printf(")\n");
  #else
  printf("\n  PFCH=1 printf('~') = *** SEGV ***\n");
  #endif//PF

  //! PFMX
  #ifndef NOPF
  #ifndef __TINYC__
  printf("\n  PFMX=%d excess arguments (%d%d%d%d%d%d%d%d%d%d%d) = (1234567891011)",
   PFMX, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  printf(" (unbound for gcc/clang builds)\n");
  #else
  printf("\n  PFMX=%d excess arguments (nop) = (tcc compile-time error)\n", PFMX);
  #endif//TINYC
  #else
  printf("\n  PFMX=%d excess arguments (%d%d%d%d%d%d%d%d%d%d%d) = (1234567891011)",
   PFMX, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  printf(" (unbound for printf(3))\n");
  #endif//PF

  //! argcount overflow
  #ifndef NOPF
  printf("\n  argc overflow (one)=(%s) (two)=(%s) (three)=(%s) (four)=(%s)\n", "one", "two");
  #else
  printf("\n  argc overflow (one)=(%s) (two)=(%s) (three)=(*** SEGV ***) four=(*** SEGV ***)=(%s)\n", "one", "two");
  #endif

  //! return value
  printf(" n=(%d) %s\n\n",
   printf("\n  nesting+retval: s=(%s %s %s) p=(%p)=(%u) c=(%c) eot=(%p)",
       "i uncover", "the soul-destroying", "abhorrence", 0xcafebabe, 0xcafebabe, 'K', 0x04),
  "//:~");

#ifdef NOLC
  exit(0);
#endif
  return 0;
}

//:~
