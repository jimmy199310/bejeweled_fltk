objects = bejeweled.o game_board.o jewel_box.o jewel_logic.o

bejeweled : $(objects)
	g++ -o bejeweled $(objects) /usr/local/lib/libfltk_images.a /usr/local/lib/libfltk_png.a -lz /usr/local/lib/libfltk_jpeg.a /usr/local/lib/libfltk.a -framework Cocoa

bejeweled.o : bejeweled.cpp ui/game_board.h
	g++ -c bejeweled.cpp

game_board.o : ui/game_board.cpp ui/game_board.h
	g++ -c ui/game_board.cpp

jewel_box.o : ui/jewel_box.cpp ui/jewel_box.h
	g++ -c ui/jewel_box.cpp

jewel_logic.o : logic/jewel_logic.cpp logic/jewel_logic.h
	g++ -c logic/jewel_logic.cpp

.PHONY : install clean app

install :
	fltk-config --post bejeweled

clean :
	rm -rf bejeweled.app bejeweled $(objects)

app :
	fltk-config --post bejeweled
	rm bejeweled $(objects)