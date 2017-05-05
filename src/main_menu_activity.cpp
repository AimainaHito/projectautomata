#include <random>
#include <ncurses.h>

#include "activity.h"
#include "main_menu_activity.h"
#include "local_map_activity.h"
#include "simplex_generator.h"
#include "simplex_noise.h"

automata::activities::main_menu_activity::main_menu_activity(std::shared_ptr<activity> transition, int max_x, int max_y) : activity(max_x, max_y) {
	this->transition = transition;
	cloud_movement_offset = (max_x / 4);
	cloud_gen_offset = 0;
	island_offset_x = 128 - (max_x / 2);
	island_offset_y = 128 - (max_y / 2);

	
	//generate permutation table with random seed
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(0,std::numeric_limits<int>::max());
	cloud_left_generator = worldgen::simplex_noise(distribution(rng));
	cloud_right_generator = worldgen::simplex_noise(distribution(rng));

	color_map = {{'.', 5},{'^', 6}, {' ', 4}, {'o', 2}, {',', 7}, {';',8}, {'+',10}, {'~',9}, {'*',11}}; //{'~', 4}
}

void automata::activities::main_menu_activity::start(std::shared_ptr<activity_observer> observer) {
	this->observer = observer;
	this->observer->set_current_activity(shared_from_this());
	
	map = worldgen::generate_island(256,256);
	cloud_left = worldgen::generate_cloud2d(max_x,max_y*1.5,cloud_left_generator,0);
	cloud_right = worldgen::generate_cloud2d(max_x,max_y*1.5,cloud_right_generator,0);

	redraw();
}

void automata::activities::main_menu_activity::redraw() {
	clear();
}

void automata::activities::main_menu_activity::update() {
	if (max_x - cloud_movement_offset >= 0) {
		clear();
		for (int y_screen = 0, y = island_offset_y;y_screen < max_y;y_screen++, y++) {
			for (int x_screen = 0, x = island_offset_x;x_screen < max_x;x_screen++, x++) {
				mvaddch(y_screen, x_screen, map[y][x] | COLOR_PAIR(color_map[map[y][x]]));
			}
		}

		cloud_left = worldgen::generate_cloud2d(max_x,max_y*1.5,cloud_left_generator,cloud_gen_offset);
		cloud_right = worldgen::generate_cloud2d(max_x,max_y*1.5,cloud_right_generator,cloud_gen_offset);

		for (int y = 0;y < max_y;y++) {
			for (int x = cloud_movement_offset,x_screen = 0;x < max_x;x++, x_screen++) { //loop until cloud_left x_size
				char current_char = cloud_left.at((y * max_x) + x);
				if (current_char != ' ') {
					mvaddch(y, x_screen, current_char | COLOR_PAIR(color_map[current_char]));
				}
			}
		}
		for (int y = 0;y < max_y;y++) {
			for (int x_screen = cloud_movement_offset,x = 0;x_screen < max_x;x_screen++, x++) {
				char current_char = cloud_right.at((y * max_x) + x);
				if (current_char != ' ') {
					mvaddch(y, x_screen, current_char | COLOR_PAIR(color_map[current_char]));
				}
			}
		}

		cloud_gen_offset += .001;
		cloud_movement_offset += .6;
		//cloud_left = worldgen::generate_cloud(max_x,max_y*1.5,cloud_left_generator,-noise_offset);
		//cloud_right = worldgen::generate_cloud(max_x,max_y*1.5,cloud_right_generator,noise_offset);
	}
	else {
		mvprintw(max_y / 2, (max_x / 2) - 5, "Main  Menu");
	}
}

void automata::activities::main_menu_activity::send_key_event(int) {
	observer->start_activity(std::make_shared<local_map_activity>(max_x, max_y),transition,observer);
}

void automata::activities::main_menu_activity::destroy() {
	observer = NULL;
}
