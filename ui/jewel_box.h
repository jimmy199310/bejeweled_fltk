#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include "../logic/jewel_logic.h"

class JewelBox : public Fl_Box {
public:
	JewelBox(Fl_Window* win, int x, int y, int width, int height);
	//int handle(int event);
private:
	Fl_Window* _win;
	// side weight of the jewel box
	int _box_side;
};

class JewelBoard : public Fl_Box {
public:
	JewelBoard(Fl_Window* win, int start_x, int start_y, int box_side);
	//JewelBox* get_jewel_box(int i , int j);
	int handle(int event);
private:
	void _init_jewel_color();

	//enum _direction {UP, DOWN, LEFT, RIGHT};
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
	Fl_Window* _win;
	// save 7-color jewel images
	Fl_PNG_Image* _jewel_images[7];
	enum _jewel_color {PURPLE, ORANGE, SILVERY, GREEN, YELLOW, RED, BLUE};
	// record current active jewel
	int _last_active_i;
	int _last_active_j;
};