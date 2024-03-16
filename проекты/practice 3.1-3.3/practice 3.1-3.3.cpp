//Практическая работа 3.1-3.3, вариант 16

#include <iostream>
#include <math.h>
#include <vector>
#include <array>
#include <chrono> // для функций из std::chrono
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

#include <set>
#include <unordered_set>

using namespace std;

class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class InvalidIndexException : public Exception
{
protected:
	int index;
public:
	InvalidIndexException(const char* s, int Index) : Exception(s) { index = Index; }
	InvalidIndexException(const InvalidIndexException& e) : Exception(e) { index = e.index; }
	virtual void print()
	{
		cout << "InvalidIndexException: " << str << "; Index: " << index << what();
	}
};


class InvalidExtractException : public Exception
{
public:
	InvalidExtractException(const char* s) : Exception(s) {};
	InvalidExtractException(const InvalidIndexException& e) : Exception(e) {}
	virtual void print()
	{
		cout << "InvalidExtractException: " << str << what();
	}
};

template <class T>
class Node
{
private:
	T value;
public:
	//установить данные в узле
	T getValue() { return value; }
	void setValue(T v) { value = v; }

	//сравнение узлов
	int operator<(Node N)
	{
		return (value < N.getValue());
	}

	int operator>(Node N)
	{
		return (value > N.getValue());
	}

	//вывод содержимого одного узла
	void print()
	{
		cout << value;
	}

	
};


template <class T>
void print(Node<T>* N)
{
	cout << N->getValue() << "\n";
}

//куча (heap)
template <class T>
class Heap
{
private:
	//массив
	Node<T>* arr;
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:

	//доступ к вспомогательным полям кучи и оператор индекса
	int getCapacity() { return size; }
	int getCount() { return len; }

	Node<T>& operator[](int index)
	{
		if (index < 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);//?

		return arr[index];
	}

	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new Node<T>[MemorySize];
		len = 0;
		size = MemorySize;
	}

	~Heap<T>()
	{
		delete[] arr;
	}
	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 < 0 || index1 >= len)
			throw InvalidIndexException("Invalid index", index1);
		if (index2 < 0 || index2 >= len)
			throw InvalidIndexException("Invalid index", index2);
		//здесь нужна защита от дурака

		Node<T> temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//скопировать данные между двумя узлами
	void Copy(Node<T>* dest, Node<T>* source)
	{
		dest->setValue(source->getValue());
	}

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
	Node<T>* GetLeftChild(int index)
	{
		if (index < 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака
		return &arr[index * 2 + 1];
	}

	Node<T>* GetRightChild(int index)
	{
		if (index < 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака

		return &arr[index * 2 + 2];
	}

	Node<T>* GetParent(int index)
	{
		if (index <= 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return &arr[index / 2 - 1];
		return &arr[index / 2];
	}

	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака

		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		//то же, что и SiftDown
		//нужно сравнить элементы и при необходимости произвести просеивание вниз
		if (len <= 0)return;
		int left_child_index = GetLeftChildIndex(index);
		int right_child_index = GetRightChildIndex(index);
		if (len <= left_child_index) return;
		if (right_child_index >= len && arr[index] < arr[left_child_index])
		{
			Swap(index, left_child_index);
			return;
		}
		if (len <= right_child_index) return;
		int max_index = arr[left_child_index] > arr[right_child_index] ? left_child_index : right_child_index;
		if (arr[max_index] > arr[index])
		{
			Swap(index, max_index);
			Heapify(max_index);
		}
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;
		if (index == 0)return;
		int parent_index = GetParentIndex(index);
		int index2 = GetLeftChildIndex(parent_index);
		if (index2 == index) index2 = GetRightChildIndex(parent_index);

		int max_index = arr[index] > arr[index2] ? index : index2;
		if (parent_index < len && parent_index >= 0 && max_index < len && arr[max_index]>arr[parent_index] && index != 0)
		{
			//нужно просеивание вверх
			Swap(parent_index, max_index);
			SiftUp(parent_index);
		}

	}


	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	template <class T>
	void push(Node<T>* N)
	{
		//добавить элемент и включить просеивание
		if (len < size)
		{
			arr[len] = *N;
			len++;
			SiftUp();
		}
	}

	//удобный интерфейс для пользователя 
	template<class T>
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}

	void Remove(int index)
	{
		Node<T>* a = arr[index];
		Node<T>* b = arr[len - 1];
		Swap(index, len - 1);
		len--;
		if (a > b)
			Heapify();
		else
			SiftUp();
	}

	Node<T>* ExtractMax()
	{
		//исключить максимум и запустить просеивание кучи
		//Node<T>* res = new Node<T>(arr[0]);
		Node<T>* res = new Node<T>; Copy(res, &arr[0]);
		if (len != 0)
			Swap(0, len - 1);
		len--;
		Heapify();
		return res;
	}

	//перечислить элементы кучи и применить к ним функцию
	void Straight(void(*f)(Node<T>*))
	{
		int i;
		for (i = 0; i < len; i++)
		{
			f(&arr[i]);
		}
	}

	//перебор элементов, аналогичный проходам бинарного дерева
	void PreOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void InOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void PostOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
	}

};


class Patient
{
public:
	string surname, birthday, phone, address;
	string name;
	int card_id;
	int bloody_type;

	Patient()
	{
		surname = " ";
		name = " ";
		birthday = " ";
		phone = " ";
		address = " ";
		card_id = NULL;
		bloody_type = NULL;
	}

	Patient(string sr, string nm, string bd, string ph, string ad, int card, int blood)
	{
		surname = sr;
		birthday = bd;
		phone = ph;
		address = ad;
		name = nm;
		card_id = card;
		bloody_type = blood;
	}

	/*Patient(Patient& element)
	{
		surname = element.surname;
		name = element.name;
		birthday = element.birthday;
		phone = element.phone;
		address = element.address;
		card_id = element.card_id;
		bloody_type = element.bloody_type;
	}*/


	bool operator > (Patient element)
	{
		if (card_id > element.card_id) return true;
		else if (card_id < element.card_id) return false;

		if (bloody_type > element.bloody_type) return true;
		else if (bloody_type < element.bloody_type) return false;

		if (surname < element.surname) return true;
		else if (surname > element.surname) return false;

		if (name < element.name) return true;
		return false;
	};


	bool operator < (Patient element)
	{
		if (card_id < element.card_id) return true;
		else if (card_id > element.card_id) return false;

		if (bloody_type < element.bloody_type) return true;
		else if (bloody_type > element.bloody_type) return false;

		if (surname > element.surname) return true;
		else if (surname < element.surname) return false;

		if (name > element.name) return true;
		return false;
	};

	bool operator <= (Patient element)
	{
		if (card_id <= element.card_id) return true;
		else if (card_id > element.card_id) return false;

		if (bloody_type <= element.bloody_type) return true;
		else if (bloody_type > element.bloody_type) return false;

		if (surname >= element.surname) return true;
		else if (surname < element.surname) return false;

		if (name >= element.name) return true;
		return false;
	};


	bool operator == (Patient element)
	{
		return surname == element.surname && name == element.name && birthday == element.birthday && phone == element.phone && address == element.address && card_id == element.card_id && bloody_type == element.bloody_type;
	}

	bool operator != (Patient element)
	{
		return surname != element.surname && name != element.name && birthday != element.birthday && phone != element.phone && address != element.address && card_id == element.card_id && bloody_type != element.bloody_type;
	}

	Patient operator = (Patient element)
	{
		surname = element.surname;
		name = element.name;
		birthday = element.birthday;
		phone = element.phone;
		address = element.address;
		card_id = element.card_id;
		bloody_type = element.bloody_type;
		return *this;
	}

	
	//friend ostream& operator << (ostream& s, Patient el);
	friend ostream& operator <<(ostream& s, Patient el);

};

bool operator < (const Patient& p1, const Patient& p2)
{
	if (p1.card_id < p2.card_id) return true;
	else if (p1.card_id > p2.card_id) return false;

	if (p1.bloody_type < p2.bloody_type) return true;
	else if (p1.bloody_type > p2.bloody_type) return false;

	if (p1.surname > p2.surname) return true;
	else if (p1.surname < p2.surname) return false;

	if (p1.name > p2.name) return true;
	return false;
}

ostream& operator <<(ostream& s, Patient el)
{
	s << el.name;
	return s;
}




template<typename T>
void print_queue(T& q) {
	while (!q.empty()) {
		cout << q.top() << " ";
		q.pop();
	}
	std::cout << '\n';
}



template <class V> class FibonacciHeap;

template <class V>
class FibNode {
private:
	/*FibNode<V>* prev;
	FibNode<V>* next;
	FibNode<V>* child;
	FibNode<V>* parent;
	V priority;
	int degree;
	bool marked;*/

	typename list<FibNode<V>*>::iterator current;
	typename list<FibNode<V>*>::iterator parent;

	list<FibNode<V>*> children; // Список содержащий указатели на дочерние узлы текущего узла

	int degree;
	V priority;

public:
	friend class FibonacciHeap<V>;
	typename list<FibNode<V>*>::iterator getCurrent() { return current; }
	typename list<FibNode<V>*>::iterator getParent() { return parent; }
	list<FibNode<V>*> getChildren() { return children; }

	V getPrioriy() { return priority; }
	
};

template <class V>
class FibonacciHeap
{
protected:
	list<FibNode<V>*> heap;
	typename list<FibNode<V>*>::iterator max;
public:

	FibonacciHeap()
	{
		heap.clear();
		max = heap.end();
	}
	virtual ~FibonacciHeap()
	{
		if (!heap.empty())
		{
			for (typename list<FibNode<V>*>::iterator it = heap.begin(); it != heap.end(); ++it)
				delete* it;

			heap.clear();
		}
	}
	FibNode<V>* push(V priority)
	{
		FibNode<V>* add = new FibNode<V>;

		add->priority = priority;
		add->degree = 0;
		add->children.clear();
		add->parent = add->children.end();
		add->current = heap.insert(heap.end(), add);

		if (heap.size() == 1 || (add->priority > (*max)->priority))
			max = add->current;

		return add;
	}
	void merge(FibonacciHeap& other)
	{
		heap.splice(heap.end(), other.heap);

		if ((*other.max)->priority > (*max)->priority)
			max = other.max;
	}

	bool isEmpty()
	{
		return heap.empty();
	}

	V getMaximum()
	{
		return *max;
	}

	V ExtractMax()
	{
		if (isEmpty())
			throw InvalidExtractException("Attempt to extract maximum from empty heap!");

		FibNode<V>* maxNode = *max;
		V maxValue = maxNode->priority;

		if (!maxNode->children.empty())
			heap.splice(heap.end(), maxNode->children);

		heap.erase(maxNode->current);
		delete maxNode;

		if (!heap.empty())
			Consolidate();
		else
			max = heap.end();

		return maxValue;
	}

	void Consolidate()
	{
		vector<FibNode<V>*> degreeTable(heap.size() + 1, nullptr);

		typename list<FibNode<V>*>::iterator it = heap.begin();
		while (it != heap.end())
		{
			FibNode<V>* x = *it;
			int degree = x->children.size();

			while (degreeTable[degree] != nullptr)
			{
				FibNode<V>* y = degreeTable[degree];
				if (x->priority < y->priority)
					swap(x, y);

				_merge(x, y);

				degreeTable[degree] = nullptr;
				degree++;
			}
			degreeTable[degree] = x;
			++it;
		}

		heap.clear();
		max = heap.end();
		for (int i = 0; i < degreeTable.size(); i++)
		{
			if (degreeTable[i] != nullptr)
			{
				degreeTable[i]->current = heap.insert(heap.end(), degreeTable[i]);
				if (max == heap.end() || degreeTable[i]->priority > (*max)->priority)
					max = degreeTable[i]->current;
			}
		}
	}

	void desc_print()
	{
		while (!isEmpty())
		{
			cout << ExtractMax() << endl;
		}
	}

private:
	void _merge(FibNode<V>* parent, FibNode<V>* child)
	{
		parent->children.push_back(child);

		if (!child->children.empty())
			parent->children.splice(parent->children.end(), child->children);

		parent->degree += child->degree;
		child->children.clear();
		parent->degree++;
	}
//	FibNode<V>* _empty() {
//		return NULL;
//	}
//
//	FibNode<V>* _singleton(V priority)
//	{
//		FibNode<V>* n = new FibNode<V>;
//		n->priority = priority;
//		n->prev = n->next = n;
//		n->degree = 0;
//		n->marked = false;
//		n->child = NULL;
//		n->parent = NULL;
//		return n;
//	}
//
//	FibNode<V>* _merge(FibNode<V>* a, FibNode<V>* b)
//	{
//		if (a == NULL)return b;
//		if (b == NULL)return a;
//		//делаем a бОльшим из двух
//		if (a->priority < b->priority)
//		{
//			FibNode<V>* temp = a;
//			a = b;
//			b = temp;
//		}
//		FibNode<V>* an = a->next;
//		FibNode<V>* bp = b->prev;
//		a->next = b;
//		b->prev = a;
//		an->prev = bp;
//		bp->next = an;
//		return a;
//	}
//
//	void _deleteAll(FibNode<V>* n)
//	{
//		if (n == NULL)
//			return;
//		FibNode<V>* c = n->getNext();
//		while (c != n)
//		{
//			FibNode<V>* prev = c;
//			c = c->getNext();
//			_deleteAll(prev->getChild());
//			delete prev;
//		}
//		_deleteAll(n->getChild());
//		delete n;
//	}
//
//	void _addChild(FibNode<V>* parent, FibNode<V>* child)
//	{
//		child->prev = child->next = child;
//		child->parent = parent;
//		parent->degree++;
//		parent->child = _merge(parent->child, child);
//	}
//
//	void _unMarkAndUnParentAll(FibNode<V>* n)
//	{
//		if (n == NULL)return;
//		FibNode<V>* c = n;
//		do {
//			c->marked = false;
//			c->parent = NULL;
//			c = c->next;
//		} while (c != n);
//	}
//
//	FibNode<V>* _removeMaximum(FibNode<V>* n)
//	{
//		_unMarkAndUnParentAll(n->child);
//		//один узел
//		if (n->next == n)
//			n = n->getChild();
//		else
//		{
//			//много узлов, удалить максимум из списка и объединить n->next, n->child (левый ребенок поднялся вверх)
//			n->next->prev = n->prev;
//			n->prev->next = n->next;
//			n = _merge(n->next, n->child);
//		}
//		if (n == NULL)
//		{
//			return NULL;
//		}
//
//		//consolidate()
//		FibNode<V>* trees[64] = { NULL };
//
//		while (true)
//		{
//			if (trees[n->degree])
//			{
//				FibNode<V>* t = trees[n->degree];
//				if (t == n)break;
//				trees[n->degree] = NULL;
//				if (n->priority > t->priority)
//				{
//					t->prev->next = t->next;
//					t->next->prev = t->prev;
//					_addChild(n, t);
//				}
//				else
//				{
//					t->prev->next = t->next;
//					t->next->prev = t->prev;
//					if (n->next == n)
//					{
//						t->next = t->prev = t;
//						_addChild(t, n);
//						n = t;
//						}
//					else
//					{
//						n->prev->next = t;
//						n->next->prev = t;
//						t->next = n->next;
//						t->prev = n->prev;
//						_addChild(t, n);
//						n = t;
//					}
//				}
//				continue;
//			}
//			else
//			{
//				trees[n->degree] = n;
//			}
//			n = n->next;
//		}
//		//обновить максимум и вернуть его
//		FibNode<V>* max = n;
//		FibNode<V>* start = n;
//		do
//		{
//			if (n->priority > max->priority) max = n;
//			n = n->next;
//		} while (n != start);
//		return max;
//	}

	

};



int main()
{
	Patient p1("Lenin", "Arseniy", "20.05.2004", "89603183896", "Vernadskogo pr-t 86", 122, 4);
	Patient p2("Ivanov", "Ivan", "21.05.2004", "89633183896", "Vernadskogo pr-t 78", 123, 2);
	Patient p3("Sidorov", "Petr", "19.05.2004", "89003183896", "Strominka 20", 121, 3);
	Patient p4("Kovalev", "Artem", "1.06.2004", "1234567891", "Sumskaya 9", 130, 1);
	Patient p5("Felinger", "Roman", "30.10.2002", "89679385897", "Chehova 5", 125, 3);
	Patient p6("Andreev", "Egor", "06.05.2004", "89696969696", "Krivozerye 5", 230, 2);
	Patient p7("Prusikin", "Ilya", "01.01.2006", "89632785654", "Sever 13", 170, 4);
	Patient p8("Dushin", "Maxim", "07.07.2004", "83222336996", "Izumrudnaya 7", 126, 1);

	//работа с адаптером priority_queue<Patient> 
	priority_queue<Patient> q;
	for (Patient n : {p1, p2, p3, p4, p5, p6, p7, p8})
		q.push(n);
	print_queue(q);
	cout << '\n';

	//используем Heap<Patient>
	Heap<Patient> heap;
	for (Patient p : {p1, p2, p3, p4, p5, p6, p7, p8})
		heap.push(p);
	heap.Straight(print);
	cout << '\n';

	//ExtractMax
	cout << "ExtractMax from heap:\n";
	while (heap.getCount())
	{
		heap.ExtractMax()->print();
		cout << endl;
	}

	//Фибоначчиева куча
	FibonacciHeap<Patient> fib_heap;
	for (Patient p : {p1, p2, p3, p4, p5, p6, p7, p8})
		fib_heap.push(p);
	
	cout << "\nExtractMax from fibonacci heap: " << endl;
	fib_heap.desc_print();

	return 0;
}