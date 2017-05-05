#include "PoissonSampler.h"

#include <cmath>
#include <cstdlib>
#include "SDL2/SDL.h"
#include <thread>
#include <chrono>
#include <iostream>

void fill_circle(SDL_Renderer *renderer, const SDL_Point &centre, int radius, SDL_Color colour)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	if (radius == 1)
	{
		SDL_RenderDrawPoint(renderer, centre.x, centre.y);
		return;
	}

	for (int width = 0; width < radius * 2; ++width)
	{
		for (int height = 0; height < radius * 2; ++height)
		{
			int dx = radius - width;
			int dy = radius - height;
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, centre.x + dx, centre.y + dy);
			}
		}
	}
}

bool init_graphics(SDL_Window *&window, SDL_Renderer *&renderer, int width, int height)
{
	//init and error handling
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL broke =C" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		std::cin.get();
		return false;
	}
	
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	
	window = SDL_CreateWindow("Poisson test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "No window for j00 =C" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		std::cin.get();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cout << "No renderer =C" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		std::cin.get();
		return false;
	}
	
	return true;
}

int main(int argc, char *args[])
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int MAX_RADIUS = 55;
	const int PADDING = 5;
	const int MAX_CANDIDATE = 5;
	const int POINT_COUNT = 500;
	
	automata::worldgen::PoissonSampler poisson(SCREEN_WIDTH, SCREEN_HEIGHT, MAX_RADIUS, PADDING, MAX_CANDIDATE, POINT_COUNT);
	auto results = poisson.generate();

	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	if (!init_graphics(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return 1;
	}
	
	SDL_Event ev;
	bool isRunning = true;

	automata::worldgen::RandomNumberGenerator rng;
	//white
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	for (auto &circle : results)
	{
		SDL_Point sdl_point = { (int) circle.position.x, (int) circle.position.y };
		fill_circle(renderer, sdl_point, (int) circle.radius, rng.next_colour(true));
	}

	while (isRunning)
	{
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}
		SDL_RenderPresent(renderer);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
