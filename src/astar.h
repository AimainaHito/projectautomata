#ifndef ASTAR_H
#define ASTAR_H

#include<vector>
#include<set>
#include<unordered_map>
#include "coord.h"

namespace automata {
namespace algorithms {
	class astar {
		private:
			std::vector<std::vector<char>> map;
			const int width,height;
			int get_minimum(const std::set<int>& open_set,const std::unordered_map<int,double>& f_score) const;
			std::vector<coord<int>> get_neighbors(const coord<int>& location) const;
			std::vector<coord<int>> reconstruct_path(const coord<int>& end,const std::unordered_map<int,int>& came_from,int width) const;

		public:
			astar(const std::vector<std::vector<char>>& map_content);
			std::tuple<std::vector<coord<int>>,std::vector<coord<int>>> astar_path(const coord<int>& start,const coord<int>& end);
	};
}
}

#endif
