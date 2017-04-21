#ifndef LOCAL_MAP_ACTIVITY_H
#define LOCAL_MAP_ACTIVITY_H

#include <vector>
#include <unordered_map>

#include "activity.h"
#include "player.h"

namespace automata {
namespace activities {
	class local_map_activity : public activity {
		private:
			int x_offset, y_offset;
			int current_x, current_y;
			bool map_moved, player_moved;
			player<int, int> player_character;

			std::vector<std::vector<char>> map;
			std::vector<std::vector<bool>> traversable;

			std::unordered_map<char,int> color_map;
			std::vector<char> color_vector;
			std::unordered_map<char,int> char_map;

			const int map_width = 512;
			const int map_height = 512;

			void draw_map();

		public:
			local_map_activity(int max_x, int max_y);

			void start(std::shared_ptr<activity_observer> observer) override;
			void destroy() override;
			void redraw() override;
			void update() override;
			void send_key_event(int char_code) override;
	};
}
}

#endif
