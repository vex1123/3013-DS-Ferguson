/**
* This file is a kind of a facade that interfaces with a more complex json
* class. The file `json.hpp` is needed for this helper to work. This facade
* strips away 99.9% of the power the original file does.
*
* The `json.hpp` source code was obtained from https://github.com/nlohmann/json and was
* authored by: Niels Lohmann
*
* For our purposes it assumes an array of objects. I might expand on it later but doubt
* it. If you are interested, learn how to use the original in many much more powerful ways.
*/
#pragma once
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

#include "json.hpp"

/*
	Edited by Ryan Ferguson.
	Added priority variable to jsonhelper/animal class
	and coressponding get function
*/


using namespace std;

using json = nlohmann::json;

class JsonHelper {
private:
	json Data;
	int Size;
	int Index;
	string FileName;
	int priority;
public:
	JsonHelper(string filename) {
		FileName = filename;

		ifstream input(FileName);
		input >> Data;
		input.close();

		Size = Data.size();
		Index = 0;
	}

	json getNext() {
		if (Index < Size) {
			return Data[Index++];
		}
		return NULL;
	}

	json getIth(int i) {
		if (i < Size) {
			return Data[i];
		}
		return NULL;
	}

	/**
	* pushBack: converts a string to json and pushes on end of array
	*
	* Params:
	*     string obj  : a string json object which whould be in the following format:
	*
	*                           "{\"key0\":"value0",\"key1\":1,\"keyn\":\"valuen"\}"
	* Returns:
	*     void
	*
	*/
	void pushBack(string obj) {

		json j = json::parse(obj);
		Data.push_back(j);
		cout << Data.dump(4) << endl;
	}

	int getSize() {
		return Size;
	}

	void saveFile(string filename = "") {
		if (filename == "") {
			filename = FileName;
		}

		ofstream out(filename);
		out << std::setw(4) << Data << endl;
		out.close();
	}

	//simple get to acess prority number
	int getPriority()
	{
		return priority;
	}
};
