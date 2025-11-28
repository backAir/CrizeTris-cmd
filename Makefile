.PHONY: make build run test


make:
	@make build
	@make run

debug:
	@make build-debug
	gdb build/QualityTris

build:
	gcc ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/QualityTris


small:
	gcc -Os -s -flto -fdata-sections -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fvisibility=hidden -Wl,--strip-all ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/QualityTrisSmol1

smalldeep:
	gcc -Os -s -mwindows -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-stack-protector ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/QualityTrisSmol2.exe

build-debug:
	gcc -g3 -Og -Wall -fno-omit-frame-pointer ./main.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build/QualityTris

run:
	cmd /c build\QualityTris.exe

test:
	gcc -DTEST ./main.c ./test.c ./game.c ./external.c ./CrizeTris/CrizeTris.c ./CrizeTris/Pieces.c ./CrizeTris/GameUtils.c ./CrizeTris/KickTable.c -o build\test
	cmd /c build\test.exe
