#ifndef RK_UTIL_H_
#define RK_UTIL_H_

#include <cmath>

namespace automata { namespace worldgen {
	//mostly POD type
	class Point
	{
		public:
			double x, y;

			Point() : x(0), y(0) {}
			Point(double x_, double y_) : x(x_), y(y_) {}
			Point(const Point &p) : x(p.x), y(p.y) {}
			bool operator==(const Point &p) const
			{
				return x == p.x && y == p.y;
			}

			bool operator!=(const Point &p) const
			{
				return x != p.x || y != p.y;
			}

			double distance(const Point &other) const
			{
				return std::sqrt( (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) );
			}
	};
	//mostly POD type
	class Circle
	{
		public:
			Point position;
			double radius;
			Circle(const Point &p, double _radius) : position(p), radius(_radius) {}
	};
}} /* namespace automata::worldgen */

#endif /* RK_UTIL_H_ */
