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

JewelBoard::JewelBoard(Fl_Window* win, int start_x, int start_y, int box_side) : Fl_Box(start_x, start_y, 8 * box_side, 8 * box_side, 0) {
	_win = win;
	_size = 8;
	_start_x = start_x;
	_start_y = start_y;
	_box_side = box_side;
	_last_active_i = -1;
	_last_active_j = -1;

	// load jewel images
	_jewel_images[0] = new Fl_PNG_Image("image/Jewel1.png");
	_jewel_images[1] = new Fl_PNG_Image("image/Jewel2.png");
	_jewel_images[2] = new Fl_PNG_Image("image/Jewel3.png");
	_jewel_images[3] = new Fl_PNG_Image("image/Jewel4.png");
	_jewel_images[4] = new Fl_PNG_Image("image/Jewel5.png");
	_jewel_images[5] = new Fl_PNG_Image("image/Jewel6.png");
	_jewel_images[6] = new Fl_PNG_Image("image/Jewel7.png");

	// get game logic
	_logic = new JewelLogic(this, 8, 7);

//_jewels[0] = new JewelBox(_win, start_x + box_side * 0, start_y + box_side * 0, box_side, box_side);

	// create 8*8 jewel boxes, set color
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_jewels[i + 8 * j] = new JewelBox(_win, start_x + box_side * i, start_y + box_side * j, box_side, box_side);
		}
	}

	//JewelBox* jbb = new JewelBox(_win, start_x - 60, start_y - 60, box_side, box_side);
	//jbb->image(_jewel_images[0]);

	// init the game, set color to each jewel box
	_init_jewel_color();
/*	//exit(1);*/
}

void JewelBoard::_init_jewel_color() {
	int* position = _logic->gen_init();
	/*for (int i = 0; i < 64; i++) {
		printf("INIT: %d\n", position[i]);
	}*/
	//_jewels[0]->image(_jewel_images[position[0]]);
	//_jewels[8]->image(_jewel_images[position[8]]);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_jewels[i + 8 * j]->image(_jewel_images[position[i + 8 * j]]);
		}
	}
}

int JewelBoard::handle(int event) {
	//printf("%d %d %d %d\n", event, FL_PUSH, FL_RELEASE, FL_DRAG);
	//exit(0);
	static int offset[2] = { 0, 0 };
	static int sensitive_move = _box_side / 2;
	int ret = Fl_Widget::handle(event);
	//exit(0);
	switch (event) {
		case FL_PUSH:
		{
			// will be used for dragging
			//offset[0] = Fl::event_x() - _start_x;
			//offset[1] = Fl::event_y() - _start_y;

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
		/*int ret = Fl_Box::handle(event);
		return 1;*/
}

void JewelBoard::eliminate(int* eliminate_i, int* eliminate_j, int eliminate_length, int* move_i, int* move_j, int move_length, int range) {
	// use select to delay for animation
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = 1; // 1us here, but same with about 20ms

	// eliminate the corresponding jewels
	for (int k = 0; k < eliminate_length; k++) {
		Fl::delete_widget(_jewels[eliminate_i[k] + _size * eliminate_j[k]]);
	}

	JewelBox* jbb = new JewelBox(_win, _start_x - 60, _start_y - 60, _box_side, _box_side);
	jbb->image(_jewel_images[0]);
	int current_i = eliminate_i[0];
	JewelBox* new_jewels[eliminate_length];
	for (int k = 0; k < eliminate_length; k++) {
		new_jewels[k] = new JewelBox(_win, _start_x + _box_side * current_i, _start_y + _box_side * (k + 1), _box_side, _box_side);
		printf("visible: %d\n", new_jewels[k]->visible());
		new_jewels[k]->image(_jewel_images[0]);
		printf("visible: %d\n", new_jewels[k]->visible());
		new_jewels[k]->show();
		printf("visible: %d\n", new_jewels[k]->visible());
	}
	Fl::check();
	Fl::redraw();

	// move the corresponding jewels
	for (int a = 0; a <= range; a += 1) {
		//printf("eliminate: %d %d %d %d %d %d %d %d\n", _box_side, a, i[0], j[0], i[1], j[1], i[2], j[2]);
		for (int k = 0; k < move_length; k++) {
			//printf("move: %d %d \n", _start_x + i[k] * _box_side, _start_y + j[k] * _box_side + a);
			_jewels[move_i[k] + _size * move_j[k]]->position(_start_x + move_i[k] * _box_side, _start_y + move_j[k] * _box_side + a);
		}
		Fl::check();
		Fl::redraw();
		select(0, NULL, NULL, NULL, &delay);
	}
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
		return;
	}

	//printf("%d %d %d\n", em.explode, em.horizontal, em.vertical);
}

void JewelBoard::_swap_jewel(int jewel1, int jewel2) {
	
}

/*JewelBox* JewelBoard::get_jewel_box(int i , int j) {
	return _jewels[i + 8 * j];
}*/
