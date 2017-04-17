#ifndef PLAYER_H
#define PLAYER_H

#include "moving_entity.h"

class player : public moving_entity {
	private:
		int x, y;

	public:
		const char representation;

		player(coord map_position, coord screen_position, char character);
};

#endif
