#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include <tuple>
#include <set>
#include <fstream>
#include <iostream>
#include <random>

#include "simplex_noise.h"
#include "simplex_generator.h"


void worldgen::cloud_generator::initialize() {
	//std::bernoulli_distribution distribution(0.5);
	simplex_noise humidity_generator(200);
	simplex_noise transition_generator(500);

	//initialize clouds to 0
	for (int y = 0;y < height;y++) {
		cloud[time][y] = std::vector<bool>(width);
		cloud_weights[y] = std::vector<int>(width);
		humidity[y] = std::vector<bool>(width);
		transition[y] = std::vector<bool>(width);

		for (int x = 0;x < width;x++) {
			cloud[time][y][x] = false;
			cloud_weights[y][x] = 0;
			humidity[y][x] = humidity_generator.scaled_octave_noise_2d(
				2,0.5,2,
				0,10,(double) x / width,(double) y / height
			) > 2;
			transition[y][x] = transition_generator.scaled_octave_noise_2d(
				2,0.5,2,
				0,10,(double) x / width,(double) y / height
			) > 4;
		}
	}
}

worldgen::cloud_generator::cloud_generator(int width, int height, int smoothing_radius) : width(width), height(height) {
	transition = std::vector<std::vector<bool>>(height);
	humidity = std::vector<std::vector<bool>>(height);
	cloud_weights = std::vector<std::vector<int>>(height);

	time = 0;
	cloud = std::vector<std::vector<std::vector<bool>>>(smoothing_radius + 1);
	cloud[time] = std::vector<std::vector<bool>>(height);

	rng.seed(400);
	p_ext = std::bernoulli_distribution(0.01);
	p_hum = std::bernoulli_distribution(0.2);
	p_trans = std::bernoulli_distribution(0.05);

	initialize();
}

bool worldgen::cloud_generator::cell_transitions(int x, int y) {
	return (x < (width - 1) && (transition[y][x + 1] || (x < (width - 2) && transition[y][x + 2])))
		|| (y < (height - 1) && (transition[y + 1][x] || (y < (height - 2) && transition[y + 2][x])))
		|| (x > 0 && (transition[y][x - 1] || (x > 1 && transition[y][x - 2])))
		|| (y > 0 && (transition[y - 1][x] || (y > 1 && transition[y - 2][x])));
}

std::vector<std::vector<char>> worldgen::cloud_generator::get_cloud() const {
	std::vector<std::vector<char>> cloud(height);
	for (int y = 0;y < height;y++) {
		cloud[y] = std::vector<char>(width);
		for (int x = 0;x < width;x++) {
			char terrain;
			
			//TODO: adjust char weights
			if (cloud_weights[y][x] < 10) {
				terrain = ' ';
			}
			else if (cloud_weights[y][x] < 20) {
				terrain = '~';
			}
			else {
				terrain = '*';
			}

			cloud[y][x] = terrain;
		}
	}
	return cloud;
}

void worldgen::cloud_generator::simulate() {
	cloud[(time + 1) % 3] = std::vector<std::vector<bool>>(height);
	for (int y = 0;y < height;y++) {
		cloud[(time + 1) % 3][y] = std::vector<bool>(width);
		for (int x = 0;x < width;x++) {
			//creation
			humidity[y][x] = humidity[y][x] && !transition[y][x];
			cloud[(time + 1) % 3][y][x] = cloud[time][y][x] || transition[y][x];
			transition[y][x] = !transition[y][x] && humidity[y][x] && cell_transitions(y, x);

			//extinction
			cloud[(time + 1) % 3][y][x] = cloud[time][y][x] && p_ext(rng);
			humidity[y][x] = humidity[y][x] || p_hum(rng);
			transition[y][x] = transition[y][x] || p_trans(rng);

			//TODO: add weighting
			int weight = (1 / (((2 * smoothing_radius) + 1) * ((2 * smoothing_radius) + 1) * ((2 * smoothing_radius) + 1)));
			int sum = 0;
			for (int t = -smoothing_radius;t <= 0;t++) {
				if (time - smoothing_radius < 0) continue;
				for (int y_offset = -smoothing_radius;y_offset <= smoothing_radius;y_offset++) {
					if (y_offset + y < 0 || y_offset + y >= height) continue;
					for (int x_offset = -smoothing_radius;x_offset <= smoothing_radius;x_offset++) {
						if (x_offset + x < 0 || x_offset + x >= width) continue;
						
						if (cloud[(time + t) % 3][y + y_offset][x + x_offset]) {
							sum += 10;
						}
					}
				}
			}

			//TODO: multiply by validation function
			cloud_weights[y][x] = (weight * sum);
		}
	}
	time++;
}

/*
 * Creates a 3D cloud with depth equal to height for rotation over a 2d plane
 */
std::vector<std::vector<std::vector<char>>> worldgen::generate_cloud3d(
		const int width, const int height, const simplex_noise& generator, const double offset) {

	const float width_to_center = std::min(width, height);
	const int center_y = (height / 2) - 1;
	const int center_x = (width / 2) - 1;

	std::vector<std::vector<std::vector<char>>> cloud_map(height);

	const double scale_factor = 3.5;
	const size_t octaves = 4;

	for (int z = 0;z < height;z++) {
		cloud_map[z] = std::vector<std::vector<char>>(height);
		for (int y = 0;y < height;y++) {
			cloud_map[z][y] = std::vector<char>(width);
			for (int x = 0;x < width;x++) {
				int noise = generator.scaled_octave_noise_3d(
					octaves,0.5,scale_factor,
					worldgen::min_value,worldgen::max_value,
					((double) x / width) + offset,((double) y / height) + offset,((double ) z / height) + offset
				);

				//manhatten distance:
				//double distance_from_center = (abs(center_x - (int) x) + abs(center_y - (int) y)) / width_to_center;
				//double distance_from_center = (abs(center_x - x) + abs(center_y - y) + abs(center_y - z)) / width_to_center;
				//euclidean distance:
				double distance_from_center = sqrt(((center_x - x) * (center_x - x)) + ((center_y - y) * (center_y - y))) / width_to_center;

				double mask = (int) (noise - (distance_from_center * worldgen::max_value));

				char terrain;

				if (mask < 10) {
					terrain = ' ';
				}
				else if (mask < 30) {
					terrain = '~';
				}
				else if (mask < 100) {
					terrain = '+';
				}
				else {
					terrain = '*';
				}

				cloud_map[z][y][x] = terrain;
			}
		}
	}
	
	return cloud_map;
}

std::vector<std::vector<char>> worldgen::generate_cloud2d(
		const int width, const int height, const simplex_noise& generator, const double offset) {

	const float width_to_center = std::min(width, height);
	const int center_y = (height / 2) - 1;
	const int center_x = (width / 2) - 1;

	std::vector<std::vector<char>> cloud_map(height);

	const double scale_factor = 3.5;
	const size_t octaves = 4;

	for (int y = 0;y < height;y++) {
		cloud_map[y] = std::vector<char>(width);
		for (int x = 0;x < width;x++) {
			int noise = generator.scaled_octave_noise_2d(
				octaves,0.5,scale_factor,
				worldgen::min_value,worldgen::max_value,
				((double) x / width) + offset,((double) y / height) + offset
			);

			//manhatten distance:
			//double distance_from_center = (abs(center_x - (int) x) + abs(center_y - (int) y)) / width_to_center;
			//euclidean distance:
			double distance_from_center = sqrt(((center_x - x) * (center_x - x)) + ((center_y - y) * (center_y - y))) / width_to_center;

			double mask = (int) (noise - (distance_from_center * worldgen::max_value));

			char terrain;

			if (mask < 10) {
				terrain = ' ';
			}
			else if (mask < 30) {
				terrain = '~';
			}
			else if (mask < 100) {
				terrain = '+';
			}
			else {
				terrain = '*';
			}

			cloud_map[y][x] = terrain;
		}
	}
	
	return cloud_map;
}

std::vector<std::vector<char>> worldgen::generate_island(const int width, const int height) {
	const float width_to_center = std::min(width, height);
	const double edge_multiplier = 1.6; //1.8 for manhatten
	const double scale_factor = 2.2; //2.2
	const size_t octaves = 8;


	const int center_y = (height / 2) - 1;
	const int center_x = (width / 2) - 1;

	//generate permutation table with random seed
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(0,std::numeric_limits<int>::max());
		
	int seed = distribution(rng);//debug seed: 968258021
	rng.seed(seed);
	simplex_noise generator = simplex_noise(seed);

	std::vector<std::vector<char>> map = std::vector<std::vector<char>>(height);
	std::uniform_int_distribution<std::mt19937::result_type> grass_distribution(40,70);
	std::uniform_int_distribution<std::mt19937::result_type> tree_distribution(70,115);

	for (int y = 0;y < height;y++) {
		map[y] = std::vector<char>(width);
		for (int x = 0;x < width;x++) {
			int noise = generator.scaled_octave_noise_2d(
				octaves,0.5,scale_factor,worldgen::min_value,worldgen::max_value,(double) x / width,(double) y / height
			);

			//manhatten distance:
			//double distance_from_center = fmax(abs(center_x - (int) x),abs(center_y - (int) y)) / width_to_center;
			//euclidean distance:
			double distance_from_center = sqrt(((center_x - x) * (center_x - x)) + ((center_y - y) * (center_y - y))) / width_to_center;

			double mask = (int) (noise - (edge_multiplier * (distance_from_center * worldgen::max_value)));
			
			char terrain;
			//int r = 0;
			//int g = 0;
			//int b = 0;
			if (mask < 10) { // < 5
				terrain = ' ';
				//g = 128;
				//b = 255;
				//g = 120;
				//b = 120;
			}
			//else if (mask < 15) {
				//terrain = '-';

				//g = (mask / 15.0) * 255;
				//b = (mask / 15.0) * 255;
				//if (g < 120) {
				//	g = 120;
				//	b = 120;
				//}
			//}
			else if (mask < 30) {
				terrain = '.';
				//r = 255 - ((mask / 35.0) * 80);
				//g = 255 - ((mask / 35.0) * 80);
			}
			else if (mask < 70) {
				int vegetation_seed = grass_distribution(rng);
				if (mask > vegetation_seed) {
					terrain = ';';
				}
				else {
					terrain = ',';
				}
				//r = 255 - ((mask / 70.0) * 102);
				//g = 255 - ((mask / 70.0) * 100);
			}
			else if (mask < 120) {
				int vegetation_seed = tree_distribution(rng);
				if (mask < vegetation_seed) {
					terrain = ';';
				}
				else {
					terrain = 'o';
				}
				//g = 255 - ((mask / 120.0) * 200);
			}
			else {
				terrain = '^';
				//r = 255 - mask;
				//g = 255 - mask;
				//b = 255 - mask;
			}
			
			map[y][x] = terrain;
			//outfile << terrain;//r << ' ' << g << ' ' << b << ':';//mask << ' ';
		}
	}
	
	return map;
}

std::vector<std::vector<char>> worldgen::fit_to_bounds(const std::vector<std::vector<char>>& map) {
	unsigned int x_min = map[0].size();
	unsigned int x_max = 0;
	unsigned int y_min = map.size();
	unsigned int y_max = 0;

	bool discovered_y_min = false;

	for (size_t y = 0;y < map.size();y++) {
		for (size_t x = 0;x < map[y].size();x++) {
			if (map[y][x] != ' ') {
				if (!discovered_y_min) {
					y_min = y;
					discovered_y_min = true;
				}
				else {
					y_max = y;
					if (x < x_min) {
						x_min = x;
					}
					else if (x > x_max) {
						x_max = x;
					}
				}
			}
		}
	}

	auto new_map = std::vector<std::vector<char>>(y_max + 1 - y_min);

	for (size_t y = y_min,y2 = 0;y <= y_max;y++,y2++) {
		new_map[y2] = std::vector<char>(x_max + 1 - x_min);
		for (size_t x = x_min,x2 = 0;x <= x_max;x++,x2++) {
			new_map[y2][x2] = map[y][x];
		}
	}
	
	return new_map;
}

void worldgen::write_map(const std::vector<std::vector<char>>& map,const std::string& filename) {
	std::ofstream outfile(filename);
	
	for (std::vector<char> row : map) {
		for (char cell : row) {
			outfile << cell;
		}
		outfile << std::endl;
	}

	outfile.close();
}
