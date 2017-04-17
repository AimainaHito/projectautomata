#ifndef COORD_H
#define COORD_H

#include <iostream>

class coord {
	public:
		int x;
		int y;

		coord(int x,int y); 
		coord(const coord& other_coord);
		int manhatte_distance(coord from) const;
		int chebyshev_distance(coord from) const;
		double euclidean_distance(coord from) const;
		bool operator==(const coord& other) const;

		int flat_index(int width) const;

		friend std::ostream& operator<<(std::ostream& strm,const coord& coord);
};

coord coord_from_index(int index,int width);

#endif
