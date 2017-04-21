#include "menu_window.h"

automata::menu::menu(int start_x,int start_y,int width,int height,std::vector<std::string> entries) {
	win = newwin(width,height,start_x,start_y);
	index = 0;

	this->start_x = start_x;
	this->start_y = start_y;
	this->width = width;
	this->height = height;
	this->end_x = start_x + width;
	this->end_y = start_y + height;
	this->entries = entries;
}

automata::menu::~menu() {
	delwin(win);
}

bool automata::menu::up() {
	if (index < (entries.size() - 1)) {
		index++;
		return true;
	}
	return false;
}

bool automata::menu::down() {
	if (index > 0) {
		index--;
		return true;
	}
	return false;
}

unsigned int automata::menu::get_index() {
	return index;
}

void automata::menu::clear() {
	wclear(win);
}
