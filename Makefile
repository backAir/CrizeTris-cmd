run:
	gcc ./main.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o ./build/CrizeTris
	build/CrizeTris

test:
	gcc -DTEST ./main.c ./test.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o ./build/test
	cmd /c build/test
