#include <cmath>
#include "coord.h"


coord::coord(int x, int y) {
	this->x = x;
	this->y = y;
}

coord::coord(const coord& other_coord) {
	this->x = other_coord.x;
	this->y = other_coord.y;
}

bool coord::operator==(const coord& other) const {
	return this->x == other.x && this->y == other.y;
}

int coord::manhatte_distance(coord from) const {
	return std::abs((from.x - this->x) + (from.y - this->y));	
}

int coord::chebyshev_distance(coord from) const {
	return fmax(abs(from.x - this->x),abs(from.y - this->y));
}

double coord::euclidean_distance(coord from) const {
	return std::sqrt(((from.x - this->x) * (from.x - this->x)) + ((from.y - this->y) * (from.y - this->y)));
}

int coord::flat_index(int width) const {
	return (width * y) + x;
}

std::ostream& operator<<(std::ostream& strm, const coord& coord) {
	return strm << "(" << coord.x << "|" << coord.y << ")";
}

coord coord_from_index(int index, int width) {
	return coord(index % width,index / width);
}
