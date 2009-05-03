OBJS=mygl.o

mygl:$(OBJS)
	g++ -lSDL $< -o $@

test: mygl
	./$<
