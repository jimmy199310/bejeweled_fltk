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

	int i1 = jewel1 % 8;
	int j1 = (jewel1 - jewel1 % 8) / 8;
	int i2 = jewel2 % 8;
	int j2 = (jewel2 - jewel2 % 8) / 8;
	int count = 0;
	bool can_eliminate = true;

	printf("start move check %d %d %d %d\n", i1, j1, i2, j2);

	while (can_eliminate) {
		switch (direction) {
			case NONE:
				break;
			case LEFT_TO_RIGHT:

				break;
			case RIGHT_TO_LEFT:
				break;
			case UP_TO_DOWN:
				break;
			case DOWN_TO_UP:
			{
				printf("*****DOWN_TO_UP*****\n");
				// check up
				/*for (int m = i1, n = j1 - 1; n >= 0; n -= 1) {
					if (_position[m + 8 * n] == _position[i1 + 8 * j1]) {
						count++;
					} else {
						break;
					}
				}*/
				count = 3;
				if (count >= 3) {
					int* eliminate_i = new int[count];
					int* eliminate_j = new int[count];
					for (int m = 0; m < count; m++) {
						eliminate_i[m] = i2;
						eliminate_j[m] = j2 - m;
					}
					int* move_i = new int[j2 - count + 1];
					int* move_j = new int[j2 - count + 1];
					for (int m = 0; m <= j2 - count; m++) {
						move_i[m] = i2;
						move_j[m] = m;
					}
					_jb->eliminate(eliminate_i, eliminate_j, count, move_i, move_j, j2 - count + 1, 180);
					can_eliminate = false;
				}
				break;
			}
			default:
				break;
		}
	}


	//for (int i = jewel; i < )
	em->explode = EXPLODE;
	em->horizontal = true;
	em->vertical = true;
	return 1;
}