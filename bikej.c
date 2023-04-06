/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
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

/*
 * bike_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef OpenBSD
#include <sys/timeb.h>
#endif
#include <time.h>

#ifdef __FreeBSD_version
#include <sys/time.h>
#define BSD_TODAY 1
#endif

#ifdef OpenBSD
#include <sys/time.h>
#define BSD_TODAY 1
#endif
#ifdef linux
#include <sys/time.h>
#endif

#ifdef __NetBSD_Version__
#include <sys/time.h>
#define BSD_TODAY 1
#endif

#include "bike.h"

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_is_numr() -- determines if all characters are numeric
 */
int j2_is_numr(char *s)

{
  if (s == (char *) NULL)
    return((int) FALSE); /* NULL pointer */

  for ( ; (*s) != JLIB2_CHAR_NULL; s++)
    {
      if ( ! isdigit((int)(*s)) )
	return(FALSE);
    }

  return(TRUE);

} /* j2_is_numr() */

/*
 * j2_today() -- get todays date
 */
void j2_today(struct s_j2_datetime *dt)

{

  struct tm     *str_tm;
  
#ifdef BSD_TODAY
  struct timeval tp;
  struct timezone tzp;
#else
#ifdef linux
  struct timeval tp;
  struct timezone tzp;
#else
  struct timeb  time_buf;
#endif
#endif

#ifdef BSD_TODAY
  gettimeofday(&tp, &tzp);
  str_tm = localtime(&(tp.tv_sec));
#else
#ifdef linux
  gettimeofday(&tp, &tzp);
  str_tm = localtime(&(tp.tv_sec));
#else
  ftime(&time_buf);
  str_tm = localtime(&(time_buf.time));
#endif
#endif
  
  (str_tm->tm_mon)++;
  
  dt->yyyy     = str_tm->tm_year + 1900;
  dt->month    = str_tm->tm_mon;
  dt->dd       = str_tm->tm_mday;
  dt->yy       = str_tm->tm_year;
  dt->hh       = str_tm->tm_hour;
  dt->minutes  = str_tm->tm_min;
  dt->ss       = str_tm->tm_sec;
  dt->tm_isdst = str_tm->tm_isdst;

#ifdef BSD_TODAY
  dt->mil      = (int) (tp.tv_usec / 1000);
#else
#ifdef linux
  dt->mil      = (int) (tp.tv_usec / 1000);
#else
  dt->mil      = time_buf.millitm;
#endif
#endif

} /* j2_today() */
