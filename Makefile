CXXFLAGS = -Wall -g

OBJDIR=./obj

OBJS = util.o
OBJS += main.o
OBJS += sdluser.o
OBJS += mygl.o

DEPS = $(foreach O,$(OBJS),.$O.d)

all: mygl

m:
	echo $(DEPS)

mygl: $(OBJS)
	g++ -g -lSDL $(OBJS) -o $@

# 比較用
mygl2: $(OBJS)
	g++ -g -lSDL $(OBJS) -o $@

%.o: %.cc
	@g++ -M $< > .$@.d
	@echo "" >> .$@.d
	@echo "cmd-$@ = g++ -c $(CXXFLAGS) $< -o $@" >> .$@.d
	@make --no-print-directory -f Makefile.build $@

clean:
	-rm mygl $(OBJS) $(DEPS)

test: mygl
	./$<

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:mygl.git 
	git push assembla master

# forceをソースにすることで、そのターゲットは強制的に実行される。
force: ;

.PHONY : clean force

sinclude $(DEPS)
