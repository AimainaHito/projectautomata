#ifndef MOVING_ENTITIY_H
#define MOVING_ENTITIY_H

#include "coord.h"

class moving_entity {
	private:
		coord map_position;
		coord screen_position;
	
	public:
		moving_entity(const coord& map_position, const coord& screen_position);
		const coord& get_position() const;
		void move_to(const coord& destionation);
		int get_screen_x() const;
		int get_screen_y() const;
		void set_screen_position(const coord& screen_position);
};

#endif
