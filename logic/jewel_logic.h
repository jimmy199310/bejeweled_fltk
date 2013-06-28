
class JewelLogic {
public:
	JewelLogic(int board_size, int color_size) : _board_size(board_size), _color_size(color_size) {};
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
	int move_check(int jewel1, int jewel2, move_direction direction, EliminationMessage* em);
private:
	int _board_size;
	int _color_size;
	int* _position;
};