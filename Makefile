CC=clang

CFLAGS = -Wall -g -O0

TARGET=native-observer

OBJS=main.o \
observer.o \
meminfo.o \
smaps.o \
analyzer.o \
process.o \
mapping.o \
classifier.o \
dashboard.o \
renderer.o \
tracker.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c observer.h meminfo.h smaps.h analyzer.h process.h mapping.h dashboard.h tracker.h
	$(CC) $(CFLAGS) -c main.c

observer.o: observer.c observer.h
	$(CC) $(CFLAGS) -c observer.c

meminfo.o: meminfo.c meminfo.h
	$(CC) $(CFLAGS) -c meminfo.c

smaps.o: smaps.c smaps.h
	$(CC) $(CFLAGS) -c smaps.c

analyzer.o: analyzer.c analyzer.h process.h
	$(CC) $(CFLAGS) -c analyzer.c

process.o: process.c process.h observer.h meminfo.h smaps.h mapping.h
	$(CC) $(CFLAGS) -c process.c

mapping.o: mapping.c mapping.h
	$(CC) $(CFLAGS) -c mapping.c

classifier.o: classifier.c classifier.h mapping.h
	$(CC) $(CFLAGS) -c classifier.c

dashboard.o: dashboard.c dashboard.h classifier.h
	$(CC) $(CFLAGS) -c dashboard.c

renderer.o: renderer.c renderer.h
	$(CC) $(CFLAGS) -c renderer.c

tracker.o: tracker.c tracker.h mapping.h
	$(CC) $(CFLAGS) -c tracker.c

clean:
	rm -f $(OBJS) $(TARGET)
