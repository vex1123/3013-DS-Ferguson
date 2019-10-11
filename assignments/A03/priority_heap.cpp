/*
 *
 * Name: Ryan Ferguson
 * Date: 10/11/2019
 * Course: CMPS 3013 MW 3- 4:20
 * Program: A03
 * Description:
 *
 *	   This program reads data from a .json file, and uses it to create
 *	   an array of Animal pointers. It initializes a heap full of animal
 *	   arrays as a size of 1000. Then the program calls a funtion to insert
 *	   the Animal information from the .json file into the heap. It then
 *	   extracts the top 5 prioty animals form each created heap
 */


#include "json_helper.cpp"
#include <fstream>
#include <iostream>


#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}

class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float adjuster;
	bool validated;
	string version;
	float priority;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		adjuster = 0.0;
		validated = 0;
		version = "";
		priority = 0.0;
	}

	Animal(string name, long _date, double lat, double lon, float adj, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		adjuster = adj;
		validated = val;
		version = ver;
		priority = 0.0;
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		adjuster = j["adjuster"];
		validated = j["validated"];
		version = j["version"];
		priority = 0.0;
	}
};

class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;
	int size;

public:
	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
			Animals[i]->priority = (6372.8 - Animals[i]->date) *Animals[i]->adjuster / Animals[i]->latitude;
			if (Animals[i]->validated == true)
			{
				Animals[i]->priority *= -1;
			}
		}
	}



	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->adjuster << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version << " "
				<< Animals[i]->priority << endl;

		}
	}

	Animal** getAnimalPointer(AnimalHelper x)
	{
		return x.Animals;
	}
};


///////////////////////////////////////////////////////////////////////////////
//                   YOU MUST COMPLETE AND COMMENT YOUR CODE!
// Title:            (program's title)
// Files:            (list of source files)
// Semester:         (course) Spring 2018
//
// Author:           (your name)
// Email:            (your email address)
// Description:
//       describe program here thoroughly
/////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <iostream>
#include <math.h>

#include "json_helper.cpp"
#include "json.hpp"


using namespace std;
using namespace std::chrono;

/**
* Class Heap:
*
* Public Methods:
*    Heap(int)
*    void Insert(int)
*    int Remove()
*    void PrintHeap()
*    int Size()
*    bool Empty()
*    void Heapify(int*,int)
*/
class Heap {
private:
	Animal **A;       // Pointer to allocate dynamic array
	int Next;     // Next available location
	int MaxSize;  // Max size since were using array
	int HeapSize; // Actual number of items in the array.
	int size;
	/**
	* Function IncreaseKey:
	*      Bubbles element up from given index.
	*
	* Params:
	*     [int] index  - index of item to be increased
	* Returns
	*      void
	*/
	void BubbleUp(int i) {
		int p = Parent(i);
		while (p > 0 && A[i]->priority > A[p]->priority) {
			Swap(i, p);
			i = p;
			p = Parent(i);
		}
	}

	/**
	* Function DecreaseKey:
	*      Bubbles element down from given index.
	*
	* Params:
	*      [int] index - index of item to be decreased
	* Returns
	*      void
	*/
	void BubbleDown(int i) {
		int c = PickChild(i);

		while (c > 0) {
			if (A[i]->priority < A[c]->priority) {
				Swap(i, c);
				i = c;
				c = PickChild(i);
			}
			else {
				c = -1;
			}
		}
	}

	/**
	* Function Swap:
	*      Swaps two elements in an array
	*
	* Params:
	*      [int] index1 - index of item to swap with
	*      [int] index2 - same
	* Returns
	*      void
	*/
	void Swap(int p, int i) {
		Animal* temp = A[p];
		A[p] = A[i];
		A[i] = temp;
	}

	/**
	* Function Parent:
	*      Returns parent index of a given index
	*
	* Params:
	*      [int]index - index to calculate parent from
	* Returns
	*      index [int]
	*/
	int Parent(int i) {
		return int(i / 2);
	}

	/**
	* Function LeftChild:
	*      Returns left index of a given index
	*
	* Params:
	*      [int] index - index to calculate child from
	* Returns
	*      [int] index - left child index
	*/
	int LeftChild(int i) {
		return i * 2;
	}

	/**
	* Function RightChild:
	*      Returns right index of a given index
	*
	* Params:
	*      [int] index - index to calculate child from
	* Returns
	*      [int] index - right child index
	*/
	int RightChild(int i) {
		return i * 2 + 1;
	}

	/**
	* Function PickChild:
	*      Return index of child to swap with or -1 to not swap.
	*
	* Params:
	*      [int] index - index of parent element
	* Returns
	*      [int] index - index to swap with or -1 to not swap
	*/
	int PickChild(int i) {
		if (RightChild(i) >= Next) {    //No right child
			if (LeftChild(i) >= Next) { //No left child
				return -1;
			}
			else { //you have a left no right
				return LeftChild(i);
			}
		}
		else {
			//right child exists
			if (A[RightChild(i)]->priority > A[LeftChild(i)]->priority) {
				return RightChild(i);
			}
			else {
				return LeftChild(i);
			}
		}
	}

public:
	/**
	* Function Heap:
	*      Constructor that allocates memory for array and
	*      inits vars. Creates Animal array of int size
	*
	* Params:
	*      int size
	* Returns
	*      void
	*/
	Heap(int size) {
		A = new Animal*[size];
		Next = 1;
		MaxSize = size;
		HeapSize = 0;
	}

	/**
	* Function Insert:
	*      Insert Animal into heap.
	*
	* Params:
	*        Animal *
	* Returns
	*      void
	*/
	void Insert(Animal *x) {
		A[Next] = x;
		BubbleUp(Next);
		Next++;
		HeapSize++;
	}

	int getMaxSize(Heap x)
	{
		return x.MaxSize;
	}

	/**
	* Function Extract:
	*      Removes top Animal priority from heap (whether min or max).
	*	   
	* Params:
	*      void
	* Returns
	*      [int] top_value - top value in the heap (min or max)
	*/
	Animal Extract() {


		Animal retval = *A[1];
		A[1] = A[--Next];
		HeapSize--;

		if (HeapSize > 1) {
			BubbleDown(1);
		}

		return retval;
	}

	/**
	* Function PrintHeap:
	*      Prints the values currently in the heap array
	*      based on array location, not heap order
	*
	* Params:
	*      void
	* Returns
	*      void
	*/
	void PrintHeap() {
		for (int i = 1; i < Next; i++) {
			cout << A[i] << " ";
		}
		cout << endl;
	}

	/**
	* Function Size:
	*      Returns the number of items in the heap
	*
	* Params:
	*      void
	* Returns
	*      [int] heapSize - size of heap
	*/
	int Size() {
		return Next - 1;
	}

	/**
	* Function Empty:
	*      Returns boolean true of array is empty
	*
	* Params:
	*      void
	* Returns
	*      [bool] empty - is array empty
	*/
	bool Empty() {
		return Next == 1;
	}

	/**
	* Function Heapify:
	*      Creates a heap out of a given array of animals in no specific order.
	*
	* Params:
	*      Animal *array
	*	   int size
	* Returns
	*      void
	*/

	void Heapify(Animal *A, int size) {
		int i = size / 2;
		// H = A;
		// Next = size;
		// HeapSize = size - 1;

		for (int j = i; j >= 1; j--) {
			BubbleDown(j);
		}
	}
};

/**
	* Function top5:
	*      extracts the top 5 animals in order of priority
	*	   and sends thier values to an outfile
	* Params:
	*      Heap **&, Animal **&, int, ofstream&  
	* Returns
	*      void
	*/
void top5(Heap **&Buckets, Animal **&Animals, int NumBuckets, ofstream &outfile) {
	Animal temp;
	outfile << "Ryan Ferguson" << endl;
	for (int i = 0; i < NumBuckets; i++) {
		outfile << "Heap # " << i + 1 << endl;
		outfile << "-------------" << endl;
		for (int j = 0; j < 5; j++) {
			temp = Buckets[i]->Extract();
			outfile << temp.animal_name << " " << temp.priority << endl;
		}
		outfile << endl;
	}
}

/**
	* fillBuckets:
	*      Initializes each bucket with the animal array
	*
	* Params:
	*      Heap **&, Animal**&, int size, int numBuckets
	* Returns
	*      void
	*/
void fillBuckets(Heap **&Buckets, Animal **&Animals, int size, int NumBuckets) {
	int which;
	for (int i = 0; i < size; i++) {
		which = abs(Animals[i]->date) % NumBuckets;
		Buckets[which]->Insert(Animals[i]);
	}
}


int main(int argc, char **argv) {
	//int size = 3;
	//debugging code for non variable testing
	ofstream outfile;
	outfile.open("out.txt");
	

	//accept variable of 3- 11 and create that ubmer of buckets
	int size;
	if (argc > 1) {
		size = atoi(argv[1]);
	}
	else {
		cout << "ERROR: no integer 3 - 11 entered" << endl;
		exit(0);
	}
	
	AnimalHelper AH("animals.json");

	int which;
	int total = 1000;
	Heap **Buckets;
	Buckets = new Heap*[size];
	Animal ** animals = AH.getAnimalPointer(AH);
	
	for (int i = 0; i < size; i++) {
		Buckets[i] = new Heap(1000);
	}

	//fill the buckets
	fillBuckets(Buckets, animals, total, size);
	
	//extract top 5 prioritys
	top5(Buckets, animals, size, outfile);

	return 0;
}
