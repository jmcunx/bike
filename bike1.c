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
#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#ifdef MSDOSTYPE
#include <getopt.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>
#include "bike.h"

#define REV_BIKE1 "$Id: bike1.c,v 3.1 2021/12/20 19:13:18 jmccue Exp $"
extern char *rev_bike_c;
extern char *rev_bike2_c;

#define USG_MSG_BIKE_1 "\tCalculate Bicycle Gear Inches\n"

/*
 * show_rev() -- displays Version Information
 */
int show_rev(struct s_work *w)

{

  fprintf(w->err.fp,"%s:\n", LIT_REV);
  fprintf(w->err.fp,"\t%s\n", rev_bike_c);
  fprintf(w->err.fp,"\t%s\n", REV_BIKE1);
  fprintf(w->err.fp,"\t%s\n", rev_bike2_c);
  fprintf(w->err.fp,"\t%s\n", REV_BIKE_H);

#ifdef J_LIB2M_H
  fprintf(w->err.fp, "\t%s\n", J_LIB2M_H);
#endif
#ifdef J_LIB2_H
  fprintf(w->err.fp, "\t%s\n", J_LIB2_H);
  fprintf(w->err.fp, "\t%s %s\n", LIT_INFO_02, j2_get_build());
#endif
#ifdef OSTYPE
  fprintf(w->err.fp,"\t%s\n", OSTYPE);
#endif
#ifdef PRETTY_NAME
  fprintf(w->err.fp,"\t%s\n",PRETTY_NAME);
#endif  /* PRETTY_NAME  */

  fprintf(w->err.fp, LIT_INFO_01, __DATE__, __TIME__);

  exit(EXIT_FAILURE);

} /* show_rev() */

/*
 * show_help() -- displays show_help info and exits
 */
int show_help(struct s_work *w)

{

  fprintf(w->err.fp, USG_MSG_USAGE_4, "bike");
  fprintf(w->err.fp, USG_MSG_BIKE_1);
  fprintf(w->err.fp, USG_MSG_OPTIONS);
  fprintf(w->err.fp, USG_MSG_ARG_ERR,       SWITCH_CHAR, ARG_ERR);
  fprintf(w->err.fp, USG_MSG_ARG_FRONT,     SWITCH_CHAR, ARG_FRONT);
  fprintf(w->out.fp, USG_MSG_ARG_FORCE,     SWITCH_CHAR, ARG_FORCE);

#ifdef JHELP_LONG
  fprintf(w->out.fp, USG_MSG_ARG_HELP_LONG, SWITCH_CHAR, ARG_HELP_LONG);
#endif

  fprintf(w->err.fp, USG_MSG_ARG_HELP,      SWITCH_CHAR, ARG_HELP);
  fprintf(w->err.fp, USG_MSG_ARG_OUT,       SWITCH_CHAR, ARG_OUT);
  fprintf(w->err.fp, USG_MSG_ARG_USE_PI,    SWITCH_CHAR, ARG_USE_PI);
  fprintf(w->err.fp, USG_MSG_ARG_REAR,      SWITCH_CHAR, ARG_REAR);
  fprintf(w->err.fp, USG_MSG_ARG_TITLE,     SWITCH_CHAR, ARG_TITLE);
  fprintf(w->err.fp, USG_MSG_ARG_VERBOSE_3, SWITCH_CHAR, ARG_VERBOSE);
  fprintf(w->err.fp, USG_MSG_ARG_VERSION,   SWITCH_CHAR, ARG_VERSION);
  fprintf(w->err.fp, USG_MSG_ARG_WHEEL,     SWITCH_CHAR, ARG_WHEEL);

  return(EXIT_FAILURE);

} /* show_help() */

/*
 * init_file() -- initialize file area
 */
void init_file(struct s_file *f)
{

  f->fp      = (FILE *) NULL;
  f->is_open = (int) FALSE;

} /* init_file() */

/*
 * leave_prog() -- initialize file area
 */
void leave_prog(int rmode, int rc, struct s_work *w)
{


  if (rmode == (int) TRUE)
    fprintf(w->err.fp, MSG_ERR_E000, "bike", SWITCH_CHAR, ARG_HELP);

  if (w->out.is_open)
    {
      fclose(w->out.fp);
      init_file(&(w->out));
      w->out.fp = stdout;
    }
  if (w->err.is_open)
    {
      fclose(w->err.fp);
      init_file(&(w->err));
      w->err.fp = stderr;
    }

  exit(rc);

} /* leave_prog() */

/*
 * open_out() -- save the file anem and check status
 */
int open_out(FILE *wfp, struct s_file *f, char *fname, int force)

{

  int errsave;

  if (fname == (char *) NULL)
    return((int) TRUE);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return((int) FALSE);
	}
    }


  f->fp = fopen(fname, "w");
  errsave = errno;
  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return((int) FALSE);
    }

  /*** success, save file name ***/
  return((int) TRUE);
} /* open_out() */

/*
 * get_args() -- load arguments
 */
void get_args(struct s_work *w, int argc, char **argv, char **front, char **rear)
{
  char ckarg[80];
  int opt;
  char *out_file   = (char *) NULL;
  char *err_file   = (char *) NULL;
  char *a_rear     = (char *) NULL;
  char *a_front    = (char *) NULL;
  char *a_wheel    = (char *) NULL;
  char *a_title    = (char *) NULL;
  int display_rev  = (int) FALSE;
  int display_help = (int) FALSE;
  int ok;
#ifdef JHELP_LONG
  int display_help_long = (int) FALSE;
#endif

#ifdef JHELP_LONG
  sprintf(ckarg, "%c:%c:%c:%c:%c:%c:%c%c%c%c%c%c", 
	  ARG_TITLE,
	  ARG_WHEEL,
	  ARG_FRONT,
	  ARG_REAR,
	  ARG_ERR, 
	  ARG_OUT, 
	  ARG_HELP, 
	  ARG_USE_PI,
	  ARG_VERBOSE,
	  ARG_VERSION,
	  ARG_FORCE,
	  ARG_HELP_LONG);
#else
  sprintf(ckarg, "%c:%c:%c:%c:%c:%c:%c%c%c%c%c", 
	  ARG_TITLE,
	  ARG_WHEEL,
	  ARG_FRONT,
	  ARG_REAR,
	  ARG_ERR, 
	  ARG_OUT, 
	  ARG_HELP, 
	  ARG_USE_PI,
	  ARG_VERBOSE,
	  ARG_VERSION,
	  ARG_FORCE);
#endif

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_VERBOSE:
	    w->verbose = (int) TRUE;
	    break;
	  case ARG_USE_PI:
	    w->use_pi = (int) TRUE;
	    break;
	  case ARG_TITLE:
	    a_title = optarg;
	    break;
	  case ARG_FRONT:
	    a_front = optarg;
	    break;
	  case ARG_REAR:
	    a_rear  = optarg;
	    break;
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_WHEEL:
	    a_wheel = optarg;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  case ARG_OUT:
	    out_file = optarg;
	    break;
	  case ARG_ERR:
	    err_file = optarg;
	    break;
#ifdef JHELP_LONG
	  case ARG_HELP_LONG:
	    display_help_long = (int) TRUE;
	    break;
#endif
	  default:
	    leave_prog((int) TRUE, EXIT_FAILURE, w);
	    break;
	}
    }

  /*** open out files ***/
  ok = open_out(stderr, &(w->err), err_file, w->force);
  if ( ok == (int) TRUE)
    ok = open_out(w->err.fp, &(w->out), out_file, w->force);
  if (ok == (int) FALSE)
    leave_prog((int) TRUE, EXIT_FAILURE, w);

  /*** show help/rev ? ***/
  ok = EXIT_SUCCESS;
#ifdef JHELP_LONG
  if (display_help_long)
    ok = show_help_long(w->out.fp);
#endif
  if (display_help)
    ok = show_help(w);
  if (display_rev)
    ok = show_rev(w);
  if (ok != EXIT_SUCCESS)
    leave_prog((int) FALSE, EXIT_FAILURE, w);

  /*** verify / save arguments ***/
  if (a_title != (char *) NULL)
    w->title = strdup(a_title);
  if (a_rear == (char *) NULL)
    {
      fprintf(w->err.fp, MSG_ERR_E022, SWITCH_CHAR, ARG_REAR);
      leave_prog((int) TRUE, EXIT_FAILURE, w);
    }
  else
    (*rear) = strdup(a_rear);
  if (a_front == (char *) NULL)
    {
      fprintf(w->err.fp, MSG_ERR_E022, SWITCH_CHAR, ARG_FRONT);
      leave_prog((int) TRUE, EXIT_FAILURE, w);
    }
  else
    (*front) = strdup(a_front);
  if (a_wheel == (char *) NULL)
    {
      fprintf(w->err.fp, MSG_ERR_E022, SWITCH_CHAR, ARG_WHEEL);
      leave_prog((int) TRUE, EXIT_FAILURE, w);
    }
  else
    {
      if ( j2_is_numr(a_wheel) )
	w->wheel_size = atoi(a_wheel);
      else
	{
	  fprintf(w->err.fp, MSG_ERR_E008, a_wheel, SWITCH_CHAR, ARG_WHEEL);
	  leave_prog((int) TRUE, EXIT_FAILURE, w);
	}
    }

} /* get_args() */

/*
 * init_w() -- initialize work area
 */
void init_w(struct s_work *w)
{

  int i, max;

  w->force           = (int) FALSE;
  w->title           = (char *) NULL;
  w->wheel_size      = 0;
  w->diameter_cm     = (double) 0;
  w->diameter_inch   = (double) 0;
  w->use_pi          = (int) FALSE;  /* PI seems inaccurate */
  w->verbose         = (int) FALSE;

  /*** initialize sprocket tables ***/
  max = MAX_SPROCKETS + 1;
  w->front.max = 0;
  w->rear.max  = 0;
  for (i = 0; i < max; i++)
    {
      w->front.sprockets[i] = 0;
      w->rear.sprockets[i]  = 0;
    }

  init_file(&(w->err));
  init_file(&(w->out));

  w->out.fp = stdout;
  w->err.fp = stderr;

} /* init_w() */

/*
 * sort_sprockets() -- Sort mapov table
 */
void sort_sprockets(int reverse, int *sprocket)
{

  int last, sorted, i, j;
  int max;
  int m;
  int test_result;

  max = MAX_SPROCKETS + 1;

  for (last = 0, i = 0; i < max; i++)
    {
      if (sprocket[i] < 1)
	break;
      last = i;
    }

  if (last < 2)
    return;

  sorted = (int) FALSE;
  
  while (sorted == (int) FALSE)
    {
      sorted = (int) TRUE;
      for (i = 0; i < last; i++)
	{
	  j = i + 1;
	  if (reverse)
	    test_result = sprocket[i] < sprocket[j];
	  else
	    test_result = sprocket[i] > sprocket[j];
	  if (test_result)
	    {
	      sorted = (int) FALSE;
	      m = sprocket[i];
	      sprocket[i] = sprocket[j];
	      sprocket[j] = m;
	    }
	}
      last--;
    }

} /* sort_sprockets() */

/*
 * load_sprockets() -- Sort mapov table
 */
int load_sprockets(struct s_sprocket *skt, char *s, struct s_work *w)
{
  char *use_s = (char *) NULL;
  char *token;
  int i, max;

  max = MAX_SPROCKETS - 1;

  if (s == (char *) NULL)
    {
      fprintf(w->err.fp, MSG_ERR_E029);
      return((int) FALSE);
    }

  /*** Duplicate and split on ',' ***/
  use_s = strdup(s);
  i     = 0;

  token = strtok(use_s, ",");
  while (token != (char *) NULL)
    {
      if (i <= max)
	{
	  if (j2_is_numr(token))
	    {
	      skt->sprockets[i] = atol(token);
	      (skt->max)++;
	      if (skt->sprockets[i] < 4)
		{
		  fprintf(w->err.fp, MSG_ERR_E032, skt->sprockets[i], 3);
		  return((int) FALSE);
		}
	    }
	  else
	    {
	      fprintf(w->err.fp, MSG_ERR_E030, token);
	      return((int) FALSE);
	    }
	}
      else
	{
	  fprintf(w->err.fp, MSG_ERR_E031, MAX_SPROCKETS);
	  return((int) FALSE);
	}
      i++;
      token = strtok((char *) NULL, ",");
    }

  /*** done ***/
  free(use_s);
  return((int) TRUE);

}  /* load_sprockets() */

/*
 * bike_gear_inch() -- Calculates Gear Inches
 */
double bike_gear_inch(int front, int rear, double diameter)
{

  return( ((double)front / (double)rear) * diameter);

} /* end bike_gear_inch() */


/*
 * init()
 */
void init(struct s_work *w, int argc, char **argv)

{

  char *front = (char *) NULL;
  char *rear = (char *) NULL;

  init_w(w);
  get_args(w, argc, argv, &front, &rear);

  /*** Load/split sprockets ***/
  if (! load_sprockets(&(w->front), front, w))
    leave_prog((int) TRUE, EXIT_FAILURE, w);
  if (! load_sprockets(&(w->rear), rear, w))
    leave_prog((int) TRUE, EXIT_FAILURE, w);
  sort_sprockets((int) FALSE, w->front.sprockets);
  sort_sprockets((int) TRUE,  w->rear.sprockets);   /* reverse sort */

  if (w->wheel_size < 5)
    {
      fprintf(w->err.fp, MSG_ERR_E032, w->wheel_size, 5);
      leave_prog((int) TRUE, EXIT_FAILURE, w);
    }

  /*** Save Data ***/
  if (w->wheel_size > 99)
    w->diameter_cm   = w->wheel_size;
  else
    w->diameter_inch = w->wheel_size;
  if (w->diameter_inch == 0)
    w->diameter_inch = w->diameter_cm * (double) .0393700787;
  if (w->diameter_cm == 0)
    w->diameter_cm = w->diameter_inch * (double) 2.54;
  w->circumference_cm   = w->diameter_cm   * PI;
  w->circumference_inch = w->diameter_inch * PI;

} /* init() */
