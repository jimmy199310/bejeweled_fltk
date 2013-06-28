#include "jewel_logic.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int* JewelLogic::gen_init() {
	_position = new int[_board_size * _board_size];
	srand(time(NULL));
	for (int i = 0; i < _board_size; i++) {
		for (int j = 0; j < _board_size; j++) {
			_position[i + _board_size * j] = rand() % _color_size;
		}
	}
	return _position;
}

int JewelLogic::move_check(int jewel1, int jewel2, move_direction direction, EliminationMessage* em) {
	// swap logic value of the two jewels
	int temp = _position[jewel1];
	_position[jewel1] = _position[jewel2];
	_position[jewel2] = temp;
	//for (int i = jewel; i < )
	em->explode = EXPLODE;
	em->horizontal = true;
	em->vertical = true;
	return 1;
}