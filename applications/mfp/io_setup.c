/* io_setup.c */

#include <time.h>
#include <string.h>

#include <vacuumms/std.h>
#include <vacuumms/rng.h>
#include <vacuumms/io_setup.h>

extern int number_of_molecules;
extern double x[], y[], z[];
extern double sigma[], epsilon[];

extern int verbose;

void readConfiguration()
{
  char line[80];
  char *xs, *ys, *zs;
  char *sigmas, *epsilons;

  number_of_molecules = 0;

  while (1)
  {
    fgets(line, 80, stdin);
    if (feof(stdin)) break;

    xs = strtok(line, "\t");
    ys = strtok(NULL, "\t");
    zs = strtok(NULL, "\t");
    sigmas = strtok(NULL, "\t");
    epsilons = strtok(NULL, "\n");

    x[number_of_molecules] = strtod(xs, NULL);
    y[number_of_molecules] = strtod(ys, NULL);
    z[number_of_molecules] = strtod(zs, NULL);
    sigma[number_of_molecules] = strtod(sigmas, NULL);
    epsilon[number_of_molecules] = strtod(epsilons, NULL);
    number_of_molecules++;
  }
 
  V printf("%d lines read.\n", number_of_molecules);
  fclose(stdin);
}
