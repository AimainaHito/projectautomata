#include <algorithm>
#include "PoissonSampler.h"
#include <iostream>

namespace RK
{
	std::vector<Circle> PoissonSampler::generate()
	{
		TwoDTree tree;
		std::vector<Circle> circles;
		circles.reserve(max_points);

		for (int i = 0; i < max_points; ++i)
		{
			double best_x, best_y, best_distance = 0;
			for (int ci = 0; ci < max_candidates || best_distance < padding; ++ci)
			{
				double x = rng.next_double(0, width);
				double y = rng.next_double(0, height);
				double min_distance = max_radius;

				std::function<bool(std::shared_ptr<Node>)> callback = [&](std::shared_ptr<Node> n) -> bool {
					const auto &circle_position = n->get_circle()->position;
					auto distance_x = x - circle_position.x;
					auto distance_y = y - circle_position.y;
					auto distance_sq = distance_x * distance_x + distance_y * distance_y;
					auto radius_sq = n->get_circle()->radius * n->get_circle()->radius;
					if (distance_sq < radius_sq)
					{
						min_distance = 0;
						return true;
					}
					auto distance = std::sqrt(distance_sq) - n->get_circle()->radius;
					if (distance < min_distance)
					{
						min_distance = distance;
					}
					return !min_distance;
				};
				tree.visit(callback);
				if (min_distance > best_distance)
				{
					best_x = x;
					best_y = y;
					best_distance = min_distance;
				}
			}
			Circle best_candidate{ Point{best_x, best_y}, best_distance - padding };
			tree.insert(best_candidate);
			circles.push_back(best_candidate);
			std::cout << "Finishing iteration " << i << " out of " << max_points << std::endl;
		}

		return circles;
	}
} /* namespace RK */
