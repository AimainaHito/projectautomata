#ifndef RK_POISSONSAMPLER_H_
#define RK_POISSONSAMPLER_H_

#include <vector>

#include "Util.h"
#include "RandomNumberGenerator.h"
#include "TwoDTree.h"

namespace automata { namespace worldgen {
	class PoissonSampler
	{
		public:
			PoissonSampler() = delete;
			PoissonSampler(int _width, int _height, int _max_radius, int _padding, int _max_candidates,
						   int _max_points) :
						   width(_width), height(_height), max_radius(_max_radius), padding(_padding),
						   max_candidates(_max_candidates), max_points(_max_points)
			{}
			~PoissonSampler() = default;

			std::vector<Circle> generate();

		private:
			int width;
			int height;
			int max_radius;
			int padding;
			int max_candidates;
			int max_points;
			RandomNumberGenerator rng;
	};
}} /* namespace automata::worldgen */

#endif /* RK_POISSONSAMPLER_H_ */
