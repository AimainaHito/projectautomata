#ifndef MOVING_ENTITIY_H
#define MOVING_ENTITIY_H

#include "coord.h"

namespace automata {
template <class map_type, class screen_type>
class moving_entity {
	private:
		coord<map_type> map_position;
		coord<screen_type> screen_position;
	
	public:
		moving_entity(const coord<map_type>& map_position, const coord<screen_type>& screen_position);
		const coord<map_type>& get_position() const;
		void move_to(const coord<map_type>& destination);
		screen_type get_screen_x() const;
		screen_type get_screen_y() const;
		void set_screen_position(const coord<screen_type>& screen_position);
};

template <class map_type, class screen_type>
moving_entity<map_type, screen_type>::moving_entity(const coord<map_type>& map_position, const coord<screen_type>& screen_position)
	: map_position(map_position), screen_position(screen_position) {

}

template <class map_type, class screen_type>
const coord<map_type>& moving_entity<map_type, screen_type>::get_position() const {
	return map_position;
}

template <class map_type, class screen_type>
screen_type moving_entity<map_type, screen_type>::get_screen_x() const {
	return screen_position.x;
}

template <class map_type, class screen_type>
screen_type moving_entity<map_type, screen_type>::get_screen_y() const {
	return screen_position.y;
}

template <class map_type, class screen_type>
void moving_entity<map_type, screen_type>::move_to(const coord<map_type>& destination) {
	this->map_position = destination;
}

template <class map_type, class screen_type>
void moving_entity<map_type, screen_type>::set_screen_position(const coord<screen_type>& screen_position) {
	this->screen_position = screen_position;
}
}

#endif
