#ifndef JEWEL_BOX_H_
#define JEWEL_BOX_H_

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include "game_board.h"
#include "../logic/jewel_logic.h"

class JewelLogic;
class GameBoard;

class JewelBox : public Fl_Box {
public:
	JewelBox(Fl_Window* win, int x, int y, int width, int height);
private:
	Fl_Window* _win;
	// side weight of the jewel box
	int _box_side;
};

class JewelBoard : public Fl_Box {
public:
	JewelBoard(/*Fl_Window* win*/GameBoard* win, int start_x, int start_y, int box_side);
	int handle(int event);
	// override for the gradient animation
	void draw();
	// invoked by JewelLogic to execute elimination
	//void eliminate(int* eliminate_i, int* eliminate_j, int eliminate_length, int* move_i, int* move_j, int move_length, int range);
	void eliminate_once(int* eliminate_i, int* eliminate_j, int eliminate_size, int* move_i, int* move_j, int* move_length, int move_size, bool* partial_x, int* partial_y, int* partial_color, int partial_size);

	void set_hint();
private:
	void _init_jewel_color();
	void _move_jewel(int last_active_i, int last_active_j, int cur_active_i, int cur_active_j);
	void _swap_jewel(int jewel1, int jewel2);

	// start position
	int _start_x;
	int _start_y;
	int _box_side;
	// board size
	int _size;
	// 64 jewel boxes
	JewelBox* _jewels[8*8];
	// record a logic state of 64 boxes
	JewelLogic* _logic;
	// use parent window to redraw, may use Fl::redraw() instead
	//Fl_Window* _win;
	GameBoard* _win;
	// save 7-color jewel images
	Fl_PNG_Image* _jewel_images[7];
	enum _jewel_color {PURPLE, ORANGE, SILVERY, GREEN, YELLOW, RED, BLUE};
	// record current active jewel
	int _last_active_i;
	int _last_active_j;
	//Fl_PNG_Image* _new_jewel_images[5];
	//int _new_jewel_size;
	// whether the new jewels are showing up
	bool _is_partial;
	// the showing up x-position and y-depth
	bool* _partial_x;
	int* _partial_y;
	// offset of the new jewels
	int _offset_x;
	int _offset_y;
	// partial color
	int* _partial_color;
	// index of the new jewels
	int _partial_index;
	// whether the partial animation finishes
	bool _is_partial_animating;
	// move speed
	int _move_speed;
	// hint position
	int _hint;
};

#endif