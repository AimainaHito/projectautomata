#ifndef MENU_WINDOW_H
#define MENU_WINDOW_H

#include <vector>
#include <string>
#include <ncurses.h>

namespace automata {
class menu {
	private:
		WINDOW* win;
		unsigned int index;
		int width,height;
		int start_x,start_y,end_x,end_y;
		std::vector<std::string> entries;

	public:
		menu(int start_x,int start_y,int width,int height,std::vector<std::string> entries);
		~menu();

		void clear();
		void draw();

		bool down();
		bool up();
		unsigned int get_index();
};
}

#endif
