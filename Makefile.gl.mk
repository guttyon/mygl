TARGET = ogltest
OBJS = ogltest.o

LIBS = -lGL -lGLU /usr/lib/libglut.so.3 -lSDL

CXXFLAGS = -Wall -g
LDFLAGS = -Wall -g $(LIBS)

ifeq ($(MAKECMDGOALS), prof)
CXXFLAGS += -pg
LDFLAGS += -pg
endif

DEPS = $(foreach O,$(OBJS),.$O.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@

%.o: %.cc
	@g++ -M $< > .$@.d
	@echo "" >> .$@.d
	@echo "cmd-$@ = g++ -c $(CXXFLAGS) $< -o $@" >> .$@.d
	g++ -c $(CXXFLAGS) $< -o $@

clean:
	-rm $(TARGET) $(OBJS) $(DEPS)

test: $(TARGET)
	./$<

prof: clean $(TARGET)
	./$(TARGET)
	gprof $(TARGET)

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:$(TARGET).git 
	git push assembla master

# forceをソースにすることで、そのターゲットは強制的に実行される。
force: ;

.PHONY : clean force

sinclude $(DEPS)
