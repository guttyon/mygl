CXXFLAGS = -Wall -g

OBJS = util.o
OBJS += main.o
OBJS += sdluser.o
OBJS += mygl.o


mygl:$(OBJS)
	g++ -g -lSDL $(OBJS) -o $@

# 比較用
mygl2:$(OBJS)
	g++ -g -lSDL $(OBJS) -o $@

clean:
	rm mygl $(OBJS)

test: mygl
	./$<

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:mygl.git 
	git push assembla master
