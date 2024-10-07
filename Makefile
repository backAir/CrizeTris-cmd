.PHONY: make build run test


make:
	@make build
	@make run

debug:
	@make build-debug
	@make run

build:
	gcc ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/CrizeTris

build-debug:
	gcc -g ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/CrizeTris

run:
	cmd /c build\CrizeTris.exe

test:
	gcc -DTEST ./main.c ./test.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build\test
	cmd /c build\test.exe
