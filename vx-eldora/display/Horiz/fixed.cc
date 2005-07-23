#include <iostream.h>
#include <stdlib.h>


main(int argc,char **argv)
{
    double d = atof(argv[1]);
    double dd = atof(argv[2]);

    int i = (int)(d * 65536.0);
    int ii = (int)(dd * 65536.0);

//    int j = (i >> 16) + (ii >> 16);

    int j = (i + (ii*ii)) >> 16;

    cout << j << endl;

    exit(0);
}
