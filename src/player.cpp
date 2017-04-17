#include "player.h"

player::player(coord map_position, coord screen_position, char representation)
	: moving_entity(map_position, screen_position), representation(representation) {

}
