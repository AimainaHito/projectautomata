#ifndef RK_TWODTREE_H_
#define RK_TWODTREE_H_

#include <vector>
#include <memory>
#include <limits>
#include "Util.h"

namespace automata { namespace worldgen {
	class Node
	{
		public:
			Node(std::shared_ptr<Circle> c) : circle(c) {}
			std::vector<std::shared_ptr<Circle>> get_circles() const;
			static std::shared_ptr<Node> insert(const Circle &c, std::shared_ptr<Node> root, int level);
			static std::shared_ptr<Node> find_nearest(const Circle &c, std::shared_ptr<Node> root, int level,
											   std::shared_ptr<Node> best, double &best_distance);
			//if the callback returns true, don't pass it down the node's children
			static void visit(std::shared_ptr<Node> root, std::function<bool(std::shared_ptr<Node>)> skip_children);
			std::shared_ptr<Circle> get_circle() const { return circle; }
			std::shared_ptr<Node> get_left() const { return left; }
			std::shared_ptr<Node> get_right() const { return right; }

		private:
			std::shared_ptr<Node> left;
			std::shared_ptr<Node> right;
			std::shared_ptr<Circle> circle;
	};

	class TwoDTree
	{
		public:
			void insert(const Circle &c)
			{
				root = Node::insert(c, root, 0);
			}

			std::shared_ptr<Circle> find_nearest(const Circle &c, double &best_distance) const
			{
				best_distance = std::numeric_limits<double>::max();
				auto best = Node::find_nearest(c, root, 0, nullptr, best_distance);

				return best ? best->get_circle() : nullptr;
			}

			std::vector<std::shared_ptr<Circle>> get_circles() const
			{
				if (root)
				{
					return root->get_circles();
				}

				return {};
			}

			//if callback returns true, don't call it on the node's children
			void visit(std::function<bool(std::shared_ptr<Node>)> skip_children)
			{
				Node::visit(root, skip_children);
			}

		private:
			std::shared_ptr<Node> root;
	};

}} /* namespace automata::worldgen */

#endif /* RK_TWODTREE_H_ */
