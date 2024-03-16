//Практическая работа 5.1 - 5.4, вариант 16

#include <iostream>
#include<queue>
#include<stack>
#include<vector>
#include<set>

using namespace std;


#define INFINITY 10000 // условное число, обозначающее бесконечность

//поиск в глубину
vector<int> dfs(vector<vector<int>> matrix)
{
	vector<int> res;
	
	vector <int> used(matrix.size());
	/*0 – вершина не посещена при поиске, 1 – помещена в
	структуру данных для вершин, но не обработана, 2 –
		обработана, смежные вершины помещены в структуру данных*/
	stack<int> Stack;
	Stack.push(0);

	while (!Stack.empty())
	{
		int vertex = Stack.top();
		Stack.pop();
		//std::cout << "\nDFS at vertex " << vertex << endl;
		if (used[vertex] == 2)
			continue;
		
		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[vertex][j] && used[j] != 2)
			{
				Stack.push(j);
				used[j] = 1;
			}
			
		}
		used[vertex] = 2;
		res.push_back(vertex);
		//std::cout << vertex << endl;
	}
	return res;
}

//функция поиска минимальной длины пути от i-й вершины во все
vector<int> Dijkstra(vector<vector<int>> matrix, int i)
{
	//очередь с приоритетом хранит номер вершины и расстояние от неё до остова
	// первое значение - расстояние, второе - вершина
	priority_queue< pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > Q;
	Q.push(make_pair(0, i));

	//вектор расстояний от i вершины до каждой
	vector<int> D(matrix.size(), INFINITY);
	D[i] = 0;
	
	while (!Q.empty())
	{
		int vertex = Q.top().second;
		int dist = Q.top().first;
		Q.pop();

		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[vertex][j] && matrix[vertex][j] + dist < D[j])
			{
				D[j] = matrix[vertex][j] + dist;
				Q.push(make_pair(D[j], j));
			}
		}
	}
	return D;
}

int min(int val1, int val2)
{
	if (val1 < val2)
		return val1;
	return val2;
}

//функция подсчёта степеней вершин в дереве
vector<int> degrees_of_vertices(vector<vector<int>> matrix)
{
	queue<int> Q;
	Q.push(0);
	vector<int> res(matrix.size());
	vector<bool> used(matrix.size());
	while (!Q.empty())
	{
		int vertex = Q.front();
		Q.pop();
		//счётчик сежный вершин
		int k = 0;
		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[vertex][j] && used[j] == false)
			{
				k++;
				Q.push(j);
			}
		}
		used[vertex] = true;
		res[vertex] = k;
	}
	return res;
}


//функция подсчёта средней степени вершин в дереве
double avg_degree(vector<vector<int>> matrix)
{
	vector<int> degrees = degrees_of_vertices(matrix);
	int sum = 0;
	for (int j = 0; j < degrees.size(); j++)
		sum += degrees[j];
	double res = sum / (degrees.size() - 1);
	return res;
}


//функция нахождения минимального остова по алгоритму Прима
vector<vector<int>> Prim(vector<vector<int>> matrix)
{
	//очередь с приоритетом хранит номер вершины и расстояние от неё до остова
	// первое значение - расстояние, второе - вершина, третье - родитель
	priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int,int>>> > Q;
	Q.push(make_pair(0, make_pair(0, 0)));
	
	//вектор вершин, входящих в остов
	vector<bool> U(matrix.size(), false);

	//итоговая матрица смежности
	vector <vector <int >> res(matrix.size(), vector<int>(matrix.size()));



	while (!Q.empty())
	{

		int vertex = Q.top().second.first;
		int parent = Q.top().second.second;
		
		if (U[vertex] == true)
		{
			Q.pop();
			continue;
		}
		res[vertex][parent] = res[parent][vertex] = Q.top().first;
		Q.pop();
		U[vertex] = true;
		for (int j = 0; j < matrix.size(); j++)
		{
			if (!U[j] && matrix[vertex][j])
			{
				Q.push(make_pair(matrix[vertex][j], make_pair(j, vertex)));
			}
		}
	}
	
	return res;
}


void FillVectorWith(vector<int>& v, int value, int size = 0)
{
	if (size != 0)
	{
		v.resize(0);
		for (int i = 0; i < size; i++)
			v.push_back(value);
	}
	for (int i = 0; i < v.size(); i++)
		v[i] = value;
}

void FillMatrixWith(vector<vector<int> >& matrix, int value, int width = 0, int height = 0)
{
	if (width != 0 && height != 0)
	{
		matrix.resize(0);
		int i, j;
		for (i = 0; i < height; i++)
		{
			vector<int> v1;
			for (j = 0; j < width; j++)
			{
				v1.push_back(value);
			}
			matrix.push_back(v1);
		}
	}
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
			matrix[i][j] = value;
	}
}

int FindPath(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) // source - исток, target - сток
{
	int i;
	int CurVertex;
	// Flow - значение потока через данную вершину на данном шаге поиска
	// Link[i] хранит номер предыдущей вешины на пути i -> исток
	vector<int> Flow; FillVectorWith(Flow, 0, vertices);
	vector<int> Link; FillVectorWith(Link, -1, vertices);
	Flow[source] = INFINITY;
	vector<int> used(vertices, 0);
	//queue<int> q; q.push(source);
	stack<int> s; s.push(source);
	//while (!q.empty())
	while (!s.empty())
	{
		//int vertex = q.front();
		int vertex = s.top();
		if (vertex == target) break;
		s.pop();
		//q.pop();

		for (int i = 0; i < vertices; i++)
		{
			//есть ребро vertex->i, в i не заходили
			if (c[vertex][i] - f[vertex][i] > 0 && used[i] == 0)
			{
				s.push(i);//q.push(i);
				Flow[i] = (c[vertex][i] - f[vertex][i] < Flow[vertex]) ? c[vertex][i] - f[vertex][i] : Flow[vertex];
				Link[i] = vertex;
			}
		}
		used[vertex] = 1;
	}

	if (Link[target] == -1) return 0;
	CurVertex = target;
	while (CurVertex != source)
	{
		f[Link[CurVertex]][CurVertex] += Flow[target];
		CurVertex = Link[CurVertex];
	}
	cout << "\n" << Flow[target] << ": ";
	for (int i = 0; i < vertices; i++) cout << Link[i] << " ";
	return Flow[target];
}


class Edge
{
public:
	int from, to, weight;
	Edge(int from = -1, int to = -1, int weight = 0) : from(from), to(to), weight(weight) {}
	Edge(const Edge& E)
	{
		from = E.from;
		to = E.to;
		weight = E.weight;
	}
	int operator<(Edge& E)
	{
		return (weight < E.weight);
	}
	friend ostream& operator<<(ostream& s, Edge& e);
};

ostream& operator<<(ostream& s, Edge& e)
{
	s << "From: " << e.from << ", to: " << e.to << ", weight: " << e.weight;
	return s;
}

int main()
{

	vector<vector<int> > c =
	{ 
	{ 0, 5, 3, 1, 0, 3, 4, 3, 6, 0, 4 },
	{ 5, 0, 6, 7, 9, 4, 3, 3, 6, 9, 6 },
	{ 3, 6, 0, 4, 5, 1, 9, 5, 3, 1, 8 },
	{ 1, 7, 4, 0, 5, 5, 2, 4, 2, 5, 8 },
	{ 0, 9, 5, 5, 0, 3, 8, 2, 6, 4, 3 },
	{ 3, 4, 1, 5, 3, 0, 3, 2, 2, 2, 8 },
	{ 4, 3, 9, 2, 8, 3, 0, 7, 6, 7, 6 },
	{ 3, 3, 5, 4, 2, 2, 7, 0, 4, 3, 4 },
	{ 6, 6, 3, 2, 6, 2, 6, 4, 0, 7, 3 },
	{ 0, 9, 1, 5, 4, 2, 7, 3, 7, 0, 9 },
	{ 4, 6, 8, 8, 3, 8, 6, 4, 3, 9, 0 } 
	};

	/*vector<vector<int>> test = 
	{
	{ 0, 6, 1, 2, 0, 0},
	{ 6, 0, 5, 0, 3, 0},
	{ 1, 5, 0, 5, 6, 4},
	{ 2, 0, 5, 0, 0, 2},
	{ 0, 3, 6, 0, 0, 7},
	{ 0, 0, 4, 2, 7, 0},
	};*/

	
	vector<int> all_vertices = dfs(c);
	for (int i = 0; i < all_vertices.size(); i++)
		cout << all_vertices[i] << ' ';


	cout << '\n' << endl;
	vector<int> min_distances = Dijkstra(c, 0);
	for (int i = 0; i < min_distances.size(); i++)
		cout << min_distances[i] << ' ';

	vector<vector<int>> mat = Prim(c);
	cout << '\n' << endl;
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat.size(); j++)
			cout << mat[i][j] << ' ';
		cout << '\n';
	}

	//применение функций из пунктов 1 - 3 к минимальному остову
	cout << endl;
	vector<int> vertices = dfs(mat);
	for (int i = 0; i < vertices.size(); i++)
		cout << vertices[i] << ' ';
	cout << endl;
	vector<int> distances = Dijkstra(mat, 0);
	for (int i = 0; i < distances.size(); i++)
		cout << distances[i] << ' ';

	cout << '\n';
	vector<int> arr1 = degrees_of_vertices(mat);
	for (int i = 0; i < arr1.size(); i++)
		cout << arr1[i] << ' ';

	cout << '\n' << endl;
	cout << avg_degree(mat);
	return 0;
}
