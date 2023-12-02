// Практическая работа 7, вариант 17
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

	void operator=(Node<T> el)
	{
		value = el.getValue();
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
		if (index < 0 || index>= len)
			throw InvalidIndexException("Invalid index", index);
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index>= len)
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

class Customer
{
public:
	string surname;
	string name;
	string city; string street; double house; double flat;
	double id;
	double mean_purchase;

	Customer()
	{
		surname = "Lenin";
		name = "Arseniy";
		city = "Penza";
		street = "Sumskaya";
		house = 9;
		flat = 16;
		id = 1111;
		mean_purchase = 700000;
	}

	Customer(string Surname, string Name, string City, string Street, double House, double Flat, double ID,  double Mean_purchase)
	{
		surname = Surname; name = Name; city = City; street = Street; house = House; flat = Flat;  mean_purchase = Mean_purchase;
	}

	virtual string getKey() { return surname + " " + name; };

	virtual bool operator >(Customer el)
	{
		//|| (mean_purchase == el.mean_purchase && id > el.id) || (id == el.id && (surname + name) > (el.surname + el.name));
		if (mean_purchase > el.mean_purchase)return true;
		if (mean_purchase != el.mean_purchase) return false;
		if (id > el.id)return true;
		if (id != el.id)return false;
		return surname + name > el.surname + el.name;
	}

	virtual bool operator < (Customer el)
	{
		if (mean_purchase < el.mean_purchase)return true;
		if (mean_purchase != el.mean_purchase) return false;
		if (id < el.id)return true;
		if (id != el.id)return false;
		return surname + name < el.surname + el.name;
	}

	virtual Customer operator = (Customer el)
	{
		surname = el.surname;
		name = el.name;
		city = el.city;
		house = el.house;
		flat = el.flat;
		id = el.id;
		mean_purchase = el.mean_purchase;
		return *this;
	}

	friend ostream& operator << (ostream& s, Customer el);
};

ostream& operator << (ostream& s, Customer el)
{
	s << el.surname << ' ' << el.mean_purchase;
	return s;
}


int main()
{
	/*Heap<int> Tree;

	Tree.push(1);
	Tree.push(-1);
	Tree.push(-2);
	Tree.push(2);
	Tree.push(5);

	Tree.push(6);
	Tree.push(-3);
	Tree.push(-4);
	Tree.push(4);
	Tree.push(3);

	cout << "\n-----\nStraight:";
	void(*p)(Node<int>*); p = print;
	Tree.Straight(p);
	cout << "\n-----\nExtractMax:";
	int i = 0;
	while (i < Tree.getCount())
	{
		Node<int>* N = Tree.ExtractMax();
		N->print();
		delete N;
		cout << "\n";
	}*/

	

	Heap<Customer> Tree2;
	Customer c1("Arshinov", "Maxim", "Belgorod", "Orbitalnaya", 12, 4, 21421, 100);
	Customer c2;
	Customer c3("Kovalev", "Artem", "Ivanovo", "Moskovskaya", 5, 28, 3125, 12305);
	Customer c4("Amosov", "Vlad", "Tver", "Kislovodkaya", 95, 39, 93451, 50);
	Customer c5("Zaycev", "German", "Krasnodar", "Zaycevskaya", 32, 54, 94000, 1242);
	Tree2.push(c1); Tree2.push(c2); Tree2.push(c3); Tree2.push(c4); Tree2.push(c5);

	void(*p2)(Node<Customer>*); p2 = print;
	cout << "\n-----\nStraight:";
	Tree2.Straight(p2);
	cout << "\n-----\nExtractMax:";
	int i = 0;
	while (i < Tree2.getCount())
	{
		Node<Customer>* N = Tree2.ExtractMax();
		N->print();
		delete N;
		cout << "\n";
	}
	
	cout << '\n' << '\t' << 1 / 2;
	char c; cin >> c;
	
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.