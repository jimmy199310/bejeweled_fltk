#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include "jewel_box.h"
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_JPEG_Image.H>

class JewelBoard;
class Fl_Menu_Window;

class GameBoard : public Fl_Window {
public:
    //GameBoard(){};
    GameBoard(int height, int width);
    //~GameBoard()

	void set_score_label(int score);
	void set_hint();
	//void draw();

	Fl_Box* _tl;
	int _time;
	char _ctl[3];
	Fl_Menu_Window* _pop_win;
	void pop_memu_window();
private:
	void _set_time_label();

	int _board_height;
	int _board_width;

	Fl_Window* _win;
	JewelBoard* _jb;
	Fl_Box* _sl;
	char _csl[4];
	//HintButton* _hb;
	 Fl_JPEG_Image* background;
};

#endif