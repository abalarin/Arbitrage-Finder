#include "Currency.h"

Currency::Currency(){
	NodeWeight = 0.0;
	ConversionRate = 1;
	Edge_Weight = INFINITY;
}

Currency::~Currency()
{
}

Currency::Currency(std::string code, double weight, double rate, double edgeWeight){
	CountryCode = code;
	NodeWeight = weight;
	ConversionRate = 1;
	Edge_Weight = INFINITY;
}
Currency::Currency(std::string code, int newPosition, double weight, double rate, double edgeWeight) {
	position = newPosition;
	CountryCode = code;
	NodeWeight = weight;
	ConversionRate = 1;
	Edge_Weight = INFINITY;
}

std::string Currency::getCountryName() const{
	return CountryCode;
}

double Currency::getNodeWeight() const{
	return NodeWeight;
}

std::vector<Currency::Edge> Currency::getEdges() const{
	return Edges;
}

int Currency::getPosition() const{
	return position;
}

void Currency::setCountryName(std::string country){
	CountryCode = country;
}

void Currency::setNodeWeight(double updatedWeight){
	NodeWeight = updatedWeight;
}
void Currency::setPosition(int newPosition){
	position = newPosition;
}
void Currency::insertEdge(Currency::Edge newEdge) {
	Edges.push_back(newEdge);
}

//void Currency::insertEdge(std::string countryCode, double nodeWeight, double rate, double edgeWeight) {
	//Edges.push_back({ countryCode, rate, edgeWeight, nodeWeight });
//}

void Currency::insertEdge(std::string countryCode, int index, double nodeWeight, double rate, double edgeWeight){
	Edges.push_back({countryCode, index, rate, edgeWeight, nodeWeight });
}

