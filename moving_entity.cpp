#include "moving_entity.h"


moving_entity::moving_entity(const coord& map_position, const coord& screen_position)
	: map_position(map_position), screen_position(screen_position) {

}

const coord& moving_entity::get_position() const {
	return map_position;
}

int moving_entity::get_screen_x() const {
	return screen_position.x;
}

int moving_entity::get_screen_y() const {
	return screen_position.y;
}

void moving_entity::move_to(const coord& destination) {
	this->map_position = destination;
}

void moving_entity::set_screen_position(const coord& screen_position) {
	this->screen_position = screen_position;
}
