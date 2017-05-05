#include <memory>
#include <vector>
#include <unordered_map>
#include <curses.h>

#include "player.h"
#include "simplex_generator.h"
#include "local_map_activity.h"

automata::activities::local_map_activity::local_map_activity(int max_x, int max_y)
	: activity(max_x, max_y), player_character(player<int>(coord<int>(0,0), coord<int>(0,0), '@')) {

	current_x = current_y = 0;
	x_offset = y_offset = 0;

	map_moved = false;
	player_moved = false;

	color_map = {{'.', 5},{'^', 6}, {' ', 4}, {'o', 2}, {',', 7}, {'~',3}, {';',8}}; //{'~', 4}
	color_vector = {' ','.',',',';','o','^','~'};
	char_map = {{' ',0},{'.',1},{',',2},{';',3},{'o',4},{'^',5},{'~',6}};
}

void automata::activities::local_map_activity::redraw() {
	for (int y = 0;y < max_y;y++) {
		for (int x = 0;x < max_x;x++) {
			mvaddch(y, x, map[y + y_offset][x + x_offset] | COLOR_PAIR(color_map.at(map[y + y_offset][x + x_offset])));
		}
	}
}

void automata::activities::local_map_activity::update() {
	if (map_moved || player_moved) {
		if (map_moved) {
			redraw();
			map_moved = false;
		}

		if (player_moved) {
			player_moved = false;

			mvaddch(
				player_character.get_screen_y(),
				player_character.get_screen_x(),
				map[player_character.get_screen_y() + y_offset][player_character.get_screen_x() + x_offset]
				| COLOR_PAIR(
					color_map[
						map[player_character.get_screen_y() + y_offset][player_character.get_screen_x() + x_offset]
					]
					)
			);

			player_character.set_screen_position(coord<int>(current_x, current_y));
		}


		mvaddch(
			player_character.get_screen_y(),
			player_character.get_screen_x(),
			player_character.representation | COLOR_PAIR(1)
		);

		move(current_y, current_x);
	}
}

void automata::activities::local_map_activity::start(std::shared_ptr<activity_observer> observer) {
	this->observer = observer;
	this->observer->set_current_activity(shared_from_this());

	map = worldgen::generate_island(map_width, map_height);
	traversable = std::vector<std::vector<bool>>(map_height);

	for (int y = 0;y < map_height;y++) {
		traversable[y] = std::vector<bool>(map_width);
		for (int x = 0;x < map_width;x++) {
			traversable[y][x] = map[y][x] == ' ';
		}
	}

	redraw();
	mvaddch(player_character.get_screen_y(), player_character.get_screen_x(),player_character.representation | COLOR_PAIR(1));

	move(0,0);
	refresh();
}

void automata::activities::local_map_activity::send_key_event(int char_code) {
	switch(char_code) {
		case 'w':
			if (current_y > 0 && map[player_character.get_position().y - 1][player_character.get_position().x] == ' ') {
				player_character.move_to(
					coord<int>(
						player_character.get_position().x,
						player_character.get_position().y - 1
					)
				);
				if (current_y > (max_y / 2) || y_offset == 0) {
					current_y--;
					player_moved = true;
				}
				if (!player_moved && y_offset > 0 && current_y == (max_y / 2)) {
					y_offset--;
					map_moved = true;
				}
			}
			break;
		case 's':
			if (current_y < (max_y - 1) && map[player_character.get_position().y + 1][player_character.get_position().x] == ' ') {
				player_character.move_to(
					coord<int>(
						player_character.get_position().x,
						player_character.get_position().y + 1
					)
				);
				if (current_y < (max_y / 2) || (max_y + y_offset) == map_height) {
					current_y++;
					player_moved = true;
				}
				map_moved = true;
				if (!player_moved && (max_y + y_offset) < map_height && current_y == (max_y / 2)) {
					y_offset++;
					map_moved = true;
				}
			}
			break;
		case 'a':
			if (current_x > 0 && map[player_character.get_position().y][player_character.get_position().x - 1] == ' ') {
				player_character.move_to(
					coord<int>(
						player_character.get_position().x - 1,
						player_character.get_position().y
					)
				);
				if (current_x > (max_x / 2) || x_offset == 0) {
					current_x--;
					player_moved = true;
				}
				map_moved = true;
				if (!player_moved && x_offset > 0 && current_x == (max_x / 2)) {
					x_offset--;
					map_moved = true;
				}
			}
			break;
		case 'd':
			if (current_x < (max_x - 1) && map[player_character.get_position().y][player_character.get_position().x + 1] == ' ') {
				player_character.move_to(
					coord<int>(
						player_character.get_position().x + 1,
						player_character.get_position().y
					)
				);
				if (current_x < (max_x / 2) || (max_x + x_offset) == map_width) {
					current_x++;
					player_moved = true;
				}
				map_moved = true;
				if (!player_moved && (max_x + x_offset) < map_width && current_x == (max_x / 2)) {
					x_offset++;
					map_moved = true;
				}
			}
			break;
	}
}

void automata::activities::local_map_activity::destroy() {
	clear();
	observer = NULL;
}
