#ifndef MAIN_MENU_ACTIVITY_H
#define MAIN_MENU_ACTIVITY_H

#include <vector>
#include <unordered_map>

#include "activity.h"
#include "simplex_noise.h"

class main_menu_activity : public activity {
	private:
		std::shared_ptr<activity> transition;
		std::vector<std::vector<char>> map;
		std::vector<std::vector<char>> cloud_left;
		std::vector<std::vector<char>> cloud_right;
		double cloud_movement_offset, cloud_gen_offset;
		simplex_noise cloud_left_generator, cloud_right_generator;
		int island_offset_x, island_offset_y;

		std::unordered_map<char,int> color_map;

	public:
		main_menu_activity(std::shared_ptr<activity> transition, int max_x, int max_y);

		void start(std::shared_ptr<activity_observer> observer) override;
		void destroy() override;
		void redraw() override;
		void update() override;
		void send_key_event(int char_code) override;
};

#endif
