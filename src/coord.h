#ifndef COORD_H
#define COORD_H

#include <cmath>
#include <iostream>

namespace automata {

template <class coord_type>
class coord {
	public:
		coord_type x;
		coord_type y;

		coord(coord_type x,coord_type y); 
		coord(const coord& other_coord);
		int manhatte_distance(coord from) const;
		int chebyshev_distance(coord from) const;
		double euclidean_distance(coord from) const;
		bool operator==(const coord& other) const;

		int flat_index(int width) const;

	template <class friend_coord_type>
	friend std::ostream& operator<<(std::ostream& strm, const coord<friend_coord_type>& coord);
};

template <class coord_type>
coord<coord_type> coord_from_index(coord_type index, coord_type width);

//Implementations

template <class coord_type>
coord<coord_type>::coord(coord_type x, coord_type y) {
	this->x = x;
	this->y = y;
}

template <class coord_type>
coord<coord_type>::coord(const coord& other_coord) {
	this->x = other_coord.x;
	this->y = other_coord.y;
}

template <class coord_type>
bool coord<coord_type>::operator==(const coord& other) const {
	return this->x == other.x && this->y == other.y;
}

template <class coord_type>
int coord<coord_type>::manhatte_distance(coord from) const {
	return std::abs((from.x - this->x) + (from.y - this->y));	
}

template <class coord_type>
int coord<coord_type>::chebyshev_distance(coord from) const {
	return fmax(abs(from.x - this->x),abs(from.y - this->y));
}

template <class coord_type>
double coord<coord_type>::euclidean_distance(coord from) const {
	return std::sqrt(((from.x - this->x) * (from.x - this->x)) + ((from.y - this->y) * (from.y - this->y)));
}

template <class coord_type>
int coord<coord_type>::flat_index(int width) const {
	return (width * y) + x;
}

template <class coord_type>
std::ostream& operator<<(std::ostream& strm, const coord<coord_type>& coord) {
	return strm << "(" << coord.x << "|" << coord.y << ")";
}

template <class coord_type>
coord<coord_type> coord_from_index(coord_type index, coord_type width) {
	return coord<coord_type>(index % width,index / width);
}
}

#endif
