#include "jewel_box.h"
#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>
#include <time.h>
#include <unistd.h>

JewelBox::JewelBox(Fl_Window* win, int x, int y, int width, int height) : Fl_Box(FL_NO_BOX, x, y, width, height, 0) {
	_win = win;
	_box_side = width;
}

/*int JewelBox::handle(int event) {
	static int offset[2] = { 0, 0 };
	static int sensitive_move = _box_side / 2;
	int ret = Fl_Box::handle(event);
	switch (event) {
		case FL_PUSH:
			offset[0] = Fl::event_x() - x();    // save where user clicked for dragging
			offset[1] = Fl::event_y() - y();


			for (int i = 0; i < 10; i++) {
				printf("PUSH: %d %d %d %d %d %d\n", x(), Fl::event_x(), y(), Fl::event_y(), Fl::event_x() - offset[0] + i, Fl::event_y() - offset[1]);
				position(Fl::event_x() - offset[0] - i, Fl::event_y() - offset[1]);
				Fl::check();
				Fl::redraw();
				usleep(500000);
			}
			return(1);
		case FL_RELEASE:
			return(1);
		case FL_DRAG:
			//printf("PUSH: %d %d %d %d %d %d\n", offset[0], Fl::event_x(), x(), offset[1], Fl::event_y(), y());
			if (Fl::event_x() - x() - offset[0] >= sensitive_move) {

			}
			//if (Fl::event_x() - offset[0] > x() && Fl::event_y() - offset[1] <= y()) {
				position(Fl::event_x() - offset[0], Fl::event_y() - offset[1]);     // handle dragging
				//printf("PUSH: %d %d %d %d\n", offset[0], Fl::event_x(), offset[1], Fl::event_y());
				_win->redraw();
				for (int i = 0; i < 100; i++) {
				position(Fl::event_x() - offset[0] + i, Fl::event_y() - offset[1]);
				redraw();
				sleep(1);
			}
			//}
			return(1);
		}
	return(ret);
}*/

JewelBoard::JewelBoard(/*Fl_Window* win*/GameBoard* win, int start_x, int start_y, int box_side) : Fl_Box(start_x, start_y, 8 * box_side, 8 * box_side, 0) {
	_win = win;
	_size = 8;
	_start_x = start_x;
	_start_y = start_y;
	_box_side = box_side;
	_last_active_i = -1;
	_last_active_j = -1;
	//_new_jewel_size = 5;
	_is_partial = false;
	_offset_x = 0;
	_offset_y = 0;
	_move_speed = 10;

	// load jewel images
	_jewel_images[0] = new Fl_PNG_Image("image/Jewel1.png");
	_jewel_images[1] = new Fl_PNG_Image("image/Jewel2.png");
	_jewel_images[2] = new Fl_PNG_Image("image/Jewel3.png");
	_jewel_images[3] = new Fl_PNG_Image("image/Jewel4.png");
	_jewel_images[4] = new Fl_PNG_Image("image/Jewel5.png");
	_jewel_images[5] = new Fl_PNG_Image("image/Jewel6.png");
	_jewel_images[6] = new Fl_PNG_Image("image/Jewel7.png");

	// get game logic
	_logic = new JewelLogic(this, _size, 7);

	// create 8*8 jewel boxes, set color
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_jewels[i + 8 * j] = new JewelBox(_win, start_x + box_side * i, start_y + box_side * j, box_side, box_side);
		}
	}

	// init the game, set color to each jewel box
	_init_jewel_color();
}

void JewelBoard::_init_jewel_color() {
	int* position = _logic->gen_init();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_jewels[i + 8 * j]->image(_jewel_images[position[i + 8 * j]]);
		}
	}
}

int JewelBoard::handle(int event) {
	//printf("%d %d %d %d\n", event, FL_PUSH, FL_RELEASE, FL_DRAG);
	static int offset[2] = { 0, 0 };
	static int sensitive_move = _box_side / 2;
	int ret = Fl_Widget::handle(event);
	switch (event) {
		case FL_PUSH:
		{
			// will be used for dragging
			//offset[0] = Fl::event_x() - _start_x;
			//offset[1] = Fl::event_y() - _start_y;
			if (_hint >= 0) {
				_jewels[_hint]->box(FL_NO_BOX);
			}
			//printf("PUSH: %d %d %d %d %d %d %d\n", Fl::event_x(), _start_x, Fl::event_y(), _start_y, _box_side, _last_active_i, _last_active_j);
			int cur_active_i = (Fl::event_x() - _start_x) / _box_side;
			int cur_active_j = (Fl::event_y() - _start_y) / _box_side;
			if (_last_active_i < 0 || _last_active_j < 0) {
				// no last active jewel, update last active jewel
				_last_active_i = cur_active_i;
				_last_active_j = cur_active_j;
			} else {
				// check adjacency
				// if adjacent, swap two boxes
				_move_jewel(_last_active_i, _last_active_j, cur_active_i, cur_active_j);
				_last_active_i = -1;
				_last_active_j = -1;
			}

			//for (int i = 0; i < 10; i++) {
			//	printf("PUSH: %d %d %d %d %d %d\n", x(), Fl::event_x(), y(), Fl::event_y(), Fl::event_x() - offset[0] + i, Fl::event_y() - offset[1]);
			//	position(Fl::event_x() - offset[0] - i, Fl::event_y() - offset[1]);
			//	Fl::check();
			//	Fl::redraw();
			//	usleep(500000);
			//}
			return(1);
		}
		case FL_RELEASE:
			return(1);
		case FL_DRAG:
			//printf("PUSH: %d %d %d %d %d %d\n", offset[0], Fl::event_x(), x(), offset[1], Fl::event_y(), y());
			if (Fl::event_x() - x() - offset[0] >= sensitive_move) {

			}
			if (Fl::event_x() - offset[0] > x() && Fl::event_y() - offset[1] <= y()) {
				position(Fl::event_x() - offset[0], Fl::event_y() - offset[1]);     // handle dragging
				//printf("PUSH: %d %d %d %d\n", offset[0], Fl::event_x(), offset[1], Fl::event_y());
				_win->redraw();
				for (int i = 0; i < 100; i++) {
				position(Fl::event_x() - offset[0] + i, Fl::event_y() - offset[1]);
				redraw();
				sleep(1);
			}
			}
			return(1);
		default:
			break;
		}
	return ret;
}

void JewelBoard::draw() {
	Fl_Box::draw();
	if (_is_partial) {
		for (int k = 0; k < _size; k++) {
			if (_partial_x[k]) {
				for (int m = 0; m < _partial_y[k]; m++) {
					if (-_offset_y <= _partial_y[k] * _box_side) {
						_is_partial_animating = true;
						_jewel_images[_partial_color[_partial_index]]->draw(_start_x + _box_side * k, _start_y, _box_side, _box_side * _partial_y[k], 0, _offset_y + _box_side * (_partial_y[k] - m));
						_partial_index++;
					} else {
						_jewel_images[_partial_color[_partial_index]]->draw(_start_x + _box_side * k, _start_y, _box_side, _box_side * _partial_y[k], 0, -_partial_y[k] * _box_side + _box_side * (_partial_y[k] - m));
						_partial_index++;
					}
				}
			}
		}
		_partial_index = 0;
	}
}

/*void JewelBoard::eliminate(int* eliminate_i, int* eliminate_j, int eliminate_length, int* move_i, int* move_j, int move_length, int range) {
	// use select to delay for animation
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = 1; // 1us here, but same with about 20ms

	printf("start to eliminate\n");

	// eliminate the corresponding jewels, maybe more effect code here
	for (int k = 0; k < eliminate_length; k++) {
		Fl::delete_widget(_jewels[eliminate_i[k] + _size * eliminate_j[k]]);
	}

	// generate new jewels and corresponding boxes to replace eliminated jewels
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
			_new_jewel_images[i] = _jewel_images[rand() % 7];
	}

	int current_i = eliminate_i[0];
	JewelBox* new_jewels[eliminate_length];
	for (int k = 0; k < eliminate_length; k++) {
		new_jewels[k] = new JewelBox(_win, _start_x + _box_side * current_i, _start_y + _box_side * k, _box_side, _box_side);
		new_jewels[k]->image(_new_jewel_images[k]);
		_win->insert(*new_jewels[k], 10);
		new_jewels[k]->hide();
	}

	// move the corresponding jewels
	_is_partial = true;
	_partial_x[eliminate_i[0]] = true;
	_partial_y[eliminate_i[0]] = eliminate_length;
	for (int a = 0; a <= range; a += 1) {
		for (int k = 0; k < move_length; k++) {
			//printf("move: %d %d \n", _start_x + i[k] * _box_side, _start_y + j[k] * _box_side + a);
			_jewels[move_i[k] + _size * move_j[k]]->position(_start_x + move_i[k] * _box_side, _start_y + move_j[k] * _box_side + a);
		}
		_offset_y = range - a;
		Fl::check();
		Fl::redraw();
		select(0, NULL, NULL, NULL, &delay);
	}
	_partial_x[eliminate_i[0]] = false;
	_partial_y[eliminate_i[0]] = 0;
	_is_partial = false;

	// adjust the boxes to the right position
	for (int k = 0; k < move_length; k++) {
		//printf("ADJUST MOVE %d %d to %d %d\n", eliminate_i[0], eliminate_j[0] - k, move_i[move_length - 1 - k], move_j[move_length - 1 - k]);
		_jewels[eliminate_i[0] + _size * (eliminate_j[0] - k)] = _jewels[move_i[move_length - 1 - k] + _size * move_j[move_length - 1 - k]];
	}

	printf("move_length:%d eliminate_j[0]:%d eliminate_length:%d\n", move_length, eliminate_j[0], eliminate_length);
	for (int k = move_length; k <= eliminate_j[0]; k++) {
		_jewels[eliminate_i[0] + _size * (eliminate_j[0] - k)] = new_jewels[eliminate_j[0] - k];
		_jewels[eliminate_i[0] + _size * (eliminate_j[0] - k)]->show();
	}
}*/

void JewelBoard::eliminate_once(int* eliminate_i, int* eliminate_j, int eliminate_size, int* move_i, int* move_j, int* move_length, int move_size, bool* partial_x, int* partial_y, int* partial_color, int partial_size) {
	// use select to delay for animation
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = 1; // 1us here, but same with about 20ms

	printf("START ELIMINATION\n");

	// test invariant
	/*printf("*****BEFORE ELIMINATION THEORY*****\n");
	for (int j = 0; j < _size; j++) {
		for (int i = 0; i < _size; i++) {
			printf(" (%d,%d)", _start_x + _box_side * i, _start_y + _box_side * j);
		}
		printf("\n");
	}
	printf("**********************************\n");*/

	// test invariant
	printf("*****BEFORE ELIMINATION*****\n");
	for (int j = 0; j < _size; j++) {
		for (int i = 0; i < _size; i++) {
			printf(" (%d,%d)", _jewels[i + _size * j]->x(), _jewels[i + _size * j]->y());
		}
		printf("\n");
	}
	printf("****************************\n");

	// eliminate the corresponding jewels, maybe more effect code here
	for (int k = 0; k < eliminate_size; k++) {
		printf("DELETING %d %d\n", eliminate_i[k], eliminate_j[k]);
		Fl::delete_widget(_jewels[eliminate_i[k] + _size * eliminate_j[k]]);
		_jewels[eliminate_i[k] + _size * eliminate_j[k]] = NULL;
	}

	// test invariant
	/*printf("*****AFTER DELETING*****\n");
	for (int j = 0; j < _size; j++) {
		for (int i = 0; i < _size; i++) {
			if (_jewels[i + _size * j] != NULL) {
				printf(" (%d,%d)", _jewels[i + _size * j]->x(), _jewels[i + _size * j]->y());
			} else {
				printf(" (%s,%s)", "000", "000");
			}
		}
		printf("\n");
	}
	printf("****************************\n");*/

	// generate new JewelBoxes to prepare to replace eliminated jewels
	JewelBox* new_jewels[eliminate_size];
	int new_jewel_index = 0;
	for (int k = 0; k < _size; k++) {
		if (partial_x[k]) {
			for (int j = 0; j < partial_y[k]; j++) {
				new_jewels[new_jewel_index] = new JewelBox(_win, _start_x + _box_side * k, _start_y + _box_side * j, _box_side, _box_side);
				new_jewels[new_jewel_index]->image(_jewel_images[partial_color[new_jewel_index]]);
				_win->insert(*new_jewels[new_jewel_index], 100);
				new_jewels[new_jewel_index]->hide();
				new_jewel_index++;
			}
		}
	}

	// backup jewels
	JewelBox* jb_copy[_size * _size];
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			jb_copy[i + _size * j] = _jewels[i + _size * j];
		}
	}

	// move the corresponding jewels
	printf("MOVE THE CORRESPONDING JEWELS\n");
	_is_partial = true;
	_partial_x = partial_x;
	_partial_y = partial_y;
	_partial_color = partial_color;
	_partial_index = 0;
	bool _is_animating = false;
	for (int m = 0; m <= _box_side * _size; m += _move_speed) {
		for (int k = 0; k < move_size; k++) {
			if (m <= move_length[k] * _box_side) {
				_jewels[move_i[k] + _size * move_j[k]]->position(_start_x + move_i[k] * _box_side, _start_y + move_j[k] * _box_side + m);
				_is_animating = true;
			}
		}
		_offset_y = -m;
		Fl::check();
		Fl::redraw();
		if (_is_animating) {
			select(0, NULL, NULL, NULL, &delay);
			_is_animating = false;
		} else {
			break;
		}
	}
	_is_partial = false;

	// elimination on the top
	printf("ELIMINATION ON THE TOP\n");
	if (move_size == 0) {
		_is_partial = true;
		_is_partial_animating = false;
		for (int m = 0; m <= _box_side * _size; m += _move_speed) {
			_offset_y = -m;
			Fl::check();
			Fl::redraw();
			if (_is_partial_animating) {
				select(0, NULL, NULL, NULL, &delay);
				_is_partial_animating = false;
			} else {
				break;
			}
		}
	}

	_is_partial = false;
	new_jewel_index = 0;
	for (int k = 0; k < _size; k++) {
		if (_partial_x[k]) {
			for (int j = 0; j < _partial_y[k]; j++) {
				printf("X:%d MOVING (%d,%d) to (%d, %d) %d\n", k, new_jewels[new_jewel_index]->x(), new_jewels[new_jewel_index]->y(), _start_x + _box_side * k, _start_y + _box_side * j, partial_y[k]);
				_jewels[k + _size * j] = new_jewels[new_jewel_index];
				new_jewel_index++;
				_jewels[k+ _size * j]->show();
			}
		}
	}

	// adjust the boxes to the right position
	printf("start %d\n", move_size);
	for (int k = 0; k < move_size; k++) {
		printf("MOVING (%d,%d) to (%d, %d)\n", move_i[k], move_j[k], move_i[k], move_j[k] + move_length[k]);
		_jewels[move_i[k] + _size * (move_j[k] + move_length[k])] = jb_copy[move_i[k] + _size * move_j[k]];
	}

	// test invariant
	printf("*****AFTER ELIMINATION*****\n");
	for (int j = 0; j < _size; j++) {
		for (int i = 0; i < _size; i++) {
			printf(" (%d,%d)\n", _jewels[i + _size * j]->x(), _jewels[i + _size * j]->y());
		}
		printf("\n");
	}
	printf("****************************\n");

	_win->set_score_label(_logic->get_score());
}

void JewelBoard::_move_jewel(int last_active_i, int last_active_j, int cur_active_i, int cur_active_j) {
	// two jewels ready to swap
	int jewel1 = last_active_i + last_active_j * _size;
	int jewel2 = cur_active_i + cur_active_j * _size;

	// use select to delay for animation
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = 1; // 1us here, but same with about 20ms

	// move jewel animation
	JewelLogic::move_direction direction;
	if (cur_active_i - last_active_i == 1 && cur_active_j - last_active_j == 0) {
		// LEFT-TO-RIGHT
		direction = JewelLogic::LEFT_TO_RIGHT;
		for (int i = 0; i <= _box_side; i += 10) {
			_jewels[jewel1]->position(_start_x + last_active_i * _box_side + i, _start_y + last_active_j * _box_side);
			_jewels[jewel2]->position(_start_x + cur_active_i * _box_side - i, _start_y + cur_active_j * _box_side);
			Fl::check();
			Fl::redraw();
			select(0, NULL, NULL, NULL, &delay);
		}
	} else if (cur_active_i - last_active_i == -1 && cur_active_j - last_active_j == 0) {
		// RIGHT-TO-LEFT
		direction = JewelLogic::RIGHT_TO_LEFT;
		for (int i = 0; i <= _box_side; i += 10) {
			_jewels[jewel1]->position(_start_x + last_active_i * _box_side - i, _start_y + last_active_j * _box_side);
			_jewels[jewel2]->position(_start_x + cur_active_i * _box_side + i, _start_y + cur_active_j * _box_side);
			Fl::check();
			Fl::redraw();
			select(0, NULL, NULL, NULL, &delay);
		}
	} else if (cur_active_i - last_active_i == 0 && cur_active_j - last_active_j == 1) {
		// UP-TO-DOWN
		direction = JewelLogic::UP_TO_DOWN;
		for (int i = 0; i <= _box_side; i += 10) {
			_jewels[jewel1]->position(_start_x + last_active_i * _box_side, _start_y + last_active_j * _box_side + i);
			_jewels[jewel2]->position(_start_x + cur_active_i * _box_side, _start_y + cur_active_j * _box_side - i);
			Fl::check();
			Fl::redraw();
			select(0, NULL, NULL, NULL, &delay);
		}
	} else if (cur_active_i - last_active_i == 0 && cur_active_j - last_active_j == -1) {
		// DOWN-TO-UP
		direction = JewelLogic::DOWN_TO_UP;
		for (int i = 0; i <= _box_side; i += 10) {
			_jewels[jewel1]->position(_start_x + last_active_i * _box_side, _start_y + last_active_j * _box_side - i);
			_jewels[jewel2]->position(_start_x + cur_active_i * _box_side, _start_y + cur_active_j * _box_side + i);
			Fl::check();
			Fl::redraw();
			select(0, NULL, NULL, NULL, &delay);
		}
	} else {
		return;
	}

	// swap the box in _jewels
	JewelBox* temp = _jewels[jewel1];
	_jewels[jewel1] = _jewels[jewel2];
	_jewels[jewel2] = temp;

	// check whether the move can eliminate jewels
	struct JewelLogic::EliminationMessage em;
	if (_logic->move_check(jewel1, jewel2, direction, &em) < 0) {
		temp = _jewels[jewel1];
		_jewels[jewel1] = _jewels[jewel2];
		_jewels[jewel2] = temp;
		switch (direction) {
			case JewelLogic::LEFT_TO_RIGHT:
			{
				for (int i = _box_side; i >= 0; i -= 10) {
					_jewels[jewel1]->position(_start_x + last_active_i * _box_side + i, _start_y + last_active_j * _box_side);
					_jewels[jewel2]->position(_start_x + cur_active_i * _box_side - i, _start_y + cur_active_j * _box_side);
					Fl::check();
					Fl::redraw();
					select(0, NULL, NULL, NULL, &delay);
				}
				break;
			}
			case JewelLogic::RIGHT_TO_LEFT:
			{
				for (int i = _box_side; i >= 0; i -= 10) {
					_jewels[jewel1]->position(_start_x + last_active_i * _box_side - i, _start_y + last_active_j * _box_side);
					_jewels[jewel2]->position(_start_x + cur_active_i * _box_side + i, _start_y + cur_active_j * _box_side);
					Fl::check();
					Fl::redraw();
					select(0, NULL, NULL, NULL, &delay);
				}
				break;
			}
			case JewelLogic::UP_TO_DOWN:
			{
				for (int i = _box_side; i >= 0; i -= 10) {
					_jewels[jewel1]->position(_start_x + last_active_i * _box_side, _start_y + last_active_j * _box_side + i);
					_jewels[jewel2]->position(_start_x + cur_active_i * _box_side, _start_y + cur_active_j * _box_side - i);
					Fl::check();
					Fl::redraw();
					select(0, NULL, NULL, NULL, &delay);
				}
				break;
			}
			case JewelLogic::DOWN_TO_UP:
			{
				for (int i = _box_side; i >= 0; i -= 10) {
					_jewels[jewel1]->position(_start_x + last_active_i * _box_side, _start_y + last_active_j * _box_side - i);
					_jewels[jewel2]->position(_start_x + cur_active_i * _box_side, _start_y + cur_active_j * _box_side + i);
					Fl::check();
					Fl::redraw();
					select(0, NULL, NULL, NULL, &delay);
				}
				break;
			}
			default:
				break;
		}
		return;
	} else {
		;
	}
}

void JewelBoard::set_hint() {
	_hint = _logic->get_hint();
	printf("HINT: %d\n", _hint);
	if (_hint >= 0) {
		_jewels[_hint]->box(FL_UP_BOX);
		Fl::check();
		Fl::redraw();
	}
}