#include "coord.h"
#include "astar.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cmath>


astar::astar(const std::vector<std::vector<char>>& map_content)
	: width(map_content[0].size()),height(map_content.size()) {

	map = map_content;
}

int astar::get_minimum(const std::set<int>& open_set,const std::unordered_map<int,double>& f_score) const {
	int min_f_score = std::numeric_limits<int>::max();
	int min_index = -1;

	for (int index : open_set) {
		if (f_score.count(index) > 0 && f_score.at(index) < min_f_score) {
			min_index = index;
			min_f_score = f_score.at(index);
		}
	}
	
	return min_index;
}

std::vector<coord> astar::get_neighbors(const coord& location) const {
	std::vector<coord> neighbors = std::vector<coord>();

	if (location.y > 0) {
		neighbors.push_back(coord(location.x,location.y - 1));
		if (location.y < (height - 1)) {
			neighbors.push_back(coord(location.x,location.y + 1));
		}
	}
	if (location.x > 0) {
		neighbors.push_back(coord(location.x - 1,location.y));
		if (location.y > 0) {
			neighbors.push_back(coord(location.x - 1,location.y - 1));
			if (location.y < (height - 1)) {
				neighbors.push_back(coord(location.x - 1,location.y + 1));
			}
		}
		if (location.x < (width - 1)) {
			neighbors.push_back(coord(location.x + 1,location.y));
			if (location.y > 0) {
				neighbors.push_back(coord(location.x + 1,location.y - 1));
				if (location.y < (height - 1)) {
					neighbors.push_back(coord(location.x + 1,location.y + 1));
				}
			}
		}
	}

	return neighbors;
}

std::vector<coord> astar::reconstruct_path(const coord& end,const std::unordered_map<int,int>& came_from,int width) const {
	std::vector<coord> path = {end};
	int current = end.flat_index(width);
	while (came_from.count(current) != 0) {
		current = came_from.at(current);
		path.push_back(coord_from_index(current,width));
	}

	return path;
}

std::tuple<std::vector<coord>,std::vector<coord>> astar::astar_path(const coord& start,const coord& end) {
	if (start == end) {
		return {{end},{end}};
	}	
	std::set<int> closed_set = std::set<int>();
	std::set<int> open_set = std::set<int>();
	open_set.insert(start.flat_index(width));

	std::unordered_map<int,int> came_from = std::unordered_map<int,int>();

	std::unordered_map<int,double> g_score = std::unordered_map<int,double>();
	g_score[start.flat_index(width)] = 0;

	std::unordered_map<int,double> f_score = std::unordered_map<int,double>();
	f_score[start.flat_index(width)] = start.euclidean_distance(end);

	std::vector<coord> search_space;
	
	while (!open_set.empty()) {
		int current = get_minimum(open_set,f_score);
		coord current_coord = coord_from_index(current,width);
		if (current_coord == end) {
			return {reconstruct_path(end,came_from,width),search_space};
		}
			
		open_set.erase(current);
		closed_set.insert(current);
		
		for (coord neighbor : get_neighbors(current_coord)) {
			if (map.at(neighbor.y).at(neighbor.x) != '.') {
				continue;
			}

			int neighbor_index = neighbor.flat_index(width);

			search_space.push_back(neighbor);

			if (closed_set.count(neighbor_index) != 0 || g_score.count(current) == 0) {
				continue;
			}
			
			//+1 is the distance from current to neighbor
			double tentative_g_score = g_score[current] + current_coord.euclidean_distance(neighbor);

			if (open_set.count(neighbor_index) == 0) {
				open_set.insert(neighbor_index);
			}
			else if (tentative_g_score >= g_score[neighbor_index]) {
				continue;
			}
			
			came_from[neighbor_index] = current;
			g_score[neighbor_index] = tentative_g_score;
			f_score[neighbor_index] = tentative_g_score + end.euclidean_distance(neighbor);
		}
	}

	//TODO: failure state
	return {};
}
