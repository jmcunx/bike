/*
 * Copyright (c) 1975,1993 ...  2021 2022
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
#ifndef REV_BIKE_H

#define REV_BIKE_H "INCLUDED"

#define MAX_SPROCKETS 50

#ifndef NULL
#  define NULL '\0'
#endif
#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE 1
#endif

#define JHELP_LONG 1

struct s_file
{
  FILE *fp;
  int is_open;
} ;
struct s_sprocket
{
  int max;
  int sprockets[(MAX_SPROCKETS + 1)];
} ;

struct s_work
{
  int force;
  int verbose;
  int use_pi;
  int wheel_size;
  double diameter_inch;
  double diameter_cm;
  double circumference_inch;
  double circumference_cm;
  char *title;
  struct s_file out;
  struct s_file err;
  struct s_sprocket front;
  struct s_sprocket rear;
} ;


/*** prototypes ***/
void init(struct s_work *, int, char **);
void leave_prog(int, int, struct s_work *);
void bike_write_report(struct s_work *);
double bike_gear_inch(int, int, double);
#ifdef JHELP_LONG
int show_help_long(FILE *);
#endif

#endif /* REV_BIKE_H */
