#include <memory>

#include "activity.h"


automata::activities::activity::activity(int max_x, int max_y) {
	this->max_x = max_x;
	this->max_y = max_y;
}

std::shared_ptr<automata::activities::activity> automata::activities::activity_observer::get_current_activity() const {
	return current_activity;
}

void automata::activities::activity_observer::set_current_activity(std::shared_ptr<activities::activity> current_activity) {
	this->current_activity = current_activity;
}

void automata::activities::activity_observer::start_activity(
	std::shared_ptr<activities::activity> next,
	std::shared_ptr<activities::activity> transition,
	std::shared_ptr<activities::activity_observer> observer
) {
	observer->get_current_activity()->destroy();
	transition->start(observer);
	next->start(observer);
	transition->destroy();
}

std::shared_ptr<automata::activities::activity_observer> automata::activities::make_observer(std::shared_ptr<activities::activity> start_activity) {
	std::shared_ptr<activities::activity_observer> observer = std::make_shared<activities::activity_observer>();
	start_activity->start(observer);
	return observer;
}
