.PHONY: make build run test


make:
	@make build
	@make run

build:
	gcc ./main.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o build/CrizeTris

run:
	cmd /c build\CrizeTris.exe

test:
	gcc -DTEST ./main.c ./test.c ./game.c ./external.c ./CrizeTris.c ./Pieces.c -o build\test
	cmd /c build\test.exe
