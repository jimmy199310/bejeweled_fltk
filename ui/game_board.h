#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

class GameBoard {
public:
    GameBoard(){};
    GameBoard(int height, int width);
    ~GameBoard(){};
    
    int init();
private:
	int _board_height;
	int _board_width;
};

#endif