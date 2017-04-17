#ifndef LOADING_ACTIVITY_H
#define LOADING_ACTIVITY_H

#include "activity.h"

class loading_activity : public activity {
	public: 
		loading_activity(int max_x, int max_y);

		void start(std::shared_ptr<activity_observer> observer) override;
		void destroy() override;
		void redraw() override;
		void update() override;
		void send_key_event(int char_code) override;
};

#endif
