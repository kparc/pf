#include"pf.h"
int
main(int argc, char **argv)
{
    printf("\n pf %d.%02d.%02d %s\n\n", PFV1, PFV2, PFV3, PFL);
    printf(" hey, %s\n\n", "guacamole!");

    printf(" %+s %#s %++s %^s\n\n", "red", "green", "blue", "amber");

    printf(" %p //:~\n\n", (void*)0x04);
    exit(0);
}
//:~
