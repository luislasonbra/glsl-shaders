LIBPATH = -L./
LDFLAGS = -lGL -lGLU -lglut -lpng

CFLAGS =
RM = rm
PROJ = glsl
# add here additional object files
OBJS = main.o

all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC) $(LIBPATH) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp

clean:
	$(RM) *.o
	$(RM) $(PROJ)
