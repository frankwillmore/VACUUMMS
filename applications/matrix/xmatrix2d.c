/* xmatrix2d.c */

/* input:  .dst */
/* output:  singular value (average of all entries, based on diffusion in 3-D cubic matrix) */

#include <ftw_std.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

double the_matrix[256][256];
double input_vals[1000000];
int n_inputs = 0;
const double rand_step = 1.0/RAND_MAX;
double Dmax=0;
int n_attempts=1000;
int n_steps=10000;

int i,j;
int i_0,j_0;
int new_i,new_j;

int main(int argc, char *argv[])
{
  char line[80];
  char *xs;
  double D;
  int attempts, steps;
  int direction;
  double distance_sq=0,sum_distance_sq=0;
  double cross_value;

  while (TRUE)
  {
    fgets(line, 80, stdin);
    if (feof(stdin)) break;

    xs = strtok(line, "\n");
    D=strtod(xs, NULL);
    if (D>Dmax) Dmax=D;
    input_vals[n_inputs++] = D;
  }
//printf("%d\n", n_inputs);

  for(i=0;i<256;i++)
  for(j=0;j<256;j++)
    the_matrix[i][j] = input_vals[(int)(rand()*rand_step*n_inputs)];

  // now start the insertions and wanderers
  for (attempts=0;attempts<n_attempts;attempts++)
  {
    // pick an insertion point
    new_i=i=i_0=65536+(rand()&255);
    new_j=j=j_0=65536+(rand()&255);

    for (steps=0;steps<n_steps;steps++)
    {
      // pick a direction
      direction=floor(rand()*rand_step*4);
      switch (direction)
      {
        case 0:
          new_i = i-1;
          break;
        case 1:
          new_i = i+1;
          break;
        case 2:
          new_j = j-1;
          break;
        case 3:
          new_j = j+1;
          break;
        default:
          printf("direction=%d, wtf?\n",direction);
          exit(0);
      }

      // accept/reject move
      cross_value =2 * the_matrix[i&255][j&255] * the_matrix[new_i&255][new_j&255] 
                  / (the_matrix[i&255][j&255] + the_matrix[new_i&255][new_j&255]); 

//printf("cross:  %lf\tmax:  %lf \n", cross_value, Dmax);

      if ((rand()*rand_step*Dmax)<cross_value) { i=new_i; j=new_j; }

//printf("%d\t%d\t%d\n",i,j,k);
    }
 
    // see how far we've traveled...
    distance_sq=(i-i_0)*(i-i_0)+(j-j_0)*(j-j_0);
    // more code here to keep track of set of distances...
//printf("%lf\n", distance_sq);
    sum_distance_sq += distance_sq;
  }

  // code to translate the set of distances back into a diffusion coefficeinet
printf("%12.24lf\n", Dmax*sum_distance_sq/(n_attempts*n_steps));

}
