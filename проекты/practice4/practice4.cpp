//Практическая работа 4, вариант 16

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <queue>


using namespace std;

//класс узел
class Node
{
private:
	//закрытые переменные Node N; N.key = 10 вызовет ошибку
	double key;
	int priority;

	int size;
	double min;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;
public:
	//доступные извне переменные и функции
	double getKey() { return key; }
	int getPriority() { return priority; }
	int getSize() { return size; }
	double getMin() { return min; }

	Node* getLeft() { return left; }
	Node* getRight() { return right; }
	Node* getParent() { return parent; }

	void setLeft(Node* N) { left = N; }
	void setRight(Node* N) { right = N; }
	void setParent(Node* N) { parent = N; }

	void setSize(int s) { size = s; }
	void setMin(double m) { min = m; }

	//конструктор устанавливает стартовые значения для указателей
	Node(double k, int p)
	{
		key = k;
		priority = p;
		left = right = NULL;
		parent = NULL;
	}

	Node(double k)
	{
		key = k;
		srand(time(NULL));
		priority = rand();
		left = right = NULL;
		parent = NULL;
	}

	Node()
	{
		left = right = NULL;
		parent = NULL;
	}


	Node* successor()
	{
		Node* Current = this;
		if (right != NULL)
		{
			for (Current = right; Current->getLeft() != NULL; Current = Current->getLeft());
			return Current;
		}
		else
		{
			double val = Current->getKey();
			if (Current->getParent() != NULL && Current->getParent()->getKey() > val)
				return Current->getParent();
			while (Current->getParent() != NULL)
			{
				Current = Current->getParent();
				if (Current->getKey() > val) return Current;
			}
			if (val > Current->getKey()) return NULL;
			return Current;
		}
	}
	Node* predecessor()
	{
		Node* Current = this;
		if (left != NULL)
		{
			for (Current = left; Current->getRight() != NULL; Current = Current->getRight());
			return Current;
		}
		else
		{
			double val = Current->getKey();
			if (Current->getParent() != NULL && Current->getParent()->getKey() < val)
				return Current->getParent();
			while (Current->getParent() != NULL)
			{
				Current = Current->getParent();
				if (Current->getKey() < val) return Current;
			}
			if (val < Current->getKey()) return NULL;
			return Current;
		}
	}


	friend ostream& operator<<(ostream& stream, Node& N)
	{
		stream << "(" << N.key << "; " << N.priority << ")" << "\n";
		if (N.getLeft() != NULL)
			stream << *N.getLeft();
		if (N.getRight() != NULL)
			stream << *N.getRight();
		return stream;
	}
};

//Класс перегрузок итераторов
class TreapIterator : public iterator<input_iterator_tag, double>
{
private:
	Node* ptr;
public:
	TreapIterator() { ptr = NULL; }
	TreapIterator(Node* p) { ptr = p; }
	TreapIterator(const TreapIterator& it) { ptr = it.ptr; }

	TreapIterator& operator=(const TreapIterator& it) { ptr = it.ptr; return *this; }
	TreapIterator& operator=(Node* p) { ptr = p; return *this; }

	bool operator!=(TreapIterator const& other) const { return ptr != other.ptr; }
	bool operator==(TreapIterator const& other) const { return ptr == other.ptr; }
	Node& operator*() { return *ptr; }
	TreapIterator& operator++() //++p
	{
		if (ptr != NULL)
			ptr = ptr->successor();
		return *this;
	}
	TreapIterator operator++(int v)
	{
		Node* previous = ptr;
		if (ptr != NULL)
			ptr = ptr->successor();
		return TreapIterator(previous);
	}

	TreapIterator& operator--() //--p
	{
		if (ptr != NULL)
			ptr = ptr->predecessor();
		return *this;
	}
	TreapIterator operator--(int v)
	{
		Node* next = ptr;
		if (ptr != NULL)
			ptr = ptr->predecessor();
		return TreapIterator(next);
	}
};

//класс дерево
class Treap
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node* root;

	Node* merge(Node* L, Node* R)
	{
		if (L->getPriority() > R->getPriority())
		{
			if (L->getRight() != NULL) {
				L->setRight(merge(L->getRight(), R));
				L->getRight()->setParent(L);
			}
			else {
				L->setRight(R);
				L->getRight()->setParent(L);
			}
		}
		else
		{
			if (R->getLeft() != NULL) {
				R->setLeft(merge(L, R->getLeft()));
				R->getLeft()->setParent(R);
			}
			else {
				R->setLeft(L);
				R->getLeft()->setParent(R);
			}
			L = R;
		}
		return L;
	}


	Node* split(Node* Current, double x, Treap* Left, Treap* Right)
	{
		if (root == NULL || Current == NULL)
		{
			Left = NULL;
			Right = NULL;
			return NULL;
		}

		TreapIterator it;
		for (it = this->begin(); (*it).getKey() < x; it++)
		{
			Left->Merge((*it).getKey(), (*it).getPriority());
		}

		Node* out;
		if ((*it).getKey() == x)
		{
			out = &(*it);
			it++;
		}
		else
			out = NULL;

		for (it; it != NULL; it++)
		{
			Right->Merge((*it).getKey(), (*it).getPriority());
		}
		root = NULL;
		return out;
	}

public:
	TreapIterator begin() { return TreapIterator(Treap::Min()); }
	TreapIterator end() { return TreapIterator(Treap::Max()); }

	//доступ к корневому элементу
	Node* getRoot() { return root; }
	void setRoot(Node* newroot) { root = newroot; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Treap() { root = NULL; }

	void PreOrder(Node* N, void (*f)(Node*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//Операция слияния деревьев
	virtual void Merge(Node* N)
	{
		Treap* R = new Treap();
		R->root = N;
		Merge(R);
	}

	virtual void Merge(double key, int prior)
	{
		Node* N = new Node(key, prior);
		Merge(N);
	}
	virtual void Merge(Treap* R)
	{
		if (root == NULL) { root = R->root; return; }
		if (R->root == NULL) return; //this;

		if (root->getPriority() > R->root->getPriority())
		{
			if (root->getRight() != NULL) {
				root->setRight(merge(root->getRight(), R->root));
				root->getRight()->setParent(root);
			}
			else {
				root->setRight(R->root);
				root->getRight()->setParent(root);
			}
		}
		else
		{
			if (R->root->getLeft() != NULL) {
				R->root->setLeft(merge(root, R->root->getLeft()));
				R->root->getLeft()->setParent(R->root);
			}

			else {
				R->root->setLeft(root);
				R->root->getLeft()->setParent(R->root);
			}
			root = R->root;
		}
	}

	//Операция разрезания дерева
	Node* Split(double x, Treap* Left, Treap* Right)
	{
		Node* result = split(root, x, Left, Right);
		return result;
	}

	virtual Node* Min(Node* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;
		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}
	virtual Node* Max(Node* Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL)
			Current = root;
		while (Current->getRight() != NULL)
			Current = Current->getRight();
		return Current;
	}


	friend ostream& operator<<(ostream& stream, Treap& T)
	{
		if (T.root != NULL)
			stream << *T.root;
		return stream;
	}
};
void print(Node* N) { cout << N->getKey() << "; " << N->getPriority(); }



//Вывод по ключу
void printUp(Treap* treap) {
	if (treap == NULL) { throw "Treap is empty"; }
	TreapIterator it = (*treap).begin();
	cout << *it << endl;
	do {
		++it;
		cout << *it << endl;
	} while (it != (*treap).end());
}
void printDown(Treap* treap) {
	if (treap == NULL) { throw "Treap is empty"; }
	TreapIterator it = (*treap).end();
	cout << *it << endl;
	do {
		--it;
		cout << *it << endl;
	} while (it != (*treap).begin());
}


class ImplicitTreap : protected Treap
{
protected:
	pair<Node*, Node*> split(Node* Current, double key)
	{
		if (Current == NULL)
			return { nullptr, nullptr };
		if (Size(Current) <= key)
		{
			root = NULL;
			return { Current, NULL };
		}
		if (key == 0)
		{
			root = NULL;
			return { NULL, Current };
		}
		if (Size(Current->getLeft()) >= key)
		{
			auto res = split(Current->getLeft(), key);
			Current->setLeft(res.second);
			Update(Current);
			return { res.first, Current };
		}
		else
		{
			auto res = split(Current->getRight(), key - Size(Current->getLeft()) - 1);
			Current->setRight(res.first);
			Update(Current);
			return { Current, res.second };
		}
	}

	// Новый метод для обновления размера и суммы узла(групповая функция)
	void Update(Node* N)
	{
		if (N != NULL)
		{
			N->setSize(1 + Size(N->getLeft()) + Size(N->getRight()));
			N->setMin(N->getKey());
			if (N->getLeft() != NULL)
			{
				N->setSize(N->getSize() + N->getLeft()->getSize());
				N->setMin(min(N->getMin(), N->getLeft()->getMin()));
			}
			if (N->getRight() != NULL)
			{
				N->setSize(N->getSize() + N->getRight()->getSize());
				N->setMin(min(N->getMin(), N->getRight()->getMin()));
			}
		}
	}

public:
	ImplicitTreap() : Treap() {}
	Node* getRoot() { return Treap::root; }

	TreapIterator begin() { return TreapIterator(Treap::Min()); }
	TreapIterator end() { return TreapIterator(Treap::Max()); }

	//Операция слияния деревьев
	virtual void Merge(Node* N)
	{
		ImplicitTreap* R = new ImplicitTreap();
		R->setRoot(N);
		Merge(R);
	}

	virtual void Merge(double key, int prior)
	{
		Node* N = new Node(key, prior);
		Merge(N);
	}
	virtual void Merge(ImplicitTreap* R)
	{
		if (root == NULL) { root = R->getRoot(); return; }
		if (R->getRoot() == NULL) return; //this;

		if (Treap::root->getPriority() > R->getRoot()->getPriority())
		{
			if (Treap::root->getRight() != NULL) {
				Treap::root->setRight(merge(Treap::root->getRight(), R->getRoot()));
				Treap::root->getRight()->setParent(Treap::root);
			}
			else {
				Treap::root->setRight(R->getRoot());
				Treap::root->getRight()->setParent(Treap::root);
			}
		}
		else
		{
			if (R->getRoot()->getLeft() != NULL) {
				R->getRoot()->setLeft(merge(root, R->root->getLeft()));
				R->getRoot()->getLeft()->setParent(R->root);
			}

			else {
				R->getRoot()->setLeft(root);
				R->getRoot()->getLeft()->setParent(R->root);
			}
			root = R->getRoot();
		}
	}

	//Операция разрезания дерева
	pair<Node*, Node*> Split(double x) { return split(root, x); }

	int Size(Node* root)
	{
		if (root != NULL) { return Treap::root->getSize(); }
		return 0;
	}

	// Метод для получения суммы на отрезке [l, r]
	double getSum(int l, int r)
	{
		pair<Node*, Node*> pair1 = Split(l);
		pair<Node*, Node*> pair2 = Split(r - l + 1);
		double sum = (pair2.first != NULL) ? pair2.first->getMin() : 0;
		// Обратно объединяем разделенные деревья
		Merge(pair1.first);
		Merge(pair2.first);
		Merge(pair2.second);
		return sum;
	}


	friend ostream& operator<<(ostream& stream, ImplicitTreap& T)
	{
		if (T.getRoot() != NULL)
			stream << *T.getRoot();
		return stream;
	}
};
void printUp(ImplicitTreap* treap) {
	if (treap == NULL) { throw "Treap is empty"; }
	TreapIterator it = (*treap).begin();
	cout << *it << endl;
	do {
		++it;
		cout << *it << endl;
	} while (it != (*treap).end());
}

int main()
{

	Treap T;
	ImplicitTreap IT;
	int arr[15];
	int i = 0;

	//заполнение дерева
	for (i = 0; i < 15; i++) arr[i] = (int)(100 * sin(4 * double(i)));
	cout << "------------------------------";
	cout << "\nArray:\n";
	for (i = 0; i < 15; i++)
	{
		//T.insert(i, arr[i]); 
		T.Merge(i, arr[i]);
		IT.Merge(i, arr[i]);
		cout << "(" << i << "; " << arr[i] << ")\n";
		//cout << "\n---\n" << i << "\n" << T;
	}
	cout << endl << "Treap:\n";
	printUp(&T);
	cout << "------------------------------";
	cout << "\ninserted\n";
	cout << T;
	Treap T1, T2;
	Node* N = T.Split(5, &T1, &T2);
	cout << "-------------------" << "\nLeft treap:\n";
	printUp(&T1);
	cout << "-------------------" << "\nRight treap:\n";
	printUp(&T2);
	cout << "----------------------";
	if (N != NULL)
		cout << "\nNode: " << *N;

	cout << endl << "ImplicitTreap:\n";
	printUp(&IT);
	pair<Node*, Node*> p;
	ImplicitTreap IT1;
	double sum = IT1.getSum(2, 5);
	cout << "Sum on the interval [2, 5]: " << sum << endl;

	char c; cin >> c;
	return 0;
}
