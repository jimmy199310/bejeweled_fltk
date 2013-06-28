#include "game_board.h"
#include "jewel_box.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <iostream>
#include <FL/Fl.H>

GameBoard::GameBoard(int height, int width) {
    Fl_Window *gb = new Fl_Window(height, width, "BEJEWELED");
    /*JewelBox* box = new JewelBox(gb, 20, 40, 260, 100);
    box->box(FL_UP_BOX);
    box->labelsize(36);
    box->labelfont(FL_BOLD + FL_ITALIC);
    Fl_PNG_Image* image = new Fl_PNG_Image("Jewel1.png");
    box->image(image);
    */JewelBoard* jb = new JewelBoard(gb, 200, 100, 60);
    //std::cout << "here" << std::endl;
    gb->end();
    //std::cout << "here" << std::endl;
    gb->show();
    //std::cout << "here" << std::endl;
    Fl::run();
}