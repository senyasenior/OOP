//Практическая работа 5, вариант 17
// функции нетривиального заполнения массива (random) и поэлементной сортировки строк матрицы (row_sort) реализованы в классе Matrix

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

template<class T>
void my_swap(T& a, T& b)
{
	//cout << "\nT my_swap";
	if (typeid(a) == typeid(int) || typeid(a) == typeid(double))
	{
		a += b;
		b = a - b;
		a -= b;
	}
	else
	{
		T temp = a;
		a = b;
		b = temp;
	}
}

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

class InvalidSizeException : public Exception
{
protected:
	int rows, cols;
public:
	InvalidSizeException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	InvalidSizeException(const InvalidSizeException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "InvalidSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class IndexOutOfBoundsException : public Exception
{
protected:
	int row, column;
public:
	IndexOutOfBoundsException(const char* s, int Row, int Column) : Exception(s) { row = Row; column = Column; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { row = e.row; column = e.column; }
	virtual void print()
	{
		cout << "IndexOutOfBoundsException: " << str << "; Row: " << row << ", Column: " << column << "; " << what();
	}
};

class WrongSizeException : public Exception
{
protected:
	int rows, cols;
public:
	WrongSizeException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	WrongSizeException(const WrongSizeException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "WrongSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class NonPositiveSizeException : public WrongSizeException
{
public:
	NonPositiveSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	NonPositiveSizeException(const NonPositiveSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "NonPositiveSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class TooLargeSizeException : public WrongSizeException
{
public:
	TooLargeSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	TooLargeSizeException(const TooLargeSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "TooLargeSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class TooSmallSizeException : public WrongSizeException
{
public:
	TooSmallSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	TooSmallSizeException(const TooSmallSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "TooSmallSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

template<class T>
class BaseMatrix
{
protected:
	T** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		//конструктор
		if (Height <= 0 || Width <= 0)
			throw NonPositiveSizeException("Matrix size can't be negative or zero in constructor int, int", Height, Width);
		if (Height > 50 || Width > 50)
			throw TooLargeSizeException("Matrix size can't be more than 50 in constructor int, int", Height, Width);
		height = Height;
		width = Width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];
	}

	BaseMatrix(const BaseMatrix& M)
	{
		//конструктор копий
		height = M.height;
		width = M.width;
		ptr = new T * [height];

		/*
		for (int i = 0; i < height; i++)
			ptr[i] = new double[width];

		for (int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		*/

		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}

	BaseMatrix(T** arr, int h, int w)
	{
		if (h <= 0 || w <= 0) throw InvalidSizeException("Matix size can't be negative or zero in constructor T**a", h, w);

		cout << "const T** consturctor is working";
		//конструктор, принимающий массив типа T**
		height = h;
		width = w;
		ptr = new T* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
			{
				ptr[i][j] = arr[i][j];
			}
		}
	}

	virtual ~BaseMatrix()
	{
		//деструктор
		if (ptr != NULL)
		{
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
		//cout << "\nBase Destructor";
	}

	void print()
	{
		//вывод
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}

	BaseMatrix operator=(const BaseMatrix<T> M)
	{
		for (int i = 0; i < height; i++)
			delete[] ptr[i];
		delete[] ptr;
		height = M.height;
		width = M.width;
		ptr = new T*[height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
		return *this;
	}

	T* operator[](int index)
	{
		if (index < 0 || index >= height)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", index, -1);
		return ptr[index];
	}

	T& operator()(int row, int column)
	{
		if (row < 0 || row >= height || column < 0 || column >= width)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, column);
		return ptr[row][column];
	}
};

template<class T>
class Matrix : public BaseMatrix<T>
{
public:
	using BaseMatrix<T>::width;
	using BaseMatrix<T>::height;
	using BaseMatrix<T>::ptr;

	Matrix<T>(int Height = 2, int Width = 2) : BaseMatrix<T>(Height, Width) { cout << "\nMatrix constructor is working!"; }
	virtual ~Matrix() { cout << "\nDerived Destructor"; }
	T operator*()
	{
		if (BaseMatrix<T>::width != BaseMatrix<T>::height)
			throw InvalidSizeException("Couldn't execute operation for rectangular matrix in operator*()", BaseMatrix<T>::height, BaseMatrix<T>::width);
		T p = 1;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			p *= this->ptr[i][this->width - 1 - i];
		}
		return p;
	}

	void random(int min = 0, int max = 32767)
	{
		//заполнение матрицы произвольными целыми числами из отрезка [min; max]
		srand(time(NULL));

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				ptr[i][j] = min + rand() % (max - min + 1);
	}

	Matrix<T> row_sort()
	{
		if (width == 1) throw TooSmallSizeException("Can't sort rows with only one element", height, width);
		Matrix<T> res(*this);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				int sorted = j - 1;
				while (sorted > -1 && res.ptr[i][sorted] < res.ptr[i][sorted + 1])
				{
					my_swap(res.ptr[i][sorted], res.ptr[i][sorted + 1]);
					sorted--;
				}
			}
		return res;
	}

	template<class T>
	friend ostream& operator<<(ostream& s, Matrix<T> M);
	template<class T>
	friend istream& operator>>(istream& s, Matrix<T>& M);
};

template <class T>
ostream& operator<<(ostream& s, Matrix<T> M)
{
	if (typeid(s) == typeid(ofstream))
	{
		s << M.height << " " << M.width << " ";
		for (int i = 0; i < M.height; i++)
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
	}
	else
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
			s << "\n";
		}
	return s;
}

template <class T>
istream& operator>>(istream& s, Matrix<T>& M)
{
	if (typeid(s) == typeid(ifstream))
	{
		int w, h; s >> h >> w;
		if (h != M.height || w != M.width)
		{
			//throw WrongSizeException("Matrices in the code and in file have got different size", h, w);
			M.height = h;
			M.width = w;
			for (int i = 0; i < M.height; i++)
				delete[] M.ptr[i];
			delete[] M.ptr;
			M.ptr = new T * [M.height];
			for (int i = 0; i < M.height; i++)
				M.ptr[i] = new T[M.width];
		}
	}
	for (int i = 0; i < M.height; i++)
	{
		for (int j = 0; j < M.width; j++)
			s >> M.ptr[i][j];
	}
	return s;
}

template<class T, int H = 2, int W = 2>
class Matrix2 : public BaseMatrix<T>
{
public:
	Matrix2<T, H, W>() : BaseMatrix<T>(H, W) { cout << "\nMatrix constructor is working!"; }
	virtual ~Matrix2() { cout << "\nDerived Destructor"; }
	T operator*()
	{
		if (BaseMatrix<T>::width != BaseMatrix<T>::height)
			throw InvalidSizeException("Couldn't execute operation for rectangular matrix in operator*()", BaseMatrix<T>::height, BaseMatrix<T>::width);
		T p = 1;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			p *= this->ptr[i][this->width - 1 - i];
		}
		return p;
	}

	template<class T, int H, int W>
	friend ostream& operator<<(ostream& s, Matrix2<T, H, W> M);
	template<class T, int H, int W>
	friend istream& operator>>(istream& s, Matrix2<T, H, W>& M);
};

template <class T, int H, int W>
ostream& operator<<(ostream& s, Matrix2<T, H, W> M)
{
	if (typeid(s) == typeid(ofstream))
	{
		s << M.height << " " << M.width << " ";
		for (int i = 0; i < M.height; i++)
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
	}
	else
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
			s << "\n";
		}
	return s;
}

template <class T, int H, int W>
istream& operator>>(istream& s, Matrix2<T, H, W>& M)
{
	if (typeid(s) == typeid(ifstream))
	{
		int w, h; s >> h >> w;
		if (h != M.height || w != M.width)
			throw WrongSizeException("Matrices in the code and in file have got different size", h, w);
	}
	for (int i = 0; i < M.height; i++)
		for (int j = 0; j < M.width; j++)
			s >> M.ptr[i][j];
	return s;
}



int main()
{
	try
	{
		//5.1
		//тест сортировки
		/*Matrix<double>M; M.random(-10, 10); cout << "\n" << M << "\n";
		Matrix<double> res;
		res = M.row_sort(); cout << "\n" << res << "\n" << M << endl;*/

		//5.2
		//тест исключений
		//Matrix<double>M(3, 2); *M;
		//Matrix<double>M; M(2, 0);
		/*ifstream fin("test.txt");
		Matrix<double>M(1, 1);
		fin >> M;
		fin.close();*/
		//Matrix<double>M(-1, 2);
		//Matrix<double>M(1, 51);
		//Matrix<double>M(20, 1); M.row_sort();



		//5.3
		//тест ввода и вывода из файла

		Matrix<double>M1; M1.random(-10, 10); M1.print(); cout << "\n";

		Matrix<double>M2;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				M2[i][j] = -M1[i][j];
		M2.print();

		Matrix<double>M3;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				M3[i][j] = i + j;
		M3.print();

		Matrix<double> arr[3];
		arr[0] = M1; arr[1] = M2; arr[2] = M3;

		//cout << M(2, 3);
		//cout << *M << endl;
		ofstream fout("test.txt");
		if (fout)
		{
			for (int i = 0; i < 3; i++)fout << arr[i];
			fout.close();
		}


		ifstream fin("test.txt");
		if (fin)
		{
			Matrix<double> res[3];
			int i = 0;
			while (!fin.eof())
			{
				fin >> res[i];
				i++;
			}
			for (int i = 0; i < 3; i++)cout << res[i];
			fin.close();
		}



	}
	catch (TooSmallSizeException e)
	{
		cout << "\nTooSmallSizeException has been caught: "; e.print();
	}
	catch (IndexOutOfBoundsException e)
	{
		cout << "\nIndexOutOfBoundsException has been caught: "; e.print();
	}
	catch (InvalidSizeException e)
	{
		cout << "\nInvalidSizeException has been caught: "; e.print();
	}
	catch (NonPositiveSizeException e)
	{
		cout << "\nNonPositiveSizeException has been caught: "; e.print();
	}
	catch (TooLargeSizeException e)
	{
		cout << "\nTooLargeSizeException has been cauth: "; e.print();
	}
	catch (WrongSizeException e)
	{
		cout << "\nWrongSizeException has been caught: "; e.print();
	}

	catch (Exception e)
	{
		cout << "\nException has been caught: "; e.print();
	}
	catch (exception e)
	{
		cout << "\nexception has been caught: "; e.what();
	};
	//char c;cin >> c;
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