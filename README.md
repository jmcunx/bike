## bike -- Calculate Bicycle Gear Ratios

bike(1) will generate a report showing Detail Bicycle
Gear Ratios.

I originally created this in 1975 using FORTRAN IV.
I then translated to c in 1993.

To build, execute build.sh to generate a Makefile from
Makefile.template on most BSD/Linux systems and IBM AIX.
For MS-DOS, it should be rather easy to create a Makefile.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, so I came up with my own method which
is a real hack.  But works for me.
