#ifndef RK_RANDOMNUMBERGENERATOR_H_
#define RK_RANDOMNUMBERGENERATOR_H_

#include <cmath>
#include <random>
#include <time.h>

#include "SDL2/SDL.h"

namespace automata { namespace worldgen {
	class RandomNumberGenerator
	{
		public:
			RandomNumberGenerator() :
				generator(std::random_device()()),
				realDistrib(0, 1)
			{
				generator.seed(time(nullptr));
			}
			RandomNumberGenerator(unsigned seed) : generator(seed), realDistrib(0, 1) {}
			~RandomNumberGenerator() = default;
			RandomNumberGenerator(const RandomNumberGenerator &) = default;
			RandomNumberGenerator(RandomNumberGenerator &&) = default;
			RandomNumberGenerator &operator=(RandomNumberGenerator &) = default;
			RandomNumberGenerator &operator=(RandomNumberGenerator &&) = default;

			//generates an int between 0 and max - 1 inclusive
			int next_int(int maxPlusOne) const
			{
				std::uniform_int_distribution<int> distr(0, maxPlusOne - 1);
				return distr(generator);
			}

			//generates an int between 0 and max inclusive in normal distribution
			int next_normal_int(int max) const
			{
				double val = next_double() + 1 / PHI();
				val -= std::floor(val);
				return (int) std::floor(val * (max + 1));
			}

			//generates a double between 0 and 1 inclusive
			double next_double() const
			{
				return realDistrib(generator);
			}

			//generate a double between min and max inclusive
			double next_double(double min, double max) const
			{
				std::uniform_real_distribution<double> distr(min, max);
				return distr(generator);
			}

			//generate a colour in normal distribution
			SDL_Color next_colour(bool opaque) const
			{
				const unsigned char r = next_normal_int(RGBA_MAX);
				const unsigned char g = next_normal_int(RGBA_MAX);
				const unsigned char b = next_normal_int(RGBA_MAX);
				const unsigned char a = opaque ? RGBA_MAX : 0;

				return {r, g, b, a};
			}

		private:
			static const unsigned char RGBA_MAX = 0xff;
			static constexpr SDL_Color WHITE = {RGBA_MAX, RGBA_MAX, RGBA_MAX, RGBA_MAX};
			static constexpr double PHI() { return (1 + std::sqrt(5)) / 2; }
			mutable std::mt19937 generator;
			mutable std::uniform_real_distribution<double> realDistrib;
	};

}} /* namespace automata::worldgen */

#endif /* RK_RANDOMNUMBERGENERATOR_H_ */
