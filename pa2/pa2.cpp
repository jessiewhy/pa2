#include "pa2.h"
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

//start list implementation 
list::list() {
	head = NULL;
	tail = NULL;
	for (int i = 0; i < 32; ++i) {
		insert("Free");
	}
}

void list::insert(string value) {
	if (head == NULL) {
		head = new Page(value, 0);
		return;
	}
	head->createPage(value);
}

void list::add(string pName, int pSize, string pAlg) {
	int availMem = head->checkAvailMem(pName);
	bool consecSpace = head->checkConsecSpaces(pSize, pName);

	if (availMem == -1) {
		cout << '\n' << "Error, Program " << pName << " is already running." << '\n';
		cout << endl;
		return;
	}
	else if (availMem < pSize || consecSpace==false ) {
		cout << '\n' << "Error, Not enough memory for Program " << pName << '\n';
		cout << endl;
		return;
	}
	if (pAlg == "worst") {
		head->worstAlg(pName, pSize);
	}
	else {
		head->bestAlg(pName, pSize, 0);
	}
}

void list::pKill(string pName) {
	head->pkill(pName, 0);
}

void list::pFrag() {
	head->pFrag(0);
}

void list::display() {
	head->display(1);
}

//start "page" implementation
Page::Page(string pName, int pSize) {
	this->pName = pName;
	this->pSize = pSize;
	next = NULL;
}

void Page::createPage(string value) {
	if (next == NULL) {
		next = new Page(value, 0);
		return;
	}
	next->createPage(value);
}

void Page::bestAlg(string pName, int pSize, int i) {
	if (this->pName == "Free") {
		this->pName = pName;
		++i;
		if (pSize > 4) {
			bestAlg(pName, pSize - 4, i);
			this->pSize = 4;
		}
		else {
			this->pSize = pSize;
			cout << '\n' << "Program " << pName << " added successfully: " << i << " page(s) used." << '\n' << endl;
		}
		return;
	}
	next->bestAlg(pName, pSize, i);

}

void Page::worstAlg(string pName, int pSize) {
	Page * temp = this;
	Page * newTemp = this;
	string tempHole[32];    //temp holds linked list so we can find index of length of holes
	int tempLengthHole[32];
	int startPositionHolder[32];
	int startIndex = 0;   //when a hole starts
	int endIndex = 0;    //when a hole ends
	int freeCounter = 0;
	int fragmentCounter = 0;
	int max = 0;   //keeps track of length of holes...looking for largest hole
	
	//storing linked list into array temporarily to find start and end of each hole in linkedlist
	while (temp != NULL) {
		for (int i = 0; i < 32; ++i) {
			tempHole[i] = temp->pName;
			temp = temp->next;
		}
	}

	//knows which path to take depending on how many starting fragments list has
	for (int i = 0; i < 32; ++i) {
		if (tempHole[i] == "Free" &&tempHole[i + 1] != "Free") {
			++fragmentCounter;
		}
	}
	cout << fragmentCounter << endl;
	
	//for one fragment
	if (fragmentCounter == 1) {
		while (newTemp->pName != "Free") {
			newTemp = newTemp->next;
		}
		for (int i = 0; i < pSize; ++i) {
			newTemp->pName = pName;
			newTemp = newTemp->next;

		}

	}
	//for more than one fragment
	else {
		for (int i = 0; i < fragmentCounter; ++i) {
			for (int j = startIndex; j < 32; ++j) {
				//case 1: until we reach a free
				if (tempHole[j] != "Free" && tempHole[j + 1] == "Free") {
					startIndex = j + 1;   //start index of hole
					int k = startIndex;
					while (tempHole[k] == "Free") {
						++freeCounter;    //length of current hole
						++k;
					}
					endIndex = k;    //where hole ended
					startPositionHolder[j] = startIndex;   //at j index, hole began 
					tempLengthHole[j] = freeCounter;      //at j index, hole is length of freeCounter

				}
				//case 2: when we start at a free
				else if (tempHole[j] == "Free" && j == 0) {
					startIndex = j;
					int k = startIndex;
					while (tempHole[k] == "Free") {
						++freeCounter;
						++k;
					}
					endIndex = k;   //where hole ended
					startPositionHolder[j] = startIndex;   //at j index, hole began 
					tempLengthHole[j] = freeCounter;      //at j index, hole is length of freeCounter
				}
				startIndex = endIndex;       //~keeping track of where to pick up next hole from
			}
		}
		//find largest length hole
		for (int i = 0; i < fragmentCounter; ++i) {
			if (tempLengthHole[i] > max) {
				max = tempLengthHole[i];   //max hole
				startIndex = startPositionHolder[i];   //position of max hole 
			}
		}
		//get to position in linked list of hole
		//case 1: starts at beginning of list
		if (startIndex == 0) {
			newTemp->pName = pName;
			for (int i = 0; i < pSize; ++i) {
				newTemp->pName = pName;
			}
		}
		//case 2: starts within list
		else {
			for (int i = 0; i < startIndex; ++i) {
				newTemp = newTemp->next;
			}
			//add program to 
			for (int i = 0; i < pSize; ++i) {
				newTemp->pName = pName;
			}
		}
	}
	cout << '\n' << "Program " << pName << " added successfully: " << pSize << " page(s) used." << '\n' << endl;
}

int Page::findContSize(string searchingFor, Page * Iterator) {
	int count = 0;
	while (Iterator!=NULL && Iterator->pName== searchingFor) {
		++count;
		Iterator->next;
	}
	return count;
}

void Page::pkill(string pName, int i) {
	Page * temp = this;
	if (this->pName == pName) {
		this->pName = "Free";
		pSize = 0;
		++i;
	}
	if (next == NULL) {
		cout << '\n' << "Program " << pName << " successfully killed, " << i << " pages reclaimed." << '\n' << endl;
		return;
	}
	else {
		next->pkill(pName, i);
	}
}

void Page::pFrag(int i) {
	Page * temp = this;
	while (temp->next != NULL) {
		if ((temp->next == NULL || temp->next->pName == "Free") && (temp->pName != "Free")) {
			++i;
		}
		temp = temp->next;
	}
	cout << '\n' << "There are " << i << " fragment(s)." << '\n' << endl;
}

void Page::display(int pIndex) {
	Page * temp = this;
	int fCounter = 0;
	cout << endl;
	while (temp != NULL) {
		cout << temp->pName << " ";
		temp = temp->next;
		++fCounter;
		if (fCounter % 8 == 0) {
			std::cout << endl;
		}
	}
	cout << endl;
}

int Page::checkAvailMem(string pName) {
	Page * temp = this;
	int i = 0;

	while (temp != NULL) {
		if (temp->pName == pName) {
			return -1;
		}
		else if (temp->pSize == 0) {
			i += 4;
		}
		temp = temp->next;
	}

	return i;
}

bool Page::checkConsecSpaces(int pSize, string pName) {
	Page * temp = this;
	Page * current = this; //assuming already at head
	temp = current;
	int count = 0;
	for(int i = 0; i<pSize; i++)
	{
		if (temp->pName == "Free"||temp->pName!=this->pName) {
			while (temp->next != NULL) {
				if(temp->pName=="Free"){
					count++;
				}
				temp = temp->next;
				if (count >= pSize) {
					return true;
				}
				else { false; }
				
			}
		}
		current = current->next;
		temp = current;
	}
}

void myMenu(int *myChoice)
{
	while (!((*myChoice == 1) || (*myChoice == 2) || (*myChoice == 3) || (*myChoice == 4) || (*myChoice == 5))) {
		cout << "1. Add program" << endl;
		cout << "2. Kill program" << endl;
		cout << "3. Fragmentation" << endl;
		cout << "4. Print memory" << endl;
		cout << "5. Exit" << endl;
		cout << endl;
		cout << "choice - ";
		cin >> *myChoice;
		if (!((*myChoice == 1) || (*myChoice == 2) || (*myChoice == 3) || (*myChoice == 4) || (*myChoice == 5))) {
			cout << endl;
			cout << "Error." << endl;
			cin.clear();
			cin.ignore(1997, '\n');
			cout << endl;
		}
	}
}

int main(int argc, char * argv[])
{
	list * linkedList = new list();
	string alg = "worst";

	string pName;
	int pSize;
	int myInput;
	int * myChoice = &myInput;
	if (argc == 2) {
		if (argv[1] == "worst") {
			string alg = "worst";
			cout << "Using worst algorithm" << endl;
		}
		if (argv[1] == "best") {
			string alg = "best";
			cout << "Using best algorithm" << endl;
		}
		else {
			cout << "Invalid or missing arguments. Using default: best algorithm." << endl;
		}
	}


	do {
		myMenu(myChoice);
		switch (*myChoice) {
		case 1: {
			cout << "Program name: ";
			cin >> pName;
			cout << "Program size (KB) - ";
			cin >> pSize;
			int pageSize = 0;
			if (pSize % 4 != 0) {
				pageSize = (pSize / 4) + 1;
			}
			else {
				pageSize = pSize / 4;
			}
			pSize = pageSize;
			linkedList->add(pName, pSize, alg);
			*myChoice = 0;
			break;
		}
		case 2: {
			pName = "";
			cout << "Program name - ";
			cin >> pName;
			linkedList->pKill(pName);
			*myChoice = 0;
			break;
		}
		case 3: {
			linkedList->pFrag();
			*myChoice = 0;
			break;
		}
		case 4: {
			linkedList->display();
			*myChoice = 0;
			break;
		}
		case 5: {
			return 0;
		}
		}
	} while ((*myChoice == 0));
	system("pause");
	return 0;
}

