#include <iostream.h>
#include <math.h>

main(int argc, char **argv)
{
    double dist = 60.0;
    double alt = 10.0;

    int opt;
    extern char *optarg;

    while ((opt = getopt(argc,argv,"d:a:")) != -1)
      {
          if (opt == 'd')
            dist = atof(optarg);
          else if (opt == 'a')
            alt = atof(optarg);
      }

    double maxAngle = M_PI / 2.0;
    double inc = M_PI / 180.0;
    double angle = 0.0;

    while (angle < maxAngle)
      {
          if ((sin(angle) * dist) >= alt)
            {
                double d = cos(angle) * dist;
                angle *= 180.0 / M_PI;
                cout << "For max dist = " << dist << " & dalt = ";
                cout << alt << " the first angle = " << angle;
                cout << " d = " << d << endl;
                break;
            }
          else
            angle += inc;
      }
    exit(0);
}
