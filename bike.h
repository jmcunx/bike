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

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_INT_NULL   ((int) '\0')
#define JLIB2_CHAR_NULL  ((char) '\0')
#define JLIB2_UCHAR_NULL ((unsigned char) '\0' )
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif
#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

#ifndef PI
#define PI ((double)  3.14159265358979323846264338)
#endif

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

#ifdef NO_JLIB
struct s_j2_datetime
{
  int month;    /* Month,           01 --12             */
  int dd;       /* Day of Month,    01 -- [28|29|30|31] */
  int yy;       /* two digit year,  00 -- 99            */
  int yyyy;     /* four digit year, 0000 -- 9999        */
  int hh;       /* Hour of the day, 00 -- 23            */
  int minutes;  /* minutes,         00 -- 59            */
  int ss;       /* seconds,         00 -- 59            */
  int mil;      /* milliseconds,    000 -- 999          */
  int tm_isdst; /* Daylight ?  0 = no, > 0 = yes        */
} ;
#endif /* NO_JLIB */

/*** messages ***/
#ifdef NO_JLIB
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_FRONT         'F'  /* Front Sprockets                    */
#define ARG_HELP       'h'  /* Show Help                      */
#define ARG_HELP_LONG  'H'  /* Show Long Help                 */
#define ARG_OUT           'o'  /* Output File                        */
#define ARG_REAR          'R'  /* Rear  Sprockets                    */
#define ARG_TITLE         't'  /* Report Title                       */
#define ARG_USE_PI        'p'  /* use PI in calculations             */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define ARG_WHEEL         'w'  /* Wheel Size                         */
#define LIT_INFO_01  "\tBuild: %s %s\n"
#define LIT_INFO_END "******* End of Report *******"
#define LIT_REV           "Revision"
#define MSG_ERR_E000    "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002    "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E008    "ERROR E008: '%s' is an invalid value for %c%c, must be numeric\n"
#define MSG_ERR_E022    "ERROR E022: Required Argument '%c%c' missing\n"
#define MSG_ERR_E025    "ERROR E025: File %s cannot be created, already exists\n"
#define MSG_ERR_E029    "ERROR E029: Sprocket list empty\n"
#define MSG_ERR_E030    "ERROR E030: Sprocket '%s' not numeric\n"
#define MSG_ERR_E031    "ERROR E031: Too many Sprockets specified, max: %d\n"
#define MSG_ERR_E032    "ERROR E032: Sprocket value of %d must be greater than %d\n"
#define SWITCH_CHAR   '-'
#define USG_MSG_ARG_ERR                  "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE                "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_FRONT                "\t%c%c n,n,n,...,n\t: Front Sprocket tooth count, comma seperated\n"
#define USG_MSG_ARG_HELP                 "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_HELP_LONG            "\t%c%c\t\t: Show detail help and exit\n"
#define USG_MSG_ARG_OUT                  "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define USG_MSG_ARG_REAR                 "\t%c%c n,n,n,...,n\t: Rear  Sprocket tooth count, comma seperated\n"
#define USG_MSG_ARG_TITLE                "\t%c%c \"string\"\t: Optional Title\n"
#define USG_MSG_ARG_USE_PI               "\t%c%c\t\t: use PI in calculations, default is not to use PI\n"
#define USG_MSG_ARG_VERBOSE_3            "\t%c%c\t\t: Show detail Gear Inch Information\n"
#define USG_MSG_ARG_VERSION              "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_ARG_WHEEL                "\t%c%c n\t\t: Wheel Size, if < 100 assume inches, > 100 assume cm\n"
#define USG_MSG_OPTIONS                  "Options\n"
#define USG_MSG_USAGE_4                  "usage:\t%s OPTIONS\n"
#endif /* NO_JLIB */

/*** prototypes ***/
void init(struct s_work *, int, char **);
void leave_prog(int, int, struct s_work *);
void bike_write_report(struct s_work *);
double bike_gear_inch(int, int, double);
#ifdef JHELP_LONG
int show_help_long(FILE *);
#endif

#ifdef NO_JLIB
int j2_f_exist(char *file_name);
int j2_is_numr(char *s);
void j2_today(struct s_j2_datetime *dt);
#endif /* NO_JLIB */

#endif /* REV_BIKE_H */
