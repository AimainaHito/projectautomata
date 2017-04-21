#include <curses.h>

#include "loading_activity.h"

automata::activities::loading_activity::loading_activity(int max_x, int max_y) : activity(max_x, max_y) {

}

void automata::activities::loading_activity::start(std::shared_ptr<activity_observer> observer) {
	this->observer = observer;
	this->observer->set_current_activity(shared_from_this());
	redraw();
}

void automata::activities::loading_activity::redraw() {
	clear();
	mvprintw(max_y / 2,(max_x / 2) - 5,"Loading...");
	refresh();
}

void automata::activities::loading_activity::update() {

}

void automata::activities::loading_activity::send_key_event(int) {

}

void automata::activities::loading_activity::destroy() {
	this->observer = NULL;
}
