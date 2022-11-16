#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <set>

struct Node
{
	std::string data;
	Node* left;
	Node* right;
};

std::vector<std::string> readFiles(std::string filename, std::vector<std::string> collectionPostingLists)
{
	std::ifstream file1;
	std::string buff;
	std::string word;

	file1.open(filename);

	while (getline(file1, buff))
	{
		for (auto& c : buff)
		{
			if (c > 33 && c < 127)
			{
				word += c;
			}
			else
			{
				if(word != "")
					collectionPostingLists.push_back(word);
				word = "";
			}
		}
		if (buff.at(buff.size() - 1) > 33) // in case of one word on row wtihout space
			collectionPostingLists.push_back(word);
		word = "";
	}
	file1.close();
	return collectionPostingLists;
}

Node* getNewNode(std::string data)
{
	Node* bst = new Node();
	bst->data = data;
	bst->left = bst->right = NULL;
	return bst;
}
Node* insertNode(Node* parent, std::string data)
{
	if (parent == NULL)
	{
		parent = getNewNode(data);
		return parent;
	}
	else if (parent->data > data)
	{
		parent->left = insertNode(parent->left, data);
		return parent;
	}
	else
	{
		parent->right = insertNode(parent->right, data);
		return parent;
	}
}
void traverseDocument(std::string data, std::vector<std::string> vec1)
{
	for (int i = 0; i < vec1.size(); i++)
	{
		if (vec1[i] == data)
		{
			std::cout << i + 1 << ", ";
		}
	}
	std::cout << std::endl;
}
void inorder(Node* parent, std::vector<std::string> vec1)
{
	if (parent == NULL)
	{
		return;
	}
	else
	{
		inorder(parent->left, vec1);
		std::cout << parent->data << " -> ";
		traverseDocument(parent->data, vec1);
		inorder(parent->right, vec1);
	}
}

Node* insertIntoTree(Node* parent, std::set<std::string> set1)
{
	for(auto& str: set1)
	{
		parent = insertNode(parent, str);
	}
	return parent;
}

int main()
{
	std::vector<std::string> words;
	std::set<std::string> uniqueWords;
	words = readFiles("index.html", words);

	for (auto& word : words)
	{
		uniqueWords.insert(word);
	}

	Node* parent = NULL;
	parent = insertIntoTree(parent, uniqueWords);
	inorder(parent, words);
}
