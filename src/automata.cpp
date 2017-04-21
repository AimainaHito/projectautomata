#include <chrono>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <ncurses.h>

#include "activity.h"
#include "main_menu_activity.h"
#include "loading_activity.h"
#include "simplex_generator.h"

void initialize_curses() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr,TRUE);
}


void end_program() {
	endwin();
	exit(0);
}

std::vector<std::vector<char>> scale_down(
	const std::vector<std::vector<char>>& map, int width, int height,
	const std::vector<char>& color_vector, const std::unordered_map<char,int>& char_map
	) {

	std::vector<std::vector<char>> scaled_map(height / 2);

	for (int y = 0, y_new = 0;y < height;y += 2, y_new++) {
		scaled_map[y_new] = std::vector<char>(width / 2);
		for (int x = 0, x_new = 0;x < width;x += 2, x_new++) {
			int score = char_map.at(map[y][x]);
			int cells = 1;
			
			if (y > 0) {
				score += char_map.at(map[y - 1][x]);
				cells++;
				if (x > 0) {
					score += char_map.at(map[y - 1][x - 1]);
					cells++;
				}
				if (x < (width - 1)) {
					score += char_map.at(map[y - 1][x + 1]);
					cells++;
				}
			}
			if (y < (height - 1)) {
				score += char_map.at(map[y + 1][x]);
				cells++;
				if (x > 0) {
					score += char_map.at(map[y + 1][x - 1]);
					cells++;
				}
				if (x < (width - 1)) {
					score += char_map.at(map[y + 1][x + 1]);
					cells++;
				}
			}
			if (x > 0) {
				score += char_map.at(map[y][x - 1]);
				cells++;
			}
			if (x < (width - 1)) {
				score += char_map.at(map[y][x + 1]);
				cells++;
			}

			scaled_map[y_new][x_new] = color_vector.at(std::round(score / (float) cells));
		}
	}

	return scaled_map;
}


std::vector<std::vector<char>> read_map(const std::string& filename) {
	auto map = std::vector<std::vector<char>>();
	std::ifstream infile(filename);
	std::string line;

	int i = 0;

	while (std::getline(infile,line)) {
		map.push_back(std::vector<char>());

		for (char c : line) {
			map[i].push_back(c);
		}
		i++;
	}

	infile.close();

	return map;
}


int main() {
	int max_x,max_y;
	initialize_curses();
	getmaxyx(stdscr,max_y,max_x);
	start_color();
	init_pair(1,202,COLOR_BLACK);//orange
	init_pair(2,64,COLOR_BLACK); //dark green
	init_pair(3,50,COLOR_BLACK); //light blue
	init_pair(4,19,COLOR_BLACK); //deep blue background for spaces
	init_pair(5,220,COLOR_BLACK); //yellow
	init_pair(6,250,COLOR_BLACK); //gray
	init_pair(7,155,COLOR_BLACK); //light green
	init_pair(8,112,COLOR_BLACK); //slighly darker green
	//Cloud colors
	init_pair(9,255,COLOR_BLACK); //
	init_pair(10,254,COLOR_BLACK); //
	init_pair(11,253,COLOR_BLACK); //
	curs_set(0);
	nodelay(stdscr,true);

	const int FRAME_TIME = 1000 / 60;

	int char_code;
	bool running = true;

	std::shared_ptr<automata::activities::loading_activity> transition = std::make_shared<automata::activities::loading_activity>(max_x, max_y);
	std::shared_ptr<automata::activities::activity_observer> observer = make_observer(
		std::make_shared<automata::activities::main_menu_activity>(transition, max_x, max_y)
	);

	std::chrono::steady_clock::time_point previous_time = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point previous_fps_time = std::chrono::steady_clock::now();
	int fps_counter = 0;
	int fps_update = 0;

	while (running && (char_code = getch()) != 'q') {
		if (char_code != ERR) {
			observer->get_current_activity()->send_key_event(char_code);
		}
		
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - previous_time).count() >= FRAME_TIME) {
			observer->get_current_activity()->update();
			previous_time = std::chrono::steady_clock::now();
			mvprintw(0,0,"%2d FPS", fps_update);
			refresh();
			fps_counter++;
		}
		if (std::chrono::duration_cast<std::chrono::seconds>(current_time - previous_fps_time).count() >= 1) {
			previous_fps_time = std::chrono::steady_clock::now();
			fps_update = fps_counter;
			fps_counter = 0;
		}
	}

	observer->get_current_activity()->destroy();
	end_program();
}
