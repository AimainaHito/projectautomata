#ifndef SIMPLEX_GENERATOR_H
#define SIMPLEX_GENERATOR_H

#include <vector>
#include <string>

#include "simplex_noise.h"

namespace automata {
namespace worldgen {
	const int noise_min_value = 0;
	const int noise_max_value = 255;

	class cloud_generator {
		private:
			std::vector<std::vector<bool>> transition;
			std::vector<std::vector<bool>> humidity;
			std::vector<std::vector<std::vector<bool>>> cloud;
			std::vector<std::vector<int>> cloud_weights;
			std::mt19937 rng;
			std::bernoulli_distribution p_ext;
			std::bernoulli_distribution p_hum;
			std::bernoulli_distribution p_trans;

			int smoothing_radius;
			int time;
			
			void initialize();
			bool cell_transitions(int x, int y);
		public:
			const int width, height;

			cloud_generator(int width, int height, int smoothing_radius);
			void simulate();
			std::vector<std::vector<char>> get_cloud() const;
	};

	void write_map(const std::vector<std::vector<char>>& map,const std::string& filename);

	std::vector<std::vector<char>> fit_to_bounds(const std::vector<std::vector<char>>& map);

	std::vector<std::vector<std::vector<char>>> generate_cloud3d(
		const int width, const int height, const simplex_noise& generator, const double offset
	);

	std::vector<std::vector<char>> generate_cloud2d(
		const int width, const int height, const simplex_noise& generator, const double offset
	);

	std::vector<std::vector<char>> generate_island(const int width, const int height);
}
}

#endif
