OBJS=mygl.o

mygl:$(OBJS)
	g++ -lSDL $< -o $@

test: mygl
	./$<

gitpush:
	@# ファイルはBOMの無い、UTF-8Nで保存すること。
	@# git remote add assembla git@git.assembla.com:guttyon.git 
	git push assembla master
