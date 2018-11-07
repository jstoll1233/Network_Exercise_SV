#include "network.h"
#include "random.h"
#include <iostream>

RandomNumbers rnum;

void Network::resize(const size_t& n){
		values.resize(n);
		rnum.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b){
		
	auto range = links.equal_range(a);
	for(auto i(range.first); i != range.second; ++i) {
		if (i->second == b) {return false;}
	}
	
	if ((a >= 0) and (values.size() > a) and (b >= 0) and (values.size() > b) and (a!=b)){
	links.insert(std::make_pair(a,b));
	links.insert(std::make_pair(b,a));
	return true;
	}
	else {return false;}
}

size_t Network::random_connect(const double& n){
	links.clear();
	
	
	std::vector<int>nb_links(values.size());
	
	rnum.poisson(nb_links, n);
	
	std::vector<size_t>to_shuffle(values.size());
	
	for(size_t k=0; k < values.size(); ++k) {
		
		to_shuffle[k] = k;
		
	}
	
	
	for(size_t i = 0; i< values.size(); ++i ) {
		
		rnum.shuffle(to_shuffle);
		
		size_t s = nb_links[i];
		
		for(size_t j = 0; j< s ; ++j){
			

			size_t m = to_shuffle[j];
			
			
		if (add_link(i, m)){
			
			//++created_links;
		}
		else if (not add_link(i, m)) { 
			add_link(i, m+1);
			
		}
			
		else  {
			add_link(i, m-1);
			
		}
					
		}
	}
		
	return links.size()/2;
}

size_t Network::set_values(const std::vector<double>& n){
	
	size_t nb_reset = 0;
	if (n.size() <= values.size()){
		for (size_t i(0); i < n.size(); ++i) {
			values[i] = n[i];
			++nb_reset;
		}
	}
	
	if (n.size() > values.size()){
		for (size_t i(0); i < values.size(); ++i) {
			values[i] = n[i];
			++nb_reset;
		}
	}
	
	return nb_reset;
}

size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t & n) const {
	if (values.size() > n){
	return links.count(n);
	}
	else {
		throw std::out_of_range("node not found in network");
	}
	
}

double Network::value(const size_t & n) const{
	if (values.size() > n) {
		return values[n];
	}
	else {
		throw std::out_of_range("node not found in network");
	}
	
}

std::vector<double> Network::sorted_values() const{
	std::vector<double>sorted = values;
	std::sort(sorted.begin(), sorted.end());
	std::reverse(sorted.begin(), sorted.end());
	return sorted;
}

std::vector<size_t> Network::neighbors(const size_t& n) const{
	std::vector<size_t> neighbors;
	if (n < values.size()){
		auto neighbor = links.equal_range(n);
		for (auto i = neighbor.first; i != neighbor.second; ++i) {
		neighbors.push_back(i->second);
		}
	}
	return neighbors;
}

