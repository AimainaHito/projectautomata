#include <curses.h>

#include "loading_activity.h"

loading_activity::loading_activity(int max_x, int max_y) : activity(max_x, max_y) {

}

void loading_activity::start(std::shared_ptr<activity_observer> observer) {
	this->observer = observer;
	this->observer->set_current_activity(shared_from_this());
	redraw();
}

void loading_activity::redraw() {
	clear();
	mvprintw(max_y / 2,(max_x / 2) - 5,"Loading...");
	refresh();
}

void loading_activity::update() {

}

void loading_activity::send_key_event(int) {

}

void loading_activity::destroy() {
	this->observer = NULL;
}
