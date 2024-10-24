/* graphics.c */

#include <vacuumms/graphics.h>
#include <vacuumms/science.h>
#include <vacuumms/std.h>

extern int side_view;
extern int wsize_x, wsize_y, wsize_z;
extern int particle_scale;
extern double box_x, box_y, box_z;
extern int change_flag;
extern int graphics;
extern int number_of_molecules;
extern int bg_color;
extern int fg_color;
extern int min_color;
extern double x[], y[], z[];

Display *dpy;
Window window, window2;
GC context, context2;
XEvent event;
XGCValues gcvalues;
char *displayname="simulation";
char *display=NULL;
int *z_rank;
int *x_rank;

extern char *display_name_1;
extern char *display_name_2;

void checkForWindowEvent()
{
  int rstat;

  rstat = XCheckMaskEvent(dpy, ExposureMask | ButtonPressMask, &event);
  if (rstat&&(event.type == ButtonPress))
  {
    XFreeGC(dpy, context);
    XCloseDisplay(dpy);
    exit(0);
  }
}

void initializeDisplay()
{
  wsize_x = (int)(particle_scale * box_x);
  wsize_y = (int)(particle_scale * box_y);
  wsize_z = (int)(particle_scale * box_z);

  z_rank = (int*)malloc(sizeof(int) * number_of_molecules);
  x_rank = (int*)malloc(sizeof(int) * number_of_molecules);

  dpy = XOpenDisplay(display);
  if (dpy == NULL) printf("Can't open the display.\n");
 
  window = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, wsize_x, wsize_y, 0,0,0);
  XSelectInput(dpy, window, (StructureNotifyMask|ExposureMask|ButtonPressMask|ButtonReleaseMask));
  XMapWindow(dpy, window);
  context = XCreateGC(dpy, window, GCForeground | GCBackground, &gcvalues);
  XStoreName(dpy, window, display_name_1);
  XSetIconName(dpy, window, display_name_2);

  if (side_view)
  {
    window2 = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), wsize_x, 0, wsize_z, wsize_y, 0,0,0);
    XSelectInput(dpy, window2, (StructureNotifyMask|ExposureMask|ButtonPressMask|ButtonReleaseMask));
    XMapWindow(dpy, window2);
    context2 = XCreateGC(dpy, window2, GCForeground | GCBackground, &gcvalues);
    XStoreName(dpy, window2, display_name_2);
    XSetIconName(dpy, window2, display_name_2);
  }

  checkForWindowEvent();
}

void setChangeFlag()
{
  change_flag = TRUE;
}

void resetChangeFlag()
{
  change_flag = FALSE;
}

int changeFlagIsSet()
{
  return change_flag;
}

int graphicsModeEnabled()
{
  return graphics;
}

void drawGraphicalRepresentation()
{
  int xx, yy, zz;
  int i, j;
  int z_shade, x_shade;
  int temporary;

  if (!changeFlagIsSet()) return;

  for (i=0; i<number_of_molecules; i++) z_rank[i] = i;
  gcvalues.foreground = bg_color;
  XChangeGC(dpy, context, GCForeground, &gcvalues);
  XFillRectangle(dpy, window, context, 0, 0, wsize_x*2, wsize_y*2);

  /* order by z values */
  for (i=number_of_molecules; i>0; i--)
    for (j=0; j<i-1; j++)
      if (z[z_rank[j]] < z[z_rank[j+1]])
      {
        temporary = z_rank[j];
        z_rank[j] = z_rank[j+1];
        z_rank[j+1] = temporary;
      }

  for(i=0;i<number_of_molecules;i++)
  {
    xx=floor(x[z_rank[i]]*particle_scale - particle_scale/2);
    yy=floor(y[z_rank[i]]*particle_scale - particle_scale/2);
    z_shade = fg_color - floor((fg_color - min_color) * z[z_rank[i]] / box_z);
    gcvalues.foreground = z_shade;
    XChangeGC(dpy, context, GCForeground, &gcvalues);
    XFillArc(dpy, window, context, xx, yy, particle_scale, particle_scale, 0, 360*64);
    XFillArc(dpy, window, context, xx, yy + wsize_y, particle_scale, particle_scale, 0, 360*64);
    XFillArc(dpy, window, context, xx + wsize_x, yy, particle_scale, particle_scale, 0, 360*64);
    XFillArc(dpy, window, context, xx + wsize_x, yy + wsize_y, particle_scale, particle_scale, 0, 360*64);
  }

  if (side_view)
  {
    for (i=0; i<number_of_molecules; i++) x_rank[i] = i;
    gcvalues.foreground = bg_color;
    XChangeGC(dpy, context2, GCForeground, &gcvalues);
    XFillRectangle(dpy, window2, context2, -wsize_z/2, -wsize_y/2, wsize_z*2, wsize_y*2);

    /* order by x values */
    for (i=number_of_molecules; i>0; i--)
      for (j=0; j<i-1; j++)
        if (x[x_rank[j]] > x[x_rank[j+1]])
        {
          temporary = x_rank[j];
          x_rank[j] = x_rank[j+1];
          x_rank[j+1] = temporary;
        }

    for(i=0; i<number_of_molecules;i++)
    {
      x_shade = min_color + floor((fg_color - min_color) * x[x_rank[i]] / box_x);
      yy=floor(y[x_rank[i]]*particle_scale - particle_scale/2);
      zz=floor(z[x_rank[i]]*particle_scale - particle_scale/2);
      gcvalues.foreground = x_shade;
      XChangeGC(dpy, context2, GCForeground, &gcvalues);
      XFillArc(dpy, window2, context2, zz, yy, particle_scale, particle_scale, 0, 360*64);
      XFillArc(dpy, window2, context2, zz, yy + wsize_y, particle_scale, particle_scale, 0, 360*64);
      XFillArc(dpy, window2, context2, wsize_z + zz, yy, particle_scale, particle_scale, 0, 360*64);
      XFillArc(dpy, window2, context2, wsize_z + zz, yy + wsize_y, particle_scale, particle_scale, 0, 360*64);
    }
  }

  XFlush(dpy);
  checkForWindowEvent();
  resetChangeFlag();
}
