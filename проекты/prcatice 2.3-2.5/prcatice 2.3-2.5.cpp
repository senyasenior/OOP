
#include <iostream>

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


class InvalidPopException : Exception
{
public:
	InvalidPopException(const char* str) : Exception(str) {};
	virtual void print()
	{
		cout << "InvalidPopException: " << str << "; " << what();
	}
};


class ReadIteratorException : Exception
{
public:
	ReadIteratorException(const char* str) : Exception(str) {};
	virtual void print()
	{
		cout << "ReadIteratorException: " << str << "; " << what();
	}
};


template <class T>
class Element
{
	//элемент связного списка
private:
	//указатель на предыдущий и следующий элемент
	Element* next;
	Element* prev;

	//информация, хранимая в поле
	T field;
public:
	//доступ к полю *next
	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }

	//доступ к полю *prev
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }

	//доступ к полю с хранимой информацией field
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T> friend ostream& operator<< (ostream& ustream, Element<T>& obj);

	Element(T value) { field = value; next = prev = NULL; }

	bool operator < (Element<T>& el) { return field < el.getValue(); }

	bool operator > (Element<T>& el) { return field < el.getValue(); }
};

template<class T>
ostream& operator << (ostream& ustream, Element<T>& obj)
{
	ustream << obj.field;
	return ustream;
}

template <class T>
class LinkedListParent
{
protected:
	//достаточно хранить начало и конец
	Element<T>* head;
	Element<T>* tail;
	//для удобства храним количество элементов
	int num;
public:
	virtual int Number() { return num; }

	virtual Element<T>* getBegin() { return head; }

	virtual Element<T>* getEnd() { return tail; }

	LinkedListParent()
	{
		//конструктор без параметров
		cout << "\nParent constructor";
		head = NULL; tail = NULL;
		num = 0;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		//деструктор - освобождение памяти
		cout << "\nParent destructor";
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}
	}

	//получение элемента по индексу - какова асимптотическая оценка этого действия?
	virtual Element<T>* operator[](int i)
	{
		//индексация
		//if (i<0 || i>num) return NULL;
		int k = 0;

		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element<T>* cur = head;
		for (k = 0; k < i && cur != NULL; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}



	template<class T1> friend ostream& operator<< (ostream& ustream, LinkedListParent<T1>& obj);
	template<class T1> friend istream& operator>> (istream& ustream, LinkedListParent<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << *current << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << *current << "\n";

	return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
	//чтение из файла и консоли совпадают
	int len;
	ustream >> len;
	//здесь надо очистить память под obj, установить obj.num = 0
	double v = 0;
	for (int i = 0; i < len; i++)
	{
		ustream >> v;
		obj.push(v);
	}
	return ustream;
}

//дописать класс итератора по списку
template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
public:
	//конструкторы
	ListIterator() { ptr = NULL; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	//методы работы с итераторами
	//присваивание
	ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
	ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

	//проверка итераторов на равенство
	bool operator!=(ListIterator const& other) const { return other.ptr != ptr; }
	bool operator==(ListIterator const& other) const { return other.ptr == ptr; }
	//получить значение
	Element<ValueType>& operator*()
	{
		if (ptr == NULL)
		{
			throw ReadIteratorException("Iterator isn't linked with any element");
		}
		return *ptr;
	}
	//перемещение с помощью итераторов
	ListIterator& operator++() { ptr = ptr->getNext();  return *this; }//префиксная ++it
	ListIterator& operator++(int v) { ptr = ptr->getNext(); return *this; } //постфиксная it++
	ListIterator& operator--() { ptr = ptr->getPrevious();  return *this; }//префиксная --it
	ListIterator& operator--(int v) { ptr = ptr->getPrevious(); return *this; } //постфиксная it--

	//текущий элемент
	Element<ValueType>* ptr;
};

//класс итерируемый список - наследник связного списка, родитель для Очереди и Стека
template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { cout << "\nIteratedLinkedList constructor"; }
	virtual ~IteratedLinkedList() { cout << "\nIteratedLinkedList destructor"; }

	ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
	ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }

	void univ_filter(LinkedListParent<T>& lst, bool (*f)(T))
	{
		ListIterator<T> it;
		it.ptr = lst.getBegin();

		while (it.ptr != NULL)
		{
			if (f(it.ptr->getValue())) { this->push(it.ptr->getValue()); }
			it++;
		}
	}
};



template <class T>
class Stack : public IteratedLinkedList<T>
{

public:
	using LinkedListParent<T>::tail;
	using LinkedListParent<T>::head;
	using LinkedListParent<T>::num;
	using IteratedLinkedList<T>::begin;
	using IteratedLinkedList<T>::end;
	Stack() : IteratedLinkedList<T>() {  }

	virtual Element<T>* push(T value)
	{
		/*Element<T>* elem = new Element<T>(value);
		if (this->head != NULL)
			this->tail->setNext(elem);
		else
			this->head = elem;
		elem->setPrevious(this->tail);
		this->tail = elem;
		num++;
		return this->tail;*/

		//с использованием итераторов
		Element<T>* elem = new Element<T>(value);
		ListIterator<T> it = end();
		num++;
		if (head == NULL)
		{
			head = tail = elem;
			return elem;
		}
		tail = elem;
		(*it).setNext(elem);
		elem->setPrevious(it.ptr);
		return elem;
	}

	T pop()
	{
		if (tail == NULL) {
			throw InvalidPopException("it's impossible to pop an element from empty stack");
		}
		//c помощью итераторов
		num--;
		ListIterator<T> it = end();
		if (head == tail)
		{
			head = tail = NULL;
		}
		tail = (*it).getPrevious();
		(*it).setPrevious(NULL);
		if (tail)tail->setNext(NULL);
		return (*it).getValue();


		/*if (tail == head) {
			T save = tail->getValue();
			delete tail;
			head = tail = NULL;
			num--;
			return save;
		}

		Element<T>* elem = tail->getPrevious();
		T save = tail->getValue();
		elem->setNext(NULL);
		//tail->setPrevious(NULL);
		delete tail;
		tail = elem;
		num--;
		return save;*/
	}

	ListIterator<T> insert(ListIterator<T> elem, T value)
	{
		//с помощью итераторов
		Element<T>* newElem = new Element<T>(value);
		if (elem == NULL)
		{
			//вставка в начало
			ListIterator<T> it = begin();
			if (it.ptr == NULL)
			{
				it.ptr = push(value);
				return it;
			}
			(*it).setPrevious(newElem);
			newElem->setNext(it.ptr);
			head = newElem;
			num++;
			return begin();
		}
		if (head == NULL) { push(value); return begin(); }
		if (elem.ptr == head) { return insert(NULL, value); }
		Element<T>* current = elem.ptr;
		Element<T>* previous = (*elem).getPrevious();
		previous->setNext(newElem);
		newElem->setPrevious(previous);
		newElem->setNext(current);
		current->setPrevious(newElem);
		elem--;
		num++;
		return elem;

		/*Element<T>* newElem = new Element<T>(value);
		if (elem == NULL)
		{
			//вставка в начало
			Element<T>* current = head;
			if (current == NULL)return push(value);
			current->setPrevious(newElem);
			newElem->setNext(current);
			head = newElem;
			num++;
			return begin();
		}

		if (head == NULL) { push(value); return begin(); }
		if (head == tail) { return insert(NULL, value); }

		Element<T>* current = elem.ptr;
		Element<T>* previous = current->getPrevious();
		if (previous == NULL)return insert(NULL, value);
		previous->setNext(newElem);
		newElem->setPrevious(previous);
		newElem->setNext(current);
		current->setPrevious(newElem);
		elem--;
		num++;
		return elem;*/
	}

	//функция вывода с помощью итераторов
	void print()
	{
		ListIterator<T> it = begin();
		cout << "\nLength: " << num << endl;
		for (it; it.ptr != NULL; it++)
		{
			cout << *it << endl;
		}
	}

	/*friend ostream& operator<<(ostream& s, Stack<T>& lst);*/
};


////функция вывода списка с помощью итераторов
//template <class T>
//ostream& operator<< (ostream& s, Stack<T>& lst)
//{
//	ListIterator<T> it = lst.begin();
//	s << "Length: " << lst.num << '\n';
//	while (it.ptr != NULL)
//	{
//		s << *it << '\n';
//		it++;
//	}
//	return s;
//}


template<class T>
class OrderedStack : public Stack<T>
{

public:
	using LinkedListParent<T>::tail;
	using LinkedListParent<T>::head;
	using LinkedListParent<T>::num;
	using IteratedLinkedList<T>::begin;
	using IteratedLinkedList<T>::end;
	using Stack<T>::insert;
	//список отсортирован по возрастанию
	OrderedStack() : Stack<T>() { cout << "OrderedStack constructor"; }
	virtual Element<T>* push(T value)
	{
		//с помощью итераторов
		ListIterator<T> it = begin();
		if (begin() == NULL)return Stack<T>::push(value);
		while (it.ptr)
		{
			if ((*it).getValue() > value) return insert(it, value).ptr;
			it++;
		}
		//if ((*it).getValue() > value) return insert(it, value).ptr;
		return Stack<T>::push(value);
	}

};



//узел
template<class T>
class Node
{
protected:
	//закрытые переменные Node N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	Node<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		height = 1;
	}


	virtual void print()
	{
		cout << "\n" << data;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	template<class T> friend ostream& operator<< (ostream& stream, Node<T>& N);

	Node* successor()
	{
		Node<T>* Current;
		if (right != NULL)
		{
			Current = right;
			while (Current->left != NULL)
				Current = Current->left;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data <= data)
				Current = Current->parent;
		}
		return Current;
	}

	Node* predecessor()
	{
		Node<T>* Current;
		if (left != NULL)
		{
			Current = left;
			while (Current->right != NULL)
				Current = Current->right;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data > data)
				Current = Current->parent;
		}
		return Current;
	}
};

template<class T>
ostream& operator<< (ostream& stream, Node<T>& N)
{
	stream << "\nNode data: " << N.data << ", height: " << N.height;
	return stream;
}
template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<T>* root;

	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (root == NULL) { root = N; return N; }

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else { Current->setLeft(N); N->setParent(Current); }
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else { Current->setRight(N); N->setParent(Current); }
		}
		//if (Current->getData() == N->getData()) ?
		//вернуть добавленный узел
		return N;
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current)
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (Current->getData() == data) return Current;

		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}
public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push(Node<T>* N)
	{
		return push_R(N, root);
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push(N);
	}

	//удаление узла
	virtual void Remove(Node<T>* N)
	{ 
		if (N->getLeft() == NULL && N->getRight() == NULL) //лист
		{
			if (N == root)
			{
				root = NULL;
				return;
			}
			if (N->getData() > N->getParent()->getData())
				N->getParent()->setRight(NULL);
			else
				N->getParent()->setLeft(NULL);
			return;
		}

		if (N->getRight() == NULL && N->getLeft() != NULL) // только левый потомок
		{

			if (N == root)
				root = N->getLeft();
			else
			{
				if (N->getData() > N->getParent()->getData())
					N->getParent()->setRight(N->getLeft());
				else
					N->getParent()->setLeft(N->getLeft());
			}
			return;
		}

		if (N->getLeft() == NULL && N->getRight() != NULL) // только правый потомок
		{

			if (N == root)
				root = N->getRight();
			else
			{
				if (N->getData() > N->getParent()->getData())
					N->getParent()->setRight(N->getRight());
				else
					N->getParent()->setLeft(N->getRight());
			}
			return;
		}

		//есть оба потомка
		//минимальный элемент правого поддерева заменяет удаляемый

		if (N->getRight()->getLeft() != NULL)
		{
			Node<T>* new_node = Min(N->getRight());
			if (new_node->getRight() != NULL)new_node->getRight()->setParent(new_node->getParent());
			new_node->getParent()->setLeft(new_node->getRight());
			new_node->setParent(NULL);
			if (N == root)
			{
				root = new_node;
				new_node->setLeft(N->getLeft());
				N->getLeft()->setParent(new_node);
				new_node->setRight(N->getRight());
				N->getRight()->setParent(new_node);

			}
			else
			{
				if (N->getData() > N->getParent()->getData())
				{
					new_node->setParent(N->getParent());
					new_node->getParent()->setRight(new_node);
					new_node->setLeft(N->getLeft());
					N->getLeft()->setParent(new_node);
					new_node->setRight(N->getRight());
					N->getRight()->setParent(new_node);
				}
				else
				{
					new_node->setParent(N->getParent());
					new_node->getParent()->setLeft(new_node);
					new_node->setLeft(N->getLeft());
					N->getLeft()->setParent(new_node);
					new_node->setRight(N->getRight());
					N->getRight()->setParent(new_node);
				}
			}
		}
		else
		{
			Node<T>* new_node = N->getRight();
			if (N == root) root = new_node;
			new_node->setParent(N->getParent());
			if (N->getData() > N->getParent()->getData())
				new_node->getParent()->setRight(new_node);
			else
				new_node->getParent()->setLeft(new_node);
			new_node->setLeft(N->getLeft());
			N->getLeft()->setParent(new_node);
		}
	}

	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL) Current = root;
		for (; Current->getRight() != NULL; Current = Current->getRight());
		return Current;
	}


	virtual Node<T>* Find(T data)
	{
		return Find_R(data, root);
	}

	//три обхода дерева
	virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}

	friend ostream& operator <<(ostream& s, const Tree<T>& el);
};

template<class T>
ostream& operator<<(ostream& s, Tree<T>& el)
{
	for (Node<T>* current = el.Min(); current != NULL; current=current->successor())
		s << *current << "\n";
	return s;
}



template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<T>* RotateRight(Node<T>* p) // правый поворот вокруг p
	{
		Node<T>* q = p->getLeft();

		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<T>::root)
			Tree<T>::root = q;
		else
		{
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}

		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<T>* RotateLeft(Node<T>* q) // левый поворот вокруг q
	{
		Node<T>* p = q->getRight();

		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<T>::root)
			Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}

		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<T>* Balance(Node<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//вызываем функцию push_R из базового класса
		Node<T>* pushedNode = Tree<T>::push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push_R(N);
	}

	//удаление узла
	virtual void Remove(Node<T>* N)
	{ 
		Node<T>* H = N->successor()->getParent();
		Tree<T>::Remove(N);
		for (Node<T>* J = H; H != NULL; H = H->getParent())
			Balance(J);
	}
};


//класс итератор по дереву
template<typename ValueType>
class TreeIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
private:

public:
	Node<ValueType>* ptr;

	TreeIterator() { ptr = NULL; }
	TreeIterator(Node<ValueType>* p) { ptr = p; }
	TreeIterator(const TreeIterator& it) { ptr = it.ptr; }

	TreeIterator& operator=(const TreeIterator& it) { ptr = it.ptr; return *this; }
	TreeIterator& operator=(Node<ValueType>* p) { ptr = p; return *this; }

	bool operator!=(TreeIterator const& other) const { return ptr != other.ptr; }
	bool operator==(TreeIterator const& other) const { return ptr == other.ptr; }
	Node<ValueType>& operator*() { return *ptr; }
	TreeIterator& operator++() //++p;
	{
		if (ptr != NULL)
			ptr = ptr->successor();
		return *this;
	}
	TreeIterator operator++(int v) //x = p++;
	{
		Node<ValueType>* p = ptr;
		if (ptr != NULL)
			ptr = ptr->successor();
		return TreeIterator<ValueType>(p);
	}
};
//класс итерируемое дерево поиска
template<class T>
class IteratedTree : public AVL_Tree<T>
{
public:
	IteratedTree<T>() : AVL_Tree<T>() {}


	TreeIterator<T> begin() { return TreeIterator<T>(Tree<T>::Min()); }
	TreeIterator<T> end() { return TreeIterator<T>(Tree<T>::Max()); }

	virtual void Remove(Node<T>* N)
	{
		TreeIterator<T> it(N);
		Node<T>* H = (it++).ptr->getParent();
		Tree<T>::Remove(N);
		for (Node<T>* J = H; H != NULL; H = H->getParent())
			AVL_Tree<T>::Balance(J);
	}

	friend ostream& operator <<(ostream& s, const IteratedTree<T>& el);
};


template <class T>
ostream& operator<<(ostream& s, IteratedTree<T>& el)
{
	TreeIterator<T> it = el.begin();
	while (it.ptr != NULL)
	{
		s << *it;
		it++;
	}
	return s;
}



template <class T>
class Equalkeys_IteratedTree : public IteratedTree<T>
{
public:
	using Tree<T>::root;
	Equalkeys_IteratedTree():IteratedTree<T>() {};

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current)
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (Current->getData() == data) return Current;

		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() <= data) return Find_R(data, Current->getRight());
	}

	OrderedStack<T> find_key(T key)
	{
		OrderedStack<T> res;
		TreeIterator<T> it(Find_R(key, root));
		while ((*it).getData() == key)
		{
			res.push((*it).getData());
			it++;
		}
		return res;
	}
	
	OrderedStack<T> operator [] (T key)
	{
		return find_key(key);
	}

	Node<T>* push(T data)
	{
		if (Find_R(data, root))
		{
			Node<T>* current = Find_R(data, root);
			Node<T>* pushednode = new Node<T>(data);
			pushednode->setParent(current);
			pushednode->setRight(current->getRight());
			if(current->getRight())
				current->getRight()->setParent(pushednode);
			current->setRight(pushednode);
			return pushednode;
		}
		return AVL_Tree<T>::push(data);
	}
};


template<class T>
class Splay1 : public IteratedTree<T>
{
protected:
	using AVL_Tree<T>::RotateLeft;
	using AVL_Tree<T>::RotateRight;

	//функция сравнения в дереве
	bool(*equalfunc)(T, T);

	//правый поворот вокруг узла N
	Node<T>* Zig(Node<T>* N)
	{
		return RotateRight(N);
	}

	//левый поворот вокруг узла N
	Node<T>* Zag(Node<T>* N)
	{
		return RotateLeft(N);
	}

	//правый-правый поворот, левый ребёнок N поднимается на два уровня
	Node<T>* ZigZig(Node<T>* N)
	{
		return RotateRight(RotateRight(N)->getParent());
	}

	//правый-левый поворот, левый ребёнок N поднимается на два уровня
	Node<T>* ZigZag(Node<T>* N)
	{
		return RotateLeft(RotateRight(N)->getParent());
	}

	//левый-правый поворот, правый ребёнок N поднимается на два уровня
	Node<T>* ZagZig(Node<T>* N)
	{
		return RotateRight(RotateLeft(N)->getParent());
	}

	//левый-левый поворот, правый ребёнок N поднимается на два уровня
	Node<T>* ZagZag(Node<T>* N)
	{
		return RotateLeft(RotateLeft(N)->getParent());
	}


public:
	

	Splay1<T>() : IteratedTree<T>() { equalfunc = NULL; }
	Splay1<T>(bool (*f)(T, T)) : IteratedTree<T>() { equalfunc = f; };
	TreeIterator<T> begin() { return IteratedTree<T>::begin(); }
	TreeIterator<T> end() { return IteratedTree<T>::end(); }
	TreeIterator<T> Push(T data)
	{
		Node<T>* pushednode = new Node<T>(data);
		Tree<T>::push_R(pushednode, Tree<T>::root);
		while (pushednode != Tree<T>::root)
		{
			if (pushednode->getParent()->getParent()) // zig-zig, либо zig-zag, либо zag-zig, либо zag-zag
			{
				if (pushednode->getParent()->getParent()->getData() > pushednode->getParent()->getData())
				{
					if (pushednode->getParent()->getData() > data)
						ZigZig(pushednode->getParent());
					else
						ZagZig(pushednode->getParent());
				}
				else
				{
					if (pushednode->getParent()->getData() > data)
						ZigZag(pushednode->getParent());
					else
						ZagZag(pushednode->getParent());
				}
			}
			else
			{
				if (pushednode->getParent()->getData() > data)
					Zig(pushednode->getParent());
				else
					Zag(pushednode->getParent());
			}
		}
		return TreeIterator<T>(pushednode);
	}

	TreeIterator<T> find(T data)
	{
		if (Tree<T>::root == NULL) return NULL;
		Node<T>* found = Tree<T>::Find(data);
		if (found == NULL) return TreeIterator<T>();
		Tree<T>::Remove(found);
		return Push(data);
	}

	TreeIterator<T> operator[] (T data)
	{
		return find(data);
	}

	TreeIterator<T> contecst_find(T data)
	{
		if (equalfunc == NULL)
			return find(data);
		TreeIterator<T> it = begin();
		while (it.ptr != NULL)
		{
			if (equalfunc((*it).getData(), data))
				return find((*it).getData());
			it++;
		}
		return TreeIterator<T>();
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

	Patient operator = (Patient& element)
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

	friend ostream& operator << (ostream& s, Patient el);

};

ostream& operator <<(ostream& s, Patient el)
{
	s << el.name;
	return s;
}

bool equal_bloody_type(Patient el1, Patient el2)
{
	return el1.bloody_type == el2.bloody_type;
}



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

	/*//используем дерево для хранения объектов Patient
	IteratedTree<Patient> it_tree;
	it_tree.push(p1);
	it_tree.push(p5);
	it_tree.push(p2);
	it_tree.push(p3);
	it_tree.push(p4);
	it_tree.push(p6);
	it_tree.push(p7);
	it_tree.push(p8);
	//вывод по возрастанию
	cout << it_tree << endl;


	//используем адаптированное дерево для хранения объектов с одинаковыми ключами
	Equalkeys_IteratedTree<Patient> eq_keys_tree;
	eq_keys_tree.push(p1);
	eq_keys_tree.push(p5);
	eq_keys_tree.push(p2);
	eq_keys_tree.push(p3);
	eq_keys_tree.push(p4);
	eq_keys_tree.push(p6);
	eq_keys_tree.push(p7);
	eq_keys_tree.push(p8);
	//добавляем узел с ключём, который уже есть в дереве
	eq_keys_tree.push(p7);
	//вывод по возрастанию
	cout << eq_keys_tree << endl;

	//список элементов по ключу p7
	eq_keys_tree[p7].print();
	*/
	//создадим splay-дерево
	Splay1<Patient> sp_tree;
	sp_tree.Push(p1);
	sp_tree.Push(p5);
	sp_tree.Push(p2);
	sp_tree.Push(p3);
	sp_tree.Push(p4);
	sp_tree.Push(p6);
	sp_tree.Push(p7);
	sp_tree.Push(p8);

	cout << sp_tree << endl;

	//действие оператора []
	cout << *(sp_tree[p5]) << endl;


	cout << sp_tree << endl;

	//протестируем метод contecst_find на элементе p4 в sp_tree
	cout << *(sp_tree.contecst_find(p4)) << endl;
	cout << sp_tree << endl;

	Splay1<Patient> sp1_tree(equal_bloody_type);
	sp1_tree.Push(p1);
	sp1_tree.Push(p5);
	sp1_tree.Push(p2);
	sp1_tree.Push(p3);
	sp1_tree.Push(p4);
	sp1_tree.Push(p6);
	sp1_tree.Push(p7);
	sp1_tree.Push(p8);

	
	cout <<"\n patient with bloody type 1" << *(sp1_tree.contecst_find(p4)) << endl;

	/*IteratedTree<double> T;
	int arr[15];
	int i = 0;
	for (i = 0; i < 15; i++) arr[i] = (int)(100 * cos(15 * double(i)));
	for (i = 0; i < 15; i++) T.push(arr[i]);
	T.push(150);

	Node<double>* M = T.Min();
	cout << "\nMin = " << M->getData() << "\tFind " << arr[3] << ": " << T.Find(arr[3]);

	void (*f_ptr)(Node<double>*); f_ptr = print;
	/*cout << "\n-----\nPreorder:";
	T.PreOrder(T.getRoot(), f_ptr);*/
	//cout << "\n-----\nInorder:";
	//T.InOrder(T.getRoot(), f_ptr);
	/*cout << "\n-----\nPostorder:";
	T.PostOrder(T.getRoot(), f_ptr);*/
	/*cout << "\nIterators:\n";
	T.iterator = T.begin();
	while (T.iterator != T.end())
	{
		cout << *T.iterator << " ";
		T.iterator++;
	}
	cout << *T.iterator << " "; */

	/*char c; cin >> c;
	return 0;*/
}