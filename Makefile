run:
	gcc ./main.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o build/CrizeTris
	cmd /c build\CrizeTris.exe

test:
	gcc -DTEST ./main.c ./test.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o build/test
	cmd /c build\test.exe
