#include <iostream>
#include <stack>
#include <list>
#include <Windows.h>

class AVLtree
{
private:
	class Node
	{
	public:
		int key;
		int height;
		Node* left, * right;

		Node(int key)
		{
			this->left = this->right = nullptr;
			this->height = 0;
			this->key = key;
		}
	};

	Node* root;
	const std::string ch_hor = u8"\u2500" /*─*/, ch_ver = u8"\u2502" /*│*/, ch_ddia = u8"\u250C" /*┌*/,
		ch_rddia = u8"\u2510" /*┐*/, ch_udia = u8"\u2514" /*└*/,
		ch_ver_hor = u8"\u251C\u2500" /*├─*/, ch_udia_hor = u8"\u2514\u2500" /*└─*/,
		ch_ddia_hor = u8"\u250C\u2500" /*┌─*/, ch_ver_spa = u8"\u2502 " /*│ */;

	int getHeight(Node* node) { return (node ? node->height : -1); }

	int balanceFactor(Node* node) { return (node ? getHeight(node->right) - getHeight(node->left) : 0); }

	void updateHeight(Node* node) {
		int heightL = getHeight(node->left), heightR = getHeight(node->right);
		heightL < heightR ? node->height = heightR + 1 : node->height = heightL + 1;
	}

	void swap(Node* node1, Node* node2)
	{
		int buffer = node1->key;
		node1->key = node2->key;
		node2->key = buffer;
	}

	void rotateLeft(Node* node)
	{
		swap(node, node->right);
		Node* buffer = node->left;
		node->left = node->right;
		node->right = node->left->right;
		node->left->right = node->left->left;
		node->left->left = buffer;
		updateHeight(node);
		updateHeight(node->left);
	}

	void rotateRight(Node* node)
	{
		swap(node, node->left);
		Node* buffer = node->right;
		node->right = node->left;
		node->left = node->right->left;
		node->right->left = node->right->right;
		node->right->right = buffer;
		updateHeight(node);
		updateHeight(node->right);
	}

	void balance(Node* node)
	{
		updateHeight(node);
		if (balanceFactor(node) == 2)
		{
			if (balanceFactor(node->right) < 0) rotateRight(node->right);
			rotateLeft(node);
		}
		else if (balanceFactor(node) == -2)
		{
			if (balanceFactor(node->left) > 0) rotateLeft(node->left);
			rotateRight(node);
		}
	}
public:
	AVLtree()
	{
		this->root = nullptr;
	}

	void addNode(int key)
	{
		Node* node = new Node(key), * ptr = root;
		std::stack<Node*> stack;
		stack.push(ptr);
		if (!root)
		{
			root = node;
			return;
		}
		do
		{
			if (node->key < ptr->key)
			{
				if (!ptr->left)
				{
					ptr->left = node;
					stack.push(ptr->left);
					break;
				}
				else
				{
					ptr = ptr->left;
					stack.push(ptr);
				}
			}
			else
			{
				if (!ptr->right)
				{
					ptr->right = node;
					stack.push(ptr->right);
					break;
				}
				else
				{
					ptr = ptr->right;
					stack.push(ptr);
				}
			}
		} while (ptr);

		while (!stack.empty())
		{
			ptr = stack.top();
			stack.pop();
			balance(ptr);
		}
	}

	void print(Node* node = nullptr, std::string rbranch = "", std::string branch = "", std::string lbranch = "")
	{
		if (!node) node = root;
		if (!node) return;
		if (node->right) print(node->right, rbranch + "  ", rbranch + ch_ddia_hor, rbranch + ch_ver_spa);
		std::cout << branch << node->key << std::endl;
		if (node->left) print(node->left, lbranch + ch_ver_spa, lbranch + ch_udia_hor, lbranch + "  ");
	}

	std::list<int> symmetricalBypass()
	{
		if (!root) return {};
		Node* ptr = root;
		size_t count = 0;
		std::list<int> list;
		std::stack<Node*> stack;
		while (!stack.empty() || ptr)
		{
			if (ptr)
			{
				stack.push(ptr);
				ptr = ptr->left;
			}
			else
			{
				ptr = stack.top();
				list.push_back(ptr->key);
				count++;
				stack.pop();
				ptr = ptr->right;
			}
		}
		return list;
	}
};

void equivalence(AVLtree* tree1, AVLtree* tree2)
{
	std::list<int> list1 = tree1->symmetricalBypass();
	std::list<int> list2 = tree2->symmetricalBypass();
	std::list<int> listOfDeleting;

	while (!list1.empty() || !list2.empty())
	{
		if (list1.empty())
		{
			listOfDeleting.insert(listOfDeleting.end(), list2.begin(), list2.end());
			break;
		}
		if (list2.empty())
		{
			listOfDeleting.insert(listOfDeleting.end(), list1.begin(), list1.end());
			break;
		}
		if (list1.front() == list2.front())
		{
			list1.pop_front();
			list2.pop_front();
		}
		else
		{
			if (list1.front() < list2.front())
			{
				listOfDeleting.push_back(list1.front());
				list1.pop_front();
			}
			else
			{
				listOfDeleting.push_back(list2.front());
				list2.pop_front();
			}
		}
	}

	for (int elem : listOfDeleting)
	{
		std::cout << elem << "  ";
	}
}

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	size_t n;
	int key;
	AVLtree* tree1 = new AVLtree;
	AVLtree* tree2 = new AVLtree;
	std::cout << u8"Введите количество узлов 1 дерева: "; std::cin >> n;
	for (size_t i = 0; i < n; i++)
	{
		std::cout << u8"Введите значение " << i + 1 << ": "; std::cin >> key;
		tree1->addNode(key);
	}
	tree1->print();
	std::cout << std::endl;
	std::cout << u8"Введите количество узлов 2 дерева: "; std::cin >> n;
	for (size_t i = 0; i < n; i++)
	{
		std::cout << u8"Введите значение " << i + 1 << ": "; std::cin >> key;
		tree2->addNode(key);
	}
	tree2->print();
	equivalence(tree1, tree2);
	return 0;
}