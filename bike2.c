/*
 * Copyright (c) 1975,1993 ...  2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "bike.h"

/*
 * bike_write_heading() -- Prints out heading area
 */
void bike_write_heading(struct s_work *w)
{
  struct s_j2_datetime d;

  j2_today(&d);

  fprintf(w->out.fp, "%02d/%02d/%04d %02d:%02d:%02d ",
          d.month, d.dd, d.yyyy,d.hh,d.minutes,d.ss);
  fprintf(w->out.fp, "      %s\n", (w->title == (char *) NULL ? " " : w->title));

  if (w->verbose)
    {
      fprintf(w->out.fp, "\n");
      fprintf(w->out.fp, "  Wheel Diameter      -- %8.4lf\" or %8.4lfcm\n",   w->diameter_inch,      w->diameter_cm);
      fprintf(w->out.fp, "  Wheel Circumference -- %8.4lf\" or %8.4lfcm\n\n", w->circumference_inch, w->circumference_cm);
      fprintf(w->out.fp, "      -Sprocket-  Gear Inches/ Feet/  Miles/  # Pedals ----MPH per Pedal RPM---\n");
      fprintf(w->out.fp, "Speed Front Rear  Inch Pedal   Pedal  Pedal   per Mile  70   80   90   100  110\n");
      fprintf(w->out.fp, "----- ----- ---- ----- ------- ----- -------- -------- ---- ---- ---- ---- ----\n");
    }

} /* bike_write_heading() */

/*
 * bike_write_chart_ul() -- Writes the Underline
 */
void bike_write_chart_ul(struct s_work *w)

{
  int i;
  
  fprintf(w->out.fp,"\n|----|");

  for (i=0 ; i < w->rear.max; i++)
    {
      fprintf(w->out.fp,"------|");
    }
  fprintf(w->out.fp,"\n");
  
} /* bike_write_chart_ul() */

/*
 * bike_write_chart() -- Creates the Chart portion of the report
 */
void bike_write_chart(struct s_work *w)

{
  int f, r;
  double ginch;

  if (w->verbose)  
    fprintf(w->out.fp,"\n");
  
  bike_write_chart_ul(w);

  /*** Show Heading ***/
  fprintf(w->out.fp,"|    |");
  for (r = (w->rear.max - 1) ; r >= 0; r--)
    fprintf(w->out.fp, " %3d  |", w->rear.sprockets[r]);
  bike_write_chart_ul(w);

  /*** show item ***/
  for (f = (w->front.max - 1); f >= 0; f--)
    {
      fprintf(w->out.fp, "|%3d |", w->front.sprockets[f]);
      for (r = (w->rear.max - 1) ; r >= 0; r--)
	{
	  ginch = bike_gear_inch(w->front.sprockets[f], w->rear.sprockets[r], w->diameter_inch);
	  fprintf(w->out.fp, "%5.1lf |", ginch);
	}
      bike_write_chart_ul(w);
    }

  fprintf(w->out.fp,"\n");
  
} /* bike_write_chart() */

/*
 * bike_write_report() -- Lists out table info in Report Format to stdout
 */
void bike_write_verbose(struct s_work *w)
{
  int f, r, speed, max;
  double gear_inch       = 0;
  double inch_per_pedal  = 0;
  double feet_per_pedal  = 0;
  double miles_per_pedal = 0;

  max = MAX_SPROCKETS - 1;

  speed = 0;

  for (f = 0;  f < max; f++)
    {
      if (w->front.sprockets[f] > 0)
	{
	  for (r = 0;  r < max; r++)
	    {
	      if (w->rear.sprockets[r] > 0)
		{
		  gear_inch  = bike_gear_inch(w->front.sprockets[f], w->rear.sprockets[r], w->diameter_inch);
		  if (w->use_pi)
		    inch_per_pedal = gear_inch * PI;
		  else
		    inch_per_pedal = (gear_inch / 60) * (double) 185;
		  feet_per_pedal   = inch_per_pedal / (double) 12;
		  miles_per_pedal  = feet_per_pedal / (double) 5280;
		  speed++;
		  fprintf(w->out.fp,"%4d   %3d  %3d  %5.1lf %7.2lf %5.2lf %8.6lf %8.3lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
			  speed,
			  w->front.sprockets[f],
			  w->rear.sprockets[r],
			  gear_inch,
			  inch_per_pedal,
			  feet_per_pedal,
			  miles_per_pedal,
			  ((double) 1 / miles_per_pedal),                  /* pedals_per_mile, */
		          (miles_per_pedal * (double) 70 * (double) 60),   /* mph_per_70rpm    */
			  (miles_per_pedal * (double) 80 * (double) 60),   /* mph_per_80rpm    */
			  (miles_per_pedal * (double) 90 * (double) 60),   /* mph_per_90rpm    */
			  (miles_per_pedal * (double) 100 * (double) 60),  /* mph_per_100rpm   */
			  (miles_per_pedal * (double) 110 * (double) 60)); /* mph_per_110rpm   */
		}
	    }
	}
    }

} /* bike_write_verbose() */

/*
 * bike_write_report() -- Lists out table info in Report Format to stdout
 */
void bike_write_report(struct s_work *w)

{
  
  bike_write_heading(w);

  if (w->verbose)
    bike_write_verbose(w);
 
  bike_write_chart(w);

  if (w->verbose)
    fprintf(w->out.fp, "\n%s\n", LIT_INFO_END);
  fprintf(w->out.fp, "\n");
  
}  /* bike_write_report() */

