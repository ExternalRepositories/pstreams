# $Id: Makefile,v 1.4.2.2 2002/01/27 13:28:39 redi Exp $
# PStreams Makefile
# Copyright (C) Jonathan Wakely
#
# This file is part of PStreams.
# 
# PStreams is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# PStreams is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with PStreams; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

CXX=g++3

CFLAGS=-Wall -Wpointer-arith -Wcast-qual -Wcast-align -Wredundant-decls
CXXFLAGS=$(CFLAGS) -Woverloaded-virtual -Wenum-clash -Wtemplate-debugging

SOURCES = pstream.h
DOCS = pstreams.html
EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO 

DISTFILES= $(SOURCES) $(DOCS) $(EXTRA_DIST)

all: test distro

test: test_pstreams test_minimum
	@./test_minimum >/dev/null
	@./test_pstreams >/dev/null || echo "TEST EXITED WITH STATUS $?"

test_pstreams: test_pstreams.cc pstream.h
	$(CXX) $(CXXFLAGS) -g3 -o $@ $<

test_minimum: test_minimum.cc pstream.h
	$(CXX) $(CXXFLAGS) -o $@ $<

distro: docs pstreams.tar.gz

docs: pstream.h
	@doxygen Doxyfile

ChangeLog:
	@cvs2cl.pl

pstreams.tar.gz: $(DISTFILES)
	@tar czf $@ $^

TODO : pstream.h  pstreams.html test_pstreams.cc
	@grep -nH TODO $^ | sed -e 's@ *// *@@' > $@

.PHONY: TODO test distro ChangeLog docs

