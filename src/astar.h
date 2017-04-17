#ifndef ASTAR_H
#define ASTAR_H

#include<vector>
#include<set>
#include<unordered_map>
#include "coord.h"

class astar {
	private:
		std::vector<std::vector<char>> map;
		const int width,height;
		int get_minimum(const std::set<int>& open_set,const std::unordered_map<int,double>& f_score) const;
		std::vector<coord> get_neighbors(const coord& location) const;
		std::vector<coord> reconstruct_path(const coord& end,const std::unordered_map<int,int>& came_from,int width) const;

	public:
		astar(const std::vector<std::vector<char>>& map_content);
		std::tuple<std::vector<coord>,std::vector<coord>> astar_path(const coord& start,const coord& end);
};

#endif
