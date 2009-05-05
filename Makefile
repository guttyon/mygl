
OBJS = util.o
OBJS += main.o
OBJS += sdluser.o
OBJS += mygl.o
OBJS += drawline.o
OBJS += intersect.o
OBJS += hodge.o
OBJS += mat.o
OBJS += matstack.o

LIBS = -lSDL

CXXFLAGS = -Wall -g
LDFLAGS = -Wall -g $(LIBS)

ifeq ($(MAKECMDGOALS), prof)
CXXFLAGS += -pg
LDFLAGS += -pg
endif

DEPS = $(foreach O,$(OBJS),.$O.d)

all: mygl

mygl: $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@

# 比較用
mygl2: $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@

%.o: %.cc
	@g++ -M $< > .$@.d
	@echo "" >> .$@.d
	@echo "cmd-$@ = g++ -c $(CXXFLAGS) $< -o $@" >> .$@.d
	@make --no-print-directory -f Makefile.build $@
#	内部では$(obj)で参照できる。
#	ただし、そうすると、Makefile.buildの先頭でincludeし、.oをターゲットとしないといけなくなる。
#	@make --no-print-directory -f Makefile.build obj=$@

clean:
	-rm mygl $(OBJS) $(DEPS)

test: mygl
	./$<

prof: clean mygl
	./mygl
	gprof mygl

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:mygl.git 
	git push assembla master

# forceをソースにすることで、そのターゲットは強制的に実行される。
force: ;

.PHONY : clean force

sinclude $(DEPS)
