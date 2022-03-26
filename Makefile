# for bike
#
# Copyright (c) 2022 2023
#     John McCue <jmccue@jmcunx.com>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

SHELL=/bin/sh

#--- where to install
PRODUCTION=$(DESTDIR)/bin
PRODPMAN=$(DESTDIR)/man
PRODMAN=$(PRODPMAN)/man1
INCJLIB2=-I$(DESTDIR)/include  # may need to be changed for j_lib2.h location

#>>>>>>> select desired OS
#--- Linux 64 bit
WALL=-Wall -m64
ETAGS=etags
GZ=.gz
NROFF=cat
OSDESC != uname -smr
LIBJLIB2=-L$(DESTDIR)/lib64

#--- Linux 32 bit
#WALL=-Wall
#ETAGS=etags
#GZ=.gz
#NROFF=cat
#OSDESC != uname -smr
#LIBJLIB2=-L$(DESTDIR)/lib

#--- BSD 64 bit
#WALL=-Wall -m64
#ETAGS=etags
#GZ=.gz
#NROFF=cat
#OSDESC != uname -smr
#LIBJLIB2=-L$(DESTDIR)/lib

#--- BSD 32 bit
#WALL=-Wall
#ETAGS=etags
#GZ=.gz
#NROFF=cat
#OSDESC != uname -smr
#LIBJLIB2=-L$(DESTDIR)/lib

#--- AIX specific
#WALL=-bnoquiet
#ETAGS=true
#GZ=
#NROFF=nroff -man
#OSDESC = `uname -a`
#LIBJLIB2=-L$(DESTDIR)/lib

#--- should not have to change these
NROFF=nroff -man
CC=cc
CHMOD=chmod
CP=cp
CTAGS=ctags
ECHO=echo
GZIP=gzip
LINK=cc
LIBS=-lj_lib2
MV=mv
RM=rm -f
STRIP=strip
MKDIR=mkdir

EXE=
OBJ=.o
CFLAGS=-c $(WALL) $(INCJLIB2) -DOSTYPE="\"$(OSDESC)\""
LFLAGS=$(WALL) $(LIBJLIB2) -o bike $(LIBS)

ALL_OBJ=bike$(OBJ) bike1$(OBJ) bike2$(OBJ) bikel$(OBJ)
ALL_C=bike.c bike2.c bike1.c bikel.c
ALL_H=bike.h

#
# do the work
#
all:	ckenv tags $(ALL_OBJ) bike.1$(GZ)
	$(LINK) $(LFLAGS) $(ALL_OBJ) $(LIBS)

ckenv:
	echo checking Variable DESTDIR
	test $(DESTDIR)

tags:	$(ALL_H) $(ALL_C)
	-$(CTAGS) $(ALL_H) $(ALL_C)
	-$(ETAGS) $(ALL_H) $(ALL_C)

bike.1.gz:	bike.1
	-$(RM) bike.1.gz
	$(GZIP) bike.1

bike.1:	bike.man
	$(NROFF) bike.man > bike.1

#--- AIX install(1) is odd compared to BSDs and Linux
install:	all
	-$(MKDIR) $(PRODUCTION)
	-$(MKDIR) $(PRODPMAN)
	-$(MKDIR) $(PRODMAN)
	-$(CHMOD) 755 $(PRODUCTION)
	-$(CHMOD) 755 $(PRODPMAN)
	-$(CHMOD) 755 $(PRODMAN)
	-$(STRIP) bike
	$(CP) bike $(PRODUCTION)/bike
	$(CHMOD) 755 $(PRODUCTION)/bike
	$(CP) bike.1$(GZ) $(PRODMAN)/bike.1$(GZ)
	$(CHMOD) 644 $(PRODMAN)/bike.1$(GZ)

uninstall:	all
	-$(RM) $(PRODUCTION)/bike
	-$(RM) $(PRODMAN)/bike.1.gz
	-$(RM) $(PRODMAN)/bike.1

clean:
	-$(RM) *$(OBJ)
	-$(RM) bike$(EXE)
	-$(RM) bike.1
	-$(RM) bike.1.gz
	-$(RM) TAGS
	-$(RM) tags
	-$(RM) core
	-$(RM) *.core
	-$(RM) a.out
	-$(RM) *.pdb
	-$(RM) *.ilk
	-$(RM) *.bak

### END

