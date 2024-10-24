/* pow.c */

#include <vacuumms/std.h>
#include <vacuumms/rng2.h>
#include <vacuumms/param.h>
#include <errno.h>
#include <math.h>

// In:  x, out: x^y 

int main(int argc, char* argv[])
{
  double x, y, z, d;
  char line[80];
  char *xs;

  y = strtod(argv[1], NULL);

  while (TRUE)
  {
    fgets(line, 80, stdin);
    if (feof(stdin)) break;

    xs = strtok(line, "\n");

    x = strtod(xs, NULL);

    printf("%lf\n", pow(x, y));
  }
}

