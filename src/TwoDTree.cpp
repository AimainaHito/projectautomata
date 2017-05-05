#include "TwoDTree.h"

namespace automata { namespace worldgen {
	std::vector<std::shared_ptr<Circle>> Node::get_circles() const
	{
		std::vector<std::shared_ptr<Circle>> circles;
		if (left)
		{
			auto left_circles = left->get_circles();
			circles.insert(circles.end(), left_circles.begin(), left_circles.end());
		}
		circles.push_back(circle);
		if (right)
		{
			auto right_circles = right->get_circles();
			circles.insert(circles.end(), right_circles.begin(), right_circles.end());
		}

		return circles;
	}

	std::shared_ptr<Node> Node::insert(const Circle &c, std::shared_ptr<Node> root, int level)
	{
		if (!root)
		{
			return std::make_shared<Node>(std::make_shared<Circle>(c));
		}

		bool to_the_right = (level % 2 == 0) ? c.position.x > root->circle->position.x : c.position.y > root->circle->position.y;

		if (to_the_right)
		{
			root->right = insert(c, root->right, level + 1);
		}
		else
		{
			root->left = insert(c, root->left, level + 1);
		}

		return root;
	}

	std::shared_ptr<Node> Node::find_nearest(const Circle &c, std::shared_ptr<Node> root, int level,
									   std::shared_ptr<Node> best_candidate, double &best_distance)
	{
		if (!root)
		{
			return best_candidate;
		}

		auto diff = (level % 2 == 0) ? c.position.x - root->circle->position.x : c.position.y - root->circle->position.y;
		auto near = (diff <= 0) ? root->left : root->right;
		best_candidate = find_nearest(c, near, level + 1, best_candidate, best_distance);
		if (diff * diff < best_distance)
		{
			auto far = (diff <= 0) ? root->right : root->left;
			best_candidate = find_nearest(c, far, level + 1, best_candidate, best_distance);
		}

		auto distance = root->circle->position.distance(c.position);
		if (distance < best_distance)
		{
			best_distance = distance;
			return root;
		}
		else
		{
			return best_candidate;
		}
	}

	void Node::visit(std::shared_ptr<Node> root, std::function<bool(std::shared_ptr<Node>)> skip_children)
	{
		if (!root)
		{
			return;
		}

		if (!skip_children(root))
		{
			if (root->get_left())
			{
				visit(root->get_left(), skip_children);
			}
			if (root->get_right())
			{
				visit(root->get_right(), skip_children);
			}
		}
	}
}} /* namespace automata::worldgen */
