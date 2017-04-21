#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <memory>

namespace automata {
namespace activities {
	class activity_observer;

	class activity : public std::enable_shared_from_this<activity> {
		protected:
			std::shared_ptr<activity_observer> observer;
			int max_x,max_y;

		public:
			activity(int max_x, int max_y);

			virtual void start(std::shared_ptr<activity_observer> observer) = 0;
			virtual void destroy() = 0;
			virtual void redraw() = 0;
			virtual void update() = 0;
			virtual void send_key_event(int event) = 0;
	};

	class activity_observer {
		private:
			std::shared_ptr<activity> current_activity;
		public:
			void set_current_activity(std::shared_ptr<activity> current_activity);
			std::shared_ptr<activity> get_current_activity() const;
			void start_activity(
				std::shared_ptr<activity> next, std::shared_ptr<activity> transition, std::shared_ptr<activity_observer> observer
			);
	};

	std::shared_ptr<activity_observer> make_observer(std::shared_ptr<activity> start_activity);
}
}

#endif
