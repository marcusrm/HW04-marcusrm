
#pragma once
#include "Starbucks.h"
#include <iostream>
#include <fstream>
#include <vector>

class marcusrmStarbucks : public Starbucks {
public:

	//constructor
	marcusrmStarbucks();


	//destructor
	~marcusrmStarbucks();

	/*
	 * add all entries in the array to your data structure
	 *
	 * The "c" parameter is a pointer to an array of all the entries to be added, and n is the length of
	 * the array.
	 *
	 * Note: If you detect that two items have the same coordinates, then do not add the new item
	 *       that has the same coordinates as another item. This is guaranteed to happen, by the way,
	 *       because some Starbucks locations are listed in the database twice. We will define two locations
	 *       to be the "same location" if both |x1 - x2| <= 0.00001 and |y1 - y2| < 0.00001
	 */

	void build(Entry* c, int n);
	
	/*
	 * Return a pointer to the entry that is closest to the given coordinates. Your
	 *  answer may be approximate, but then you will lose points on the "Accuracy" quality measure
	 */
	Entry* getNearest(double x, double y);

	//insert a node into the tree of locations
	void insert(Entry* current);

	

	//These methods use the unsorted array approach to find the correct position.
	void insertSlow(Entry* current);

	Entry* getNearestSlow(double x, double y);


};

//imports the data from a file into an array called 'locations'
	void importData(vector<Entry>* locations, string fileName);