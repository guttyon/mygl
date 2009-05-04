CXXFLAGS = -Wall -g

OBJS = mygl.o


mygl:$(OBJS)
	g++ -g -lSDL $< -o $@

# 比較用
mygl2:$(OBJS)
	g++ -g -lSDL $< -o $@

clean:
	rm mygl $(OBJS)

test: mygl
	./$<

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:mygl.git 
	git push assembla master
