#include "game_board.h"
#include "jewel_box.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Button.H>
#include <iostream>
#include <FL/Fl.H>
#include <stdlib.h>
#include <stdio.h>
#include "stdlib.h"

static void set_time_label(void* vgb) {
    //printf("ENTER\n");
    GameBoard* gb = (GameBoard*) vgb;
    gb->_time--;
    if (gb->_time < 0) {
        gb->_time = 10;
    }
    sprintf(gb->_ctl, "%d", gb->_time);
    gb->_tl->label(gb->_ctl);
    Fl::repeat_timeout(1, set_time_label, vgb);
    //Fl::remove_timeout(set_time_label);
    //gb->_pop_win = new Fl_Menu_Window(200, 100);
    //gb->pop_memu_window();
}

static void set_hint_button(Fl_Widget*, void* vgb) {
    GameBoard* gb = (GameBoard*) vgb;
    gb->set_hint();
}

GameBoard::GameBoard(int height, int width) : Fl_Window(height, width, "BEJEWELED") {
    //Fl_Window *gb = new Fl_Window(height, width, "BEJEWELED");
    /*JewelBox* box = new JewelBox(gb, 20, 40, 260, 100);
    box->box(FL_UP_BOX);
    box->labelsize(36);
    box->labelfont(FL_BOLD + FL_ITALIC);
    Fl_PNG_Image* image = new Fl_PNG_Image("Jewel1.png");
    box->image(image);
    */
    //JewelBoard* jb = new JewelBoard(gb, 200, 100, 60);
    //_win = new Fl_Window(height, width, "BEJEWELED");
    //_jb = new JewelBoard(gb, 200, 100, 60);
    Fl_Color c = fl_rgb_color(255, 80, 156);
    /*Fl_JPEG_Image**/ background = new Fl_JPEG_Image("image/back_1.jpg");
    Fl_Box* b = new Fl_Box(0, 0, 800, 600);
    b->box(FL_NO_BOX);
    b->image(background);
    //this->image(background);
    //background->draw(200, 200, 500, 400, 0, 0);
    _tl = new Fl_Box(FL_DOWN_BOX, 100, 150, 60, 25, "10");
    _sl = new Fl_Box(FL_DOWN_BOX, 100, 200, 60, 25, "0");
    Fl_PNG_Image* label = new Fl_PNG_Image("image/button_l.png");
    //_tl->image(label);
    //_sl->image(label);
    _tl->color(c);
    _sl->color(c);
    _time = 10;
    Fl::add_timeout(1, set_time_label, this);
    _jb = new JewelBoard(this, 200, 100, 60);
    //_jb->image(background);
    Fl_Button* hb = new Fl_Button(100, 300, 60, 60);
    Fl_PNG_Image* hint_image = new Fl_PNG_Image("image/hint_l.png");
    hb->callback(set_hint_button, this);
    hb->image(hint_image);
    //std::cout << "here" << std::endl;
    /*gb->*/end();
    //std::cout << "here" << std::endl;
    /*gb->*/show();
    //std::cout << "here" << std::endl;
    Fl::run();
}

void GameBoard::set_hint() {
    _jb->set_hint();
}

void GameBoard::pop_memu_window() {
    _win->insert(*_pop_win, 100);
}

void GameBoard::set_score_label(int score) {
    sprintf(_csl, "%d", score);
    _sl->label(_csl);
    _sl->show();
    Fl::check();
    Fl::redraw();
    printf("SCORE:%d\n", score);
}