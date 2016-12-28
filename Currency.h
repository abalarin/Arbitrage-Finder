#pragma once

#include<string>
#include<vector>

class Currency
{
public:
	struct Edge {
		std::string C;
		int I;
		double R, W, V;
	};

	Currency();
	~Currency();
	//Object for details of Edges

	//Overloaders
	Currency(std::string, double, double, double);
	Currency(std::string, int, double, double, double);

	//Getters
	std::string getCountryName() const;
	double getNodeWeight() const;
	std::vector<Edge> getEdges() const;
	int getPosition() const;

	//Setters
	void setCountryName(std::string) ;
	void setNodeWeight(double);
	void setPosition(int);
	void insertEdge(Currency::Edge newEdge);
	//void insertEdge(std::string, double, double, double);//Deprecated
	void insertEdge(std::string, int, double, double, double);

private:

	std::string CountryCode;
	int position; //Position relaive to a graph
	double NodeWeight;

	// Next 2 represent and egde from itself to itself
	double ConversionRate;	//For itself it will always be 1
	double Edge_Weight;	//For itself it will always be INF

	std::vector<Edge> Edges;

	//std::vector < std::tuple<std::string, double, double, double>> Edges;
};

