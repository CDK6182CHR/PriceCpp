#pragma once
#include<iostream>
#include<stack>

template<class T> class Graph;
template<class T> std::ostream & operator<<(std::ostream & out, const Graph<T>& graph);

/**
 * 边类。其中T代表结点数据类型。
 * 注意：假定任意两点之间只有一条有向边。如果有多条，一般情况下仅考虑第一条边。
 * 此种情况下未必能保证正确性。
 */
template<class T>
struct Edge {
	int dest;
	double cost;
    T data;  //可选的数据，T应当有默认构造函数
	Edge<T>* link;
	Edge(int dst,double cst=-1.0):dest(dst),cost(cst),link(nullptr){}
    Edge(int dst,double cst,const T& dt):dest(dst),cost(cst),data(dt),link(nullptr){}
};

enum class Color {
	WHITE,
	GRAY,
	BLACK,
};

template<class T>
struct Vertex {
	T data;
	Edge<T>* adj;//链表实现
	int inDegree, outDegree;
	Vertex(T dt):adj(nullptr),data(dt),inDegree(0),outDegree(0){}
	~Vertex();
	bool addEdge(int dest, double cost = -1);
    bool addEdge(int dest, double cost, const T& data);
	bool delEdge(int dest);
};

template<class T>
class Graph {
protected:
	Vertex<T>** vertexTable;//动态数组实现
	int* est;//earliest finish time topology排序中的最早开始时间。
	int maxSize;
	int size;
public:
    using PVertex=Vertex<T>*;
    using PEdge=Edge<T>*;
    static const int INF = 0x7FFFFFFF;
	Graph(int mxsz = 16) :maxSize(mxsz), vertexTable(nullptr) ,est(nullptr),size(0)
	{ 
		vertexTable = new Vertex<T>*[mxsz]; 
	}
	~Graph() { delete[] vertexTable; }
	bool insertVertex(T data);
	bool removeVertex(T data);
	bool removeVertex(Vertex<T>* vtx);
	bool insertEdge(int from, int to,double cost=-1);
    bool insertEdge(int from, int to,double cost,const T& data);
	bool removeEdge(int from, int to);
    Edge<T>* shortestEdge(int from, int to);
	T getValue(int i)const;
	int getFirstNeighbor(int v)const;
	int getNextNeighbor(int v, int w)const;
	int getVertexPos(const T& data)const;
	int findOrCreateVertex(const T& data); //如果存在data结点则返回，不存在则创建一个再返回
	//friend std::ostream& operator<< <>(std::ostream& out, const Graph<T>& graph);
	void show()const;
//	template<class T>  //VS要这个，MinGW不能要这个
	friend std::ostream& operator<<(std::ostream& out, const Graph<T>& graph);
    void sssp(const T& source);//single-source-shortest-path
protected:
	void delVertex(int i);
	void dijkstra(int s, int* pi, double* heap)const;
	Edge<T>* getFirstEdge(int v);
	Edge<T>* getNextEdge(int v, int w);
	void calculateEst();
	void calEft(Color colors[],int eft[],int start, int& topNum);//DFS递归
    virtual void processSsspResult(int s,int pi[],double length[]);
private:
	static int getMin(double* heap, int n);
};

template<class T>
inline Vertex<T>::~Vertex()
{
	if (!adj)
		return;
	Edge<T>* cur = adj;
	Edge<T>* former = adj;
	while (cur->link) {
		former = cur;
		cur = cur->link;
		delete former;
	}
}

template<class T>
inline bool Vertex<T>::addEdge(int dest, double cost)
{
	Edge<T>* cur = adj;
	Edge<T>* new_edge = new Edge<T>(dest, cost);
	if (!new_edge)
		return false;
	if (!adj)
		adj = new_edge;
	else {
		while (cur->link)
			cur = cur->link;
		cur->link = new_edge;
	}
	outDegree++;
	return true;
}

template<class T>
inline bool Vertex<T>::addEdge(int dest, double cost, const T& data)
{
    Edge<T>* cur = adj;
    Edge<T>* new_edge = new Edge<T>(dest, cost, data);
    if (!new_edge)
        return false;
    if (!adj)
        adj = new_edge;
    else {
        while (cur->link)
            cur = cur->link;
        cur->link = new_edge;
    }
    outDegree++;
    return true;
}

template<class T>
inline bool Vertex<T>::delEdge(int dest)
{
	Edge<T>* cur = adj, *former = adj;
	if (!adj)
		return false;
	while (cur&&cur->dest!=dest) {
		former = cur;
		cur = cur->link;
	}
	if (!cur)
		return false;
	former->link = cur->link;
	outDegree--;
	return true;
}

template<class T>
inline bool Graph<T>::insertVertex(T data)
{
	if (size >= maxSize)
		return false;
	vertexTable[size] = new Vertex<T>(data);
	size++;
	return true;
}

template<class T>
inline bool Graph<T>::removeVertex(T data)
{
	for (int i = 0; i < size; i++) {
		if (vertexTable[i]->data == data) {
			delVertex(i);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool Graph<T>::insertEdge(int from, int to,double cost)
{
	if(from>=size||to>=size)
		return false;
	bool flag = vertexTable[from]->addEdge(to, cost);
	if (flag)
		vertexTable[to]->inDegree++;
	return flag;
}

template<class T>
inline bool Graph<T>::insertEdge(int from, int to,double cost,const T& data)
{
    if(from>=size||from<0||to<0||to>=size)
        return false;
    bool flag = vertexTable[from]->addEdge(to,cost,data);
    if (flag)
        vertexTable[to]->inDegree++;
    return flag;
}

template<class T>
inline Edge<T>* Graph<T>::shortestEdge(int from, int to)
{
    if(from>=size||from<0||to<0||to>=size)
        return nullptr;
    PVertex pv=vertexTable[from];
    PEdge pe=pv->adj,
          curMinEd=nullptr;  //当前最短的符合条件的边
    double curMinLen=INF;
    while(pe){
        if(pe->dest==to){
            if(pe->cost<curMinLen){
                curMinLen=pe->cost;
                curMinEd=pe;
            }
        }
        pe=pe->link;
    }
    return curMinEd;
}

template<class T>
inline bool Graph<T>::removeEdge(int from, int to)
{
	if (from >= size || to >= size)
		return false;
	bool flag = vertexTable[from]->delEdge(to);
	if (flag)
		vertexTable[to]->inDegree--;
	return flag;
}

template<class T>
inline T Graph<T>::getValue(int i) const
{
	//precondition: i is existed.
	return vertexTable[i]->data;
}

template<class T>
inline int Graph<T>::getFirstNeighbor(int v) const
{
	if (v < 0 || v >= size)
		return -1;
	Edge<T>* pEdge = vertexTable[v]->adj;
	if (!pEdge)
		return -1;
	else
		return pEdge->dest;
}

template<class T>
inline int Graph<T>::getNextNeighbor(int v, int w) const
{
	if (v < 0 || v >= size || w<0 || w>size)
		return -1;
	Edge<T>* edge = vertexTable[v]->adj;
	while (edge->link) {
		if (edge->link->dest == w)
			break;
		edge = edge->link;
	}
	if (!(edge->link))
		return -1;
	else
		return edge->link->dest;
}

/**
 * Find vertex index.
 * If vertex does not exist, return -1.
 */
template<class T>
inline int Graph<T>::getVertexPos(const T & data) const
{
	for (int i = 0; i < size; i++)
		if (vertexTable[i]->data == data)
			return i;
	return -1;
}

template<class T>
inline int Graph<T>::findOrCreateVertex(const T& data)
{
	for (int i = 0; i < size; i++) {
		Vertex<T>* v = vertexTable[i];
		if (v->data == data)
			return i;
	}
	insertVertex(data);
	return size - 1;
}

template<class T>
std::ostream & operator<<(std::ostream & out, const Graph<T>& graph)
{
	for (int i = 0; i < graph.size; i++) {
		out << "Vertex" << i << " data:" << graph.vertexTable[i]->data << 
			" In "<<graph.vertexTable[i]->inDegree<<" Out "<<graph.vertexTable[i]->outDegree
			<<" Edges:";
		Edge<T>* pEdge = graph.vertexTable[i]->adj;
		while (pEdge) {
			out << "dest:" << pEdge->dest << " cost: " << pEdge->cost << " | ";
			pEdge = pEdge->link;
		}
		out << '\n';
	}
	return out;
}

template<class T>
inline void Graph<T>::show() const
{
#include<iostream>
	using namespace std;
	cout << *this << endl;
}

template<class T>
inline void Graph<T>::delVertex(int n)
{
	//precondition: i指向的顶点存在
	if (size > 1) {
		for (int i = 0; i < size; i++) {
			if (i == n)
				continue;
			Edge<T>* adj = vertexTable[i]->adj, *former = vertexTable[i]->adj;
			while (adj) {
				if (adj->dest == n) {
					//删除此节点
					if (adj == vertexTable[i]->adj) {
						//头结点
						vertexTable[i]->adj = adj->link;
					}
					else {
						former->link = adj->link;
					}
					//former = adj; //adj已经被删掉，不能作为former
					Edge<T>* temp = adj;
					adj = adj->link;
					delete temp;
					vertexTable[i]->outDegree--;
				}
				else if (adj->dest == size - 1) {
					adj->dest = n;
					former = adj;
					adj = adj->link;
				}
				else {
					former = adj;
					adj = adj->link;
				}
			}
		}
		delete vertexTable[n];
		vertexTable[n] = vertexTable[--size];
		vertexTable[size] = nullptr;
	}
	else {
		delete vertexTable[n];
		vertexTable[n] = nullptr;
	}
}

/*
 * Dijkstra最短路径算法的实现，pi是当前结点的前驱结点编号，heap记载各个结点到源点的距离.
 * pi和heap的范围都是当前结点数size。
 * heap初始为INF，若删除则为-1.
 */
template<class T>
inline void Graph<T>::dijkstra(int s,int * pi, double * miles) const
{

	double* heap = new double[size];
	for (int i = 0; i < size; i++)
		pi[i] = -1, heap[i] = INF, miles[i] = INF;
	heap[s] = 0; miles[s] = 0;
	int count = 0;
	while (count < size) {
		int v = getMin(heap, size);//取队首元素
		Edge<T>* pEdge = vertexTable[v]->adj;
		while (pEdge) {
			int w = pEdge->dest;
			double cost = pEdge->cost;
			if (miles[w] > cost+miles[v]) {
				heap[w] = cost + heap[v];
				miles[w] = cost + miles[v];
				pi[w] = v;
			}
			pEdge = pEdge->link;
		}
		heap[v] = -1; count++;
	}
}

template<class T>
inline Edge<T>* Graph<T>::getFirstEdge(int v)
{
	if (v < 0 || v >= size)
		return nullptr;
	return vertexTable[v]->adj;
}

template<class T>
inline Edge<T>* Graph<T>::getNextEdge(int v, int w)
{
	if (v < 0 || v >= size)
		return nullptr;
	Edge<T>* remAdj = vertexTable[v]->adj;
	while (remAdj->link) {
		if (remAdj->dest == w)
			return remAdj->link;
		remAdj = remAdj->link;
	}
	return nullptr;
}

/*
用topology排序方法计算最早完成时间earliest finish time
*/
template<class T>
inline void Graph<T>::calculateEst()
{
	Color* colors = new Color[size];
	if (est)delete[] est;
	est = new int[size];
	int topoNum = 0;
	for (int i = 0; i < size; i++) {
		colors[i] = Color::WHITE;
		est[i] = INF;
	}
	for (int i = 0; i < size; i++) {
		Vertex<T>* vertex = vertexTable[i];
		if (colors[i] == Color::WHITE) {
			calEft(colors, est, i, topoNum);
		}
	}
	delete[] colors;
}

template<class T>
inline void Graph<T>::calEft(Color colors[], int est[], const int start, int& topNum)
{
	colors[start] = Color::GRAY;
	int eft = 0;//所依赖的任务的最早完成时间，或者说所依赖的任务中完成时间最晚的一个
	Edge<T>* pEdge = vertexTable[start]->adj;
	while(pEdge) {
		int dest = pEdge->dest;
		if (colors[dest] == Color::WHITE) {
			calEft(colors, est, dest, topNum);
		}
		else if (colors[dest] == Color::GRAY) {
			std::cout << "疑似非DAG图 " << vertexTable[start]->data << vertexTable[dest]->data << std::endl;
		}
		int finish_time = est[dest] + pEdge->cost;//上一个任务的终止时间
		eft = eft > finish_time ? eft : finish_time;
		pEdge = pEdge->link;
	}
	colors[start] = Color::BLACK;
	est[start] = eft;
}


/**
 * 返回heap中最小数据的编号，除了-1。
 * 注意和minCost的比较必须用<=。否则，如果出现不连通的情况，
 * 则INF没有有效处理导致返回-1.
 */
template<class T>
inline int Graph<T>::getMin(double * heap, int n)
{
	int minIndex=-1; double minCost = INF;
    for (int i = 0; i < n; i++)
        if (heap[i] >= 0 && heap[i] <= minCost)
			minIndex = i, minCost = heap[i];
    if(minIndex==-1){
        int x=1;
    }
	return minIndex;
}

template<class T>
inline void Graph<T>::sssp(const T &source)
{
    using namespace std;
    int s = this->getVertexPos(source);
    if (s < 0 || s >= size) {
        cerr << "No such station!" << endl;
        return;
    }
    int* pi = new int[size];
    double* miles = new double[size];
    dijkstra(s, pi, miles);

    processSsspResult(s,pi,miles);

    delete[] pi;
    delete[] miles;
}

/**
 * default implement
 * output length and show paths
 */
template<class T>
inline void Graph<T>::processSsspResult(int s,int pi[],double length[])
{
//    using namespace std;
//    for (int i = 0; i < size; i++) {
//        stack<T> via;
//        int v = pi[i];
//        while (v!=-1) {
//            if (vertexTable[v]->outDegree > 2)
//                via.push(vertexTable[v]->data);
//            v = pi[v];
//        }
//        while (!via.empty()) {
//            cout << via.top() << ' ';
//            via.pop();
//        }
//        cout << endl;
//    }
}
