#ifndef JEWEL_LOGIC_H_
#define JEWEL_LOGIC_H_

#include "../ui/jewel_box.h"

class JewelBoard;

class JewelLogic {
public:
	JewelLogic(JewelBoard* jb, int board_size, int color_size) : _jb(jb), _board_size(board_size), _color_size(color_size), _max_elimination_count(64), _max_move_count(64), _score(0) {};
	// generate a start state of jewel board
	int* gen_init();
	// move the jewel and check elimination
	enum explode_type {EXPLODE, LIGHTNING, RANDOM};
	struct EliminationMessage {
		explode_type explode;
		bool horizontal;
		int h_start;
		int h_end;
		bool vertical;
		int v_start;
		int v_end;
	};
	enum move_direction {NONE, LEFT_TO_RIGHT, RIGHT_TO_LEFT, UP_TO_DOWN, DOWN_TO_UP};
	//enum elimination_type {HORIZONTAL, VERTICAL, BOTH};
	enum direction {UP, DOWN, LEFT, RIGHT};
	int move_check(int jewel1, int jewel2, move_direction direction, EliminationMessage* em);
	int get_score();
	int get_hint();
private:
	enum _check_direction {HORIZONTAL, VERTICAL};
	int _check_elimination(int x, int y, _check_direction cd, int* start, int* end);
	bool _check_elimination_all(int* eliminate_i, int* eliminate_j, int eliminate_size, int* move_i, int* move_j, int* move_length, int move_size, bool* partial_x, int* partial_y, int* partial_color, int partial_size);

	JewelBoard* _jb;
	int _board_size;
	int _color_size;
	int* _position;

	int _max_elimination_count;
	int _max_move_count;

	int _score;
};

#endif