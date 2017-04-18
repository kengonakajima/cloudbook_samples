UNAME=$(shell uname)

SNAPPYOBJS=moyai/snappy/snappy-sinksource.o moyai/snappy/snappy-c.o moyai/snappy/snappy.o


ifeq ($(UNAME),Darwin)
CFLAGS=-I./moyai/ -I/usr/local/Cellar/libuv/1.8.0/include/ -I./moyai/glfw-3.2/include #-g
UNTZDEPENDLIB=-framework AudioToolbox /usr/local/lib/libvorbis.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libogg.a
FREETYPELIB=moyai/freetype-2.4.10/objs/.libs/libfreetype.a
BZ2LIB=moyai/bzip2-1.0.6/libbz2.a
ZLIBLIB=moyai/zlib-1.2.7/libz.a
LIBFLAGS=moyai/libmoyaicl.a /usr/local/Cellar/libuv/1.8.0/lib/libuv.a $(UNTZDEPENDLIB) -framework Cocoa -framework IOKit -framework OpenGL -framework CoreFoundation ./moyai/glfw-3.2/src/libglfw3.a -framework CoreVideo -ljpeg -L/usr/local/lib $(SNAPPYOBJS) moyai/libftgl.a $(FREETYPELIB) $(BZ2LIB) $(ZLIBLIB) -framework OpenAL moyai/libalut.a
else
CFLAGS=-I./moyai/ -std=c++11 -g
JPEGLIB=moyai/jpeg-8d/libjpeg.a
LIBFLAGS=moyai/libmoyaicl.a -L/usr/local/lib $(ZLIBLIB) $(SNAPPYOBJS) -luv $(JPEGLIB)
endif


all : min dm rv duel bench scroll

min : min.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) min.cpp sample_common.cpp $(LIBFLAGS) -o min

dm : danmaku.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) danmaku.cpp sample_common.cpp $(LIBFLAGS) -o dm

rv : reversi.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) reversi.cpp sample_common.cpp $(LIBFLAGS) -o rv

duel : duel.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) duel.cpp sample_common.cpp $(LIBFLAGS) -o duel

bench : bench.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) bench.cpp sample_common.cpp $(LIBFLAGS) -o bench

scroll : scroll.cpp sample_common.cpp moyai/libmoyaicl.a
	g++ $(CFLAGS) scroll.cpp sample_common.cpp $(LIBFLAGS) -o scroll

clean :
	rm min dm rv duel bench scroll
	rm -rf *.dSYM
