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
