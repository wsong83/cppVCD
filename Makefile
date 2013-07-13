# Copyright (c) 2013-2013 Wei Song <songw@cs.man.ac.uk> 
#    Advanced Processor Technologies Group, School of Computer Science
#    University of Manchester, Manchester M13 9PL UK
#
#    This source code is free software; you can redistribute it
#    and/or modify it in source code form under the terms of the GNU
#    General Public License as published by the Free Software
#    Foundation; either version 2 of the License, or (at your option)
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# 03/07/2013   Wei Song
#
#

# global variables
#TARGET = RELEASE
TARGET = DEBUG

export BISON_EXE = bison
export CXX = ccache g++
export MC_FLAG = 
#export MC_FLAG = -j4

ifeq ($(TARGET),RELEASE)
	export CXXFLAGS = -std=c++0x -Wall -Wextra -o2 -DNDEBUG
else
	export CXXFLAGS = -std=c++0x -Wall -Wextra -g3
endif

export LINKFLAGS = -lgmpxx -lgmp 

SUBDIRS = src
BISONDIRS = src.bison

all: bison subdirs
	-mkdir bin
	-mv src/test bin/

bison: $(BISONDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(MC_FLAG) -C $@

src.bison:
	$(MAKE) bison -C src

clean:
	-for d in $(SUBDIRS); do $(MAKE) -C $$d clean; done
	-rm -fr bin

.PHONY: bison subdirs $(SUBDIRS) clean
