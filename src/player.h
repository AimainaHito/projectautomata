#ifndef PLAYER_H
#define PLAYER_H

#include "moving_entity.h"

namespace automata {
template <class map_type, class screen_type>
class player : public moving_entity<map_type, screen_type> {
	private:
		map_type x, y;

	public:
		const char representation;

		player(coord<map_type> map_position, coord<screen_type> screen_position, char character);
};


template <class map_type, class screen_type>
player<map_type, screen_type>::player(coord<map_type> map_position, coord<screen_type> screen_position, char representation)
	: moving_entity<map_type, screen_type>(map_position, screen_position), representation(representation) {

}
}

#endif
