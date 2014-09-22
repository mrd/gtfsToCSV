# gtfsToCSV - convert GTFS realtime vehicle locations to CSV
# Copyright (C) 2014 Matthew Danish
# Homepage - http://github.com/mrd/gtfsToCSV
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc
CFLAGS = $(shell pkg-config --cflags 'libprotobuf-c >= 1.0.0')
LIBS = $(shell pkg-config --libs 'libprotobuf-c >= 1.0.0')
LDFLAGS = $(LIBS)
PBUFCC = protoc-c

gtfsToCSV: gtfsToCSV.o gtfs-realtime.pb-c.o
gtfsToCSV.o: gtfs-realtime.pb-c.h gtfsToCSV.c
gtfs-realtime.pb-c.o: gtfs-realtime.pb-c.c

gtfs-realtime.pb-c.h gtfs-realtime.pb-c.c: gtfs-realtime.proto
	$(PBUFCC) --c_out=. $<

.PHONY: clean
clean:
	rm -f gtfsToCSV.o gtfs-realtime.pb-c.c gtfs-realtime.pb-c.h gtfs-realtime.pb-c.o gtfsToCSV
