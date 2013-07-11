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

	printf("*****FIRST INIT*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			printf(" %d", _position[i + _board_size * j]);
		}
		printf("\n");
	}
	printf("****************************\n");

	int start, end;
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			// check potential elimination
			printf("CHECKING %d %d\n", i, j);
			int around[_color_size];
			if (_check_elimination(i, j, VERTICAL, &start, &end) > 0 || _check_elimination(i, j, HORIZONTAL, &start, &end) > 0) {
				printf("FIND POTENTIAL ELIMINATION %d %d\n", i, j);
				if (i - 1 >= 0) {
					printf("COLOR %d CANCELED\n", _position[i - 1 + _board_size * j]);
					around[_position[i - 1 + _board_size * j]] = -1;
				}
				if (j - 1 >= 0) {
					printf("COLOR %d CANCELED\n", _position[i + _board_size * (j - 1)]);
					around[_position[i + _board_size * (j - 1)]] = -1;
				}
				if (i + 1 < _board_size) {
					printf("COLOR %d CANCELED\n", _position[i + 1 + _board_size * j]);
					around[_position[i + 1 + _board_size * j]] = -1;
				}
				if (j + 1 < _board_size) {
					printf("COLOR %d CANCELED\n", _position[i + _board_size * (j + 1)]);
					around[_position[i + _board_size * (j + 1)]] = -1;
				}
				int count = 0;
				for (int k = 0; k < _color_size; k++) {
					if (around[k] == -1) {
						count++;
					}
				}
				int choosable[_color_size - count];
				int index = 0;
				for (int k = 0; k < _color_size; k++) {
					if (around[k] != -1) {
						choosable[index] = k;
						index++;
					}
				}
				int before = _position[i + _board_size * j];
				_position[i + _board_size * j] = choosable[rand() % (_color_size - count)];
				printf("CHANGE %d %d FROM %d TO %d\n", i, j, before, _position[i + _board_size * j]);

			}
		}
	}

	printf("*****FINAL INIT*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			printf(" %d", _position[i + _board_size * j]);
		}
		printf("\n");
	}
	printf("****************************\n");

	return _position;
}

int JewelLogic::move_check(int jewel1, int jewel2, move_direction direction, EliminationMessage* em) {
	// swap logic value of the two jewels
	int temp = _position[jewel1];
	_position[jewel1] = _position[jewel2];
	_position[jewel2] = temp;

	// test elimination
	printf("*****BEFORE ELIMINATION*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			printf(" %d", _position[i + _board_size * j]);
		}
		printf("\n");
	}
	printf("****************************\n");

	int i1 = jewel1 % 8;
	int j1 = (jewel1 - jewel1 % 8) / 8;
	int i2 = jewel2 % 8;
	int j2 = (jewel2 - jewel2 % 8) / 8;
	printf("start move check %d %d %d %d\n", i1, j1, i2, j2);

	// prepare for elimination
	int* eliminate_i = new int[_max_elimination_count];
	int* eliminate_j = new int[_max_elimination_count];
	int eliminate_index = 0;
	for (int k = 0; k < _max_elimination_count; k++) {
		eliminate_i[k] = -1;
		eliminate_j[k] = -1;
	}
	int* move_i = new int[_max_move_count];
	int* move_j = new int[_max_move_count];
	int* move_length = new int[_max_move_count];
	int move_index = 0;
	for (int k = 0; k < _max_move_count; k++) {
		move_i[k] = -1;
		move_j[k] = -1;
		move_length[k] = 0;
	}
	bool* partial_x = new bool[_board_size];
	int* partial_y = new int[_board_size];
	int partial_size = 0;
	int* partial_color;
	for (int k = 0; k < _board_size; k++) {
		partial_x[k] = false;
		partial_y[k] = 0;
	}

	// get a temporary state
	printf("GENERATE A TEMPORARY STATE\n");
	int start, end;
	int _position2 = _position[i2 + _board_size * j2];
	int _position1 = _position[i1 + _board_size * j1];
	bool will_eliminate = false;
	bool will_eliminate2 = false;
	bool will_eliminate1 = false;
	if (_check_elimination(i2, j2, VERTICAL, &start, &end) > 0) {
		printf("2 VERTICAL START:%d END:%d\n", start, end);
		will_eliminate = true;
		will_eliminate2 = true;
		for (int j = start; j <= end; j++) {
			_position[i2 + _board_size * j] = -1;
		}
	}
	_position[i2 + _board_size * j2] = _position2;
	if (_check_elimination(i2, j2, HORIZONTAL, &start, &end) > 0) {
		printf("2 HORIZONTAL START:%d END:%d\n", start, end);
		will_eliminate = true;
		for (int i = start; i <= end; i++) {
			_position[i + _board_size * j2] = -1;
		}
	}
	if (will_eliminate2) {
		_position[i2 + _board_size * j2] = -1;
	}
	if (_check_elimination(i1, j1, VERTICAL, &start, &end) > 0) {
		printf("1 VERTICAL START:%d END:%d\n", start, end);
		will_eliminate = true;
		will_eliminate1 = true;
		for (int j = start; j <= end; j++) {
			_position[i1 + _board_size * j] = -1;
		}
	}
	_position[i1 + _board_size * j1] = _position1;
	if (_check_elimination(i1, j1, HORIZONTAL, &start, &end) > 0) {
		printf("1 HORIZONTAL START:%d END:%d\n", start, end);
		will_eliminate = true;
		for (int i = start; i <= end; i++) {
			_position[i + _board_size * j1] = -1;
		}
	}
	if (will_eliminate1) {
		_position[i1 + _board_size * j1] = -1;
	}

	// test elimination
	printf("*****AFTER ELIMINATION*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			printf(" %d", _position[i + _board_size * j]);
		}
		printf("\n");
	}
	printf("****************************\n");

	// summarize the result
	printf("SUMMARIZE THE RESULT\n");
	if (will_eliminate) {
		for (int i = 0; i < _board_size; i++) {
			bool will_eliminate_column = false;
			int will_eliminate_count = 0;
			// fill elimination arrays
			for (int j = 0; j < _board_size; j++) {
				if (_position[i + _board_size * j] < 0) {
					will_eliminate_column = true;
					will_eliminate_count++;
					eliminate_i[eliminate_index] = i;
					eliminate_j[eliminate_index] = j;
					eliminate_index++;
				}
			}
			if (will_eliminate_column) {
				// fill move arrays
				for (int j = 0; j < _board_size; j++) {
					if (_position[i + _board_size * j] >= 0) {
						move_i[move_index] = i;
						move_j[move_index] = j;
						move_length[move_index] = will_eliminate_count;
						move_index++;
					} else {
						break;
					}
				}
				// fill partial arrays
				partial_x[i] = true;
				partial_y[i] = will_eliminate_count;
				partial_size += will_eliminate_count;
			}
		}

		// generate the colors of the new jewels
		partial_color = new int[partial_size];
		srand(time(NULL));
		for (int k = 0; k < partial_size; k++) {
			partial_color[k] = rand() % _color_size;
		}
	}

	// test elimination
	printf("*****SUMMARIZE ELIMINATION*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			bool is_eliminate = false;
			for (int k = 0; k < eliminate_index; k++) {
				if (eliminate_i[k] + _board_size * eliminate_j[k] == i + _board_size * j) {
					is_eliminate = true;
				}
			}
			if (is_eliminate) {
				printf(" %d", 1);
			} else {
				printf(" %d", 0);
			}
		}
		printf("\n");
	}
	printf("****************************\n");

	// test elimination
	printf("*****SUMMARIZE MOVE*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			bool is_move = false;
			for (int k = 0; k < move_index; k++) {
				if (move_i[k] + _board_size * move_j[k] == i + _board_size * j) {
					is_move = true;
				}
			}
			if (is_move) {
				printf(" %d", 1);
			} else {
				printf(" %d", 0);
			}
		}
		printf("\n");
	}
	printf("****************************\n");

	// test elimination
	printf("*****SUMMARIZE PARTIAL*****\n");
	for (int k = 0; k < _board_size; k++) {
		if (partial_x[k]) {
			printf(" %d", partial_y[k]);
		} else {
			printf(" %d", 0);
		}
	}
	printf("\n %d\n", partial_size);
	for (int k = 0; k < partial_size; k++) {
		printf(" %d", partial_color[k]);
	}
	printf("\n****************************\n");

	// call GUI to show the result
	printf("CALL GUI\n");
	if (will_eliminate) {
		_score += 10;
		_jb->eliminate_once(eliminate_i, eliminate_j, eliminate_index, move_i, move_j, move_length, move_index, partial_x, partial_y, partial_color, partial_size);
	}

	// get the current state
	partial_size = 0;
	int* position_copy = new int[_board_size * _board_size];
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			position_copy[i + _board_size * j] = _position[i + _board_size * j];
		}
	}
	for (int k = 0; k < move_index; k++) {
		//printf("MOVING (%d,%d)=%d TO (%d,%d)=%d\n", move_i[k], move_j[k], position_copy[move_i[k] + _board_size * move_j[k]], move_i[k], move_j[k] + move_length[k], _position[move_i[k] + _board_size * (move_j[k] + move_length[k])]);
		_position[move_i[k] + _board_size * (move_j[k] + move_length[k])] = position_copy[move_i[k] + _board_size * move_j[k]];
	}
	for (int k = 0; k < _board_size; k++) {
		if (partial_x[k]) {
			for (int j = 0; j < partial_y[k]; j++) {
				//printf("SETTING (%d,%d)=%d TO %d\n", k, j, _position[k + _board_size * j], partial_color[partial_size]);
				_position[k + _board_size * j] = partial_color[partial_size];
				partial_size++;
			}
		}
	}

	// test elimination
	printf("*****FINAL STATE*****\n");
	for (int j = 0; j < _board_size; j++) {
		for (int i = 0; i < _board_size; i++) {
			printf(" %d", _position[i + _board_size * j]);
		}
		printf("\n");
	}
	printf("****************************\n");

	// check_elimination_all
	// prepare for elimination
	int* eliminate_i_all = new int[_max_elimination_count];
	int* eliminate_j_all = new int[_max_elimination_count];
	int eliminate_index_all = 0;
	for (int k = 0; k < _max_elimination_count; k++) {
		eliminate_i_all[k] = -1;
		eliminate_j_all[k] = -1;
	}
	int* move_i_all = new int[_max_move_count];
	int* move_j_all = new int[_max_move_count];
	int* move_length_all = new int[_max_move_count];
	int move_index_all = 0;
	for (int k = 0; k < _max_move_count; k++) {
		move_i_all[k] = -1;
		move_j_all[k] = -1;
		move_length_all[k] = 0;
	}
	bool* partial_x_all = new bool[_board_size];
	int* partial_y_all = new int[_board_size];
	int partial_size_all = 0;
	int* partial_color_all;
	for (int k = 0; k < _board_size; k++) {
		partial_x_all[k] = false;
		partial_y_all[k] = 0;
	}

	printf("CONTINUE ELIMINATION CHECK\n");
	bool test = true;
	while (_check_elimination_all(eliminate_i, eliminate_j, eliminate_index, move_i, move_j, move_length, move_index, partial_x, partial_y, partial_color, partial_size) && test) {
		printf("CONTINUE ELIMINATION\n");
		// clear
		eliminate_index_all = 0;
		for (int k = 0; k < _max_elimination_count; k++) {
			eliminate_i_all[k] = -1;
			eliminate_j_all[k] = -1;
		}
		move_index_all = 0;
		for (int k = 0; k < _max_move_count; k++) {
			move_i_all[k] = -1;
			move_j_all[k] = -1;
			move_length_all[k] = 0;
		}
		partial_size_all = 0;
		for (int k = 0; k < _board_size; k++) {
			partial_x_all[k] = false;
			partial_y_all[k] = 0;
		}

		printf("*****BEFORE ELIMINATION ALL*****\n");
		for (int j = 0; j < _board_size; j++) {
			for (int i = 0; i < _board_size; i++) {
				printf(" %d", _position[i + _board_size * j]);
			}
			printf("\n");
		}
		printf("****************************\n");

		// get a temporary state
		int start, end;
		for (int k = 0; k < eliminate_index; k++) {
			int current_position = _position[eliminate_i[k] + _board_size * eliminate_j[k]];
			bool will_eliminate_current = false;
			if (_check_elimination(eliminate_i[k], eliminate_j[k], VERTICAL, &start, &end) > 0) {
				will_eliminate_current = true;
				for (int j = start; j <= end; j++) {
					_position[eliminate_i[k] + _board_size * j] = -1;
				}
			}
			_position[eliminate_i[k] + _board_size * eliminate_j[k]] = current_position;
			if (_check_elimination(eliminate_i[k], eliminate_j[k], HORIZONTAL, &start, &end) > 0) {
				will_eliminate_current = true;
				for (int i = start; i <= end; i++) {
					_position[i + _board_size * eliminate_j[k]] = -1;
				}
			}
			if (will_eliminate_current) {
				_position[eliminate_i[k] + _board_size * eliminate_j[k]] = -1;
			}
		}
		for (int k = 0; k < move_index; k++) {
			int current_position = _position[move_i[k] + _board_size * move_j[k]];
			bool will_eliminate_current = false;
			if (_check_elimination(move_i[k], move_j[k], VERTICAL, &start, &end) > 0) {
				will_eliminate_current = true;
				for (int j = start; j <= end; j++) {
					_position[move_i[k] + _board_size * j] = -1;
				}
			}
			_position[move_i[k] + _board_size * move_j[k]] = current_position;
			if (_check_elimination(move_i[k], move_j[k], HORIZONTAL, &start, &end) > 0) {
				will_eliminate_current = true;
				for (int i = start; i <= end; i++) {
					_position[i + _board_size * move_j[k]] = -1;
				}
			}
			if (will_eliminate_current) {
				_position[move_i[k] + _board_size * move_j[k]] = -1;
			}
		}
		for (int k = 0; k < _board_size; k++) {
			if (partial_x[k]) {
				for (int j = 0; j < partial_y[k]; j++) {
					int current_position = _position[k + _board_size * j];
					bool will_eliminate_current = false;
					if (_check_elimination(k, j, VERTICAL, &start, &end) > 0) {
						will_eliminate_current = true;
						for (int j2 = start; j2 <= end; j2++) {
							_position[k + _board_size * j2] = -1;
						}
					}
					_position[k + _board_size * j] = current_position;
					if (_check_elimination(k, j, HORIZONTAL, &start, &end) > 0) {
						will_eliminate_current = true;
						for (int i = start; i <= end; i++) {
							_position[i + _board_size * j] = -1;
						}
					}
					if (will_eliminate_current) {
						_position[k + _board_size * j] = -1;
					}
				}
			}
		}

		// test elimination
		printf("*****AFTER ELIMINATION ALL*****\n");
		for (int j = 0; j < _board_size; j++) {
			for (int i = 0; i < _board_size; i++) {
				printf(" %d", _position[i + _board_size * j]);
			}
			printf("\n");
		}
		printf("****************************\n");

		printf("SUMMARIZE THE RESULT\n");
		for (int i = 0; i < _board_size; i++) {
			bool will_eliminate_column = false;
			int will_eliminate_count = 0;
			// fill elimination arrays
			for (int j = 0; j < _board_size; j++) {
				if (_position[i + _board_size * j] < 0) {
					will_eliminate_column = true;
					will_eliminate_count++;
					eliminate_i_all[eliminate_index_all] = i;
					eliminate_j_all[eliminate_index_all] = j;
					printf("ELIMINATION i:%d j:%d\n", eliminate_i_all[eliminate_index], eliminate_j_all[eliminate_index]);
					eliminate_index_all++;
				}
			}
			if (will_eliminate_column) {
				// fill move arrays
				for (int j = 0; j < _board_size; j++) {
					if (_position[i + _board_size * j] >= 0) {
						move_i_all[move_index_all] = i;
						move_j_all[move_index_all] = j;
						move_length_all[move_index_all] = will_eliminate_count;
						move_index_all++;
					} else {
						break;
					}
				}
				// fill partial arrays
				partial_x_all[i] = true;
				partial_y_all[i] = will_eliminate_count;
				partial_size_all += will_eliminate_count;
			}

		for (int k = 0; k < eliminate_index_all; k++) {
			printf("ELIMINATION IN i:%d j:%d\n", eliminate_i_all[k], eliminate_j_all[k]);
		}
		}
		for (int k = 0; k < eliminate_index_all; k++) {
			printf("ELIMINATION OUT i:%d j:%d\n", eliminate_i_all[k], eliminate_j_all[k]);
		}

		// generate the colors of the new jewels
		partial_color_all = new int[partial_size_all];
		srand(time(NULL));
		for (int k = 0; k < partial_size_all; k++) {
			partial_color_all[k] = rand() % _color_size;
		}

		// call GUI to show the result
		printf("CALL GUI\n");
		_score += 10;
		for (int k = 0; k < eliminate_index_all; k++) {
			printf("ELIMINATION i:%d j:%d\n", eliminate_i_all[k], eliminate_j_all[k]);
		}
		_jb->eliminate_once(eliminate_i_all, eliminate_j_all, eliminate_index_all, move_i_all, move_j_all, move_length_all, move_index_all, partial_x_all, partial_y_all, partial_color_all, partial_size_all);
		
		// get the current state
		partial_size_all = 0;
		int* position_copy = new int[_board_size * _board_size];
		for (int j = 0; j < _board_size; j++) {
			for (int i = 0; i < _board_size; i++) {
				position_copy[i + _board_size * j] = _position[i + _board_size * j];
			}
		}
		for (int k = 0; k < move_index_all; k++) {
			//printf("MOVING (%d,%d)=%d TO (%d,%d)=%d\n", move_i[k], move_j[k], position_copy[move_i[k] + _board_size * move_j[k]], move_i[k], move_j[k] + move_length[k], _position[move_i[k] + _board_size * (move_j[k] + move_length[k])]);
			_position[move_i_all[k] + _board_size * (move_j_all[k] + move_length_all[k])] = position_copy[move_i_all[k] + _board_size * move_j_all[k]];
		}
		for (int k = 0; k < _board_size; k++) {
			if (partial_x_all[k]) {
				for (int j = 0; j < partial_y_all[k]; j++) {
					//printf("SETTING (%d,%d)=%d TO %d\n", k, j, _position[k + _board_size * j], partial_color[partial_size]);
					_position[k + _board_size * j] = partial_color_all[partial_size_all];
					partial_size_all++;
				}
			}
		}

		eliminate_i = eliminate_i_all;
		eliminate_j = eliminate_j_all;
		eliminate_index = eliminate_index_all;
		move_i = move_i_all;
		move_j = move_j_all;
		move_length = move_length_all;
		move_index = move_index_all;
		partial_x = partial_x_all;
		partial_y = partial_y_all;
		partial_color = partial_color_all;
		partial_size = partial_size_all;

		//test = false;
	}

	printf("STOP ELIMINATION CHECK\n");

	if (will_eliminate) {
		return 1;
	} else {
		return -1;
	}
}


bool JewelLogic::_check_elimination_all(int* eliminate_i, int* eliminate_j, int eliminate_size, int* move_i, int* move_j, int* move_length, int move_size, bool* partial_x, int* partial_y, int* partial_color, int partial_size) {
	int start, end;
	for (int k = 0; k < eliminate_size; k++) {
		if (_check_elimination(eliminate_i[k], eliminate_j[k], HORIZONTAL, &start, &end) || _check_elimination(eliminate_i[k], eliminate_j[k], VERTICAL, &start, &end)) {
			return true;
		}
	}
	for (int k = 0; k < move_size; k++) {
		if (_check_elimination(move_i[k], move_j[k], HORIZONTAL, &start, &end) || _check_elimination(move_i[k], move_j[k], VERTICAL, &start, &end)) {
			return true;
		}
	}
	for (int k = 0; k < _board_size; k++) {
		if (partial_x[k]) {
			for (int j = 0; j < partial_y[k]; j++) {
				if (_check_elimination(k, j, HORIZONTAL, &start, &end) || _check_elimination(k, j, VERTICAL, &start, &end)) {
					return true;
				}
			}
		}
	}
	return false;
}

int JewelLogic::_check_elimination(int x, int y, _check_direction cd, int* start, int* end) {
	int i, j;
	int count = 0;
	*start = -1;
	*end = -1;
	switch (cd) {
		case HORIZONTAL:
			count++;
			for (i = x - 1; i >= 0; i--) {
				if (_position[i + _board_size * y] == _position[x + _board_size * y]) {
					printf("(%d %d) (%d %d) are the same %d %d\n", i, y, x, y, _position[i + _board_size * y], _position[x + _board_size * y]);
					count++;
					*start = i;
					printf("start:%d\n", *start);
				} else {
					break;
				}
			}
			if (*start == -1) {
				*start = x;
				//printf("temp start:%d\n", *start);
			}
			for (i = x + 1; i < _board_size; i++) {
				if (_position[i + _board_size * y] == _position[x + _board_size * y]) {
					printf("(%d %d) (%d %d) are the same %d %d\n", i, y, x, y, _position[i + _board_size * y], _position[x + _board_size * y]);
					count++;
					*end = i;
				} else {
					break;
				}
			}
			printf("HORIZONTAL COUNT: %d\n", count);
			if (*end == -1) {
				*end = x;
			}
			break;
		case VERTICAL:
			count++;
			for (j = y - 1; j >= 0; j--) {
				if (_position[x + _board_size * j] == _position[x + _board_size * y]) {
					printf("(%d %d) (%d %d) are the same %d %d\n", x, j, x, y, _position[x + _board_size * j], _position[x + _board_size * y]);
					count++;
					*start = j;
				} else {
					break;
				}
			}
			if (*start == -1) {
				*start = y;
			}
			for (j = y + 1; j < _board_size; j++) {
				if (_position[x + _board_size * j] == _position[x + _board_size * y]) {
					printf("(%d %d) (%d %d) are the same %d %d\n", x, j, x, y, _position[x + _board_size * j], _position[x + _board_size * y]);
					count++;
					*end = j;
				} else {
					break;
				}
			}
			printf("VERTICAL COUNT: %d\n", count);
			if (*end == -1) {
				*end = y;
			}
			break;
		default:
			break;
	}
	if (count >= 3) {
		return count;
	} else {
		return -1;
	}
}

int JewelLogic::get_score() {
	return _score;
}

int JewelLogic::get_hint() {
	int start, end;
	int result = -1;
	bool get_hint = false;
	for (int j = 0; j < _board_size && !get_hint; j++) {
		for (int i = 0; i < _board_size && !get_hint; i++) {
			// move to left
			if (i - 1 >= 0) {
				int temp = _position[i - 1 + _board_size * j];
				_position[i - 1 + _board_size * j] = _position[i + _board_size * j];
				_position[i + _board_size * j] = temp;
				if (_check_elimination(i - 1, j, VERTICAL, &start, &end) > 0 || _check_elimination(i - 1, j, HORIZONTAL, &start, &end) > 0 || _check_elimination(i, j, VERTICAL, &start, &end) > 0 || _check_elimination(i, j, HORIZONTAL, &start, &end) > 0) {
					printf("MOVE TO LEFT\n");
					result = i + _board_size * j;
					get_hint = true;
					temp = _position[i - 1 + _board_size * j];
					_position[i - 1 + _board_size * j] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
					break;
				} else {
					temp = _position[i - 1 + _board_size * j];
					_position[i - 1 + _board_size * j] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
				}
			}
			// move to right
			else if (i + 1 < _board_size) {
				int temp = _position[i + 1 + _board_size * j];
				_position[i + 1 + _board_size * j] = _position[i + _board_size * j];
				_position[i + _board_size * j] = temp;
				if (_check_elimination(i + 1, j, VERTICAL, &start, &end) > 0 || _check_elimination(i + 1, j, HORIZONTAL, &start, &end) > 0 || _check_elimination(i, j, VERTICAL, &start, &end) > 0 || _check_elimination(i, j, HORIZONTAL, &start, &end) > 0) {
					printf("MOVE TO RIGHT\n");
					result = i + _board_size * j;
					get_hint = true;
					temp = _position[i + 1 + _board_size * j];
					_position[i + 1 + _board_size * j] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
					break;
				} else {
					temp = _position[i + 1 + _board_size * j];
					_position[i + 1 + _board_size * j] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
				}
			}
			// move to up
			else if (j - 1 >= 0) {
				int temp = _position[i + _board_size * (j - 1)];
				_position[i + _board_size * (j - 1)] = _position[i + _board_size * j];
				_position[i + _board_size * j] = temp;
				if (_check_elimination(i, j - 1, VERTICAL, &start, &end) > 0 || _check_elimination(i, j - 1, HORIZONTAL, &start, &end) > 0 || _check_elimination(i, j, VERTICAL, &start, &end) > 0 || _check_elimination(i, j, HORIZONTAL, &start, &end) > 0) {
					printf("MOVE TO UP\n");
					result = i + _board_size * j;
					get_hint = true;
					temp = _position[i + _board_size * (j - 1)];
					_position[i + _board_size * (j - 1)] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
					break;
				} else {
					temp = _position[i + _board_size * (j - 1)];
					_position[i + _board_size * (j - 1)] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
				}
			}
			// move to down
			else if (j + 1 < _board_size) {
				int temp = _position[i + _board_size * (j + 1)];
				_position[i + _board_size * (j + 1)] = _position[i + _board_size * j];
				_position[i + _board_size * j] = temp;
				if (_check_elimination(i, j + 1, VERTICAL, &start, &end) > 0 || _check_elimination(i, j + 1, HORIZONTAL, &start, &end) > 0 || _check_elimination(i, j, VERTICAL, &start, &end) > 0 || _check_elimination(i, j, HORIZONTAL, &start, &end) > 0) {
					printf("MOVE TO DOWN\n");
					result = i + _board_size * j;
					get_hint = true;
					temp = _position[i + _board_size * (j + 1)];
					_position[i + _board_size * (j + 1)] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
					break;
				} else {
					temp = _position[i + _board_size * (j + 1)];
					_position[i + _board_size * (j + 1)] = _position[i + _board_size * j];
					_position[i + _board_size * j] = temp;
				}
			}
		}
	}
	return result;
}