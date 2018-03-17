#include "pa2.h"
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

/*START LIST IMPLEMENATION*/
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

//ADDING P2 AGAIN EVEN THO P2 IS ALREADY RUNNING
void list::add(string pName, int pSize, string pAlg) {
	bool checkPRun = head->checkProgramRunning(pName);

	if (checkPRun == true) {
		cout << '\n' << "Error, Program " << pName << " is already running." << '\n' << endl;
		cout << endl;
		return;
	}
	
	//see if biggest hole can fit reqest
	if (pAlg == "worst") {
		bool worstHole = head->checkHolesWorst(pName, pSize);
		if (worstHole == true) {
			head->worstAlg(pName, pSize);
		}
		else {
			cout << '\n' << "Error, Not enough memory for Program " << pName << '\n' << endl;
		}
	}
	//FIXME: see if smallest hole can fit request
	if (pAlg == "best") {
		bool bestHole = head->checkHolesBest(pName, pSize);
		if (bestHole == true) {
			head->bestAlg(pName, pSize);
		}
		else {
			cout << '\n' << "Error, Not enough memory for Program " << pName << '\n' << endl;
		}
	}
}

//FIXME: NOT KILLING PROPERLY
void list::pKill(string pName) {
	head->pKill(pName, 0);
}

void list::pFrag() {
	head->pFrag(0);
}

void list::display() {
	head->display(1);
}

/*START PAGE IMPLEMENTATION*/
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

//FIX ME: STILL USING WORST ALG
void Page::bestAlg(string pName, int pSize) {
	Page * temp = this;
	int counter = 0;
	Page * newTemp = this;
	string tempHole[32];    //temp holds linked list so we can find index of length of holes
	int tempLengthHole[32];
	int startPositionHolder[32];
	int startIndex = 0;   //when a hole starts
	int endIndex = 0;    //when a hole ends
	int freeCounter = 0;
	int fragmentCounter = 0;
	int min = 0;   //keeps track of length of holes...looking for smallest hole

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
	//if fragment is > 1, then find each hole, make sure hole >= pSize, from that list find smallest hole
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

		//find smallest length
		for (int i = 0; i < fragmentCounter; ++i) {
			if (tempLengthHole[i] >= pSize) {
				min = tempLengthHole[i];   //start of possible mins
				for (int j = 0; j < fragmentCounter; ++j) {
					if (tempLengthHole[j] < tempLengthHole[i]) {
						min = tempLengthHole[j];
						startIndex = startPositionHolder[j];   //position of min hole 
					}
				}
			}
		}

		//get to position in linked list of hole
		//case 1: starts at beginning of list
		if (startIndex == 0) {
			newTemp->pName = pName;
			for (int i = 0; i < pSize; ++i) {
				newTemp->pName = pName;
				newTemp = newTemp->next;
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
				newTemp = newTemp->next;
			}
		}
	}
	cout << '\n' << "Program " << pName << " added successfully: " << pSize << " page(s) used." << '\n' << endl;
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
				newTemp = newTemp->next;
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
				newTemp = newTemp->next;
			}
		}
	}
	cout << '\n' << "Program " << pName << " added successfully: " << pSize << " page(s) used." << '\n' << endl;
}

void Page::pKill(string pName, int i) {
	Page * temp = this;
	
	if (this->pName == pName) {
		this->pName = "Free";
		pSize = 0;
		++i;
	}
	if (i == 0) {
		cout << '\n' << "Error, " << pName << " not running, " << i << " pages reclaimed." << '\n' << endl;
		return;
	}
	if (next == NULL) {
		cout << '\n' << "Program " << pName << " successfully killed, " << i << " pages reclaimed." << '\n' << endl;
		return;
	}
	else {
		next->pKill(pName, i);
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

bool Page::checkProgramRunning(string pName) {
	Page * temp = this;

	while (temp != NULL) {
		//case 1: checking whether program is running or not
		if (temp->pName == pName) {
			return true;
		}
		return false;
	}
}

bool Page::checkHolesWorst(string pName, int pSize) {
	Page * temp = this;
	int counter = 0;
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

	//if fragment is only one, then just check to make sure this is enough "free" to handle process
	if (fragmentCounter == 1) {
		for(int i=0;i<32;++i) {
			if (tempHole[i] != "Free") {
				++i;
			}
			else { ++counter; }
		}
		if (counter >= pSize) {
			return true;
		}
		else return false;
	}

	//if fragment is > 1, then find each hole, find largest hole, make sure largest hole can hold process
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
		if (max >= pSize) {
			return true;
		}
		else return false;
	}

}

bool Page::checkHolesBest(string pName, int pSize){
	Page * temp = this;
	int counter = 0;
	Page * newTemp = this;
	string tempHole[32];    //temp holds linked list so we can find index of length of holes
	int tempLengthHole[32];
	int startPositionHolder[32];
	int startIndex = 0;   //when a hole starts
	int endIndex = 0;    //when a hole ends
	int freeCounter = 0;
	int fragmentCounter = 0;
	int min = 32;   //keeps track of length of holes...looking for smallest hole

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

	//if fragment is only one, then just check to make sure this is enough "free" to handle process
	if (fragmentCounter == 1) {
		for (int i = 0; i<32; ++i) {
			if (tempHole[i] != "Free") {
				++i;
			}
			else { ++counter; }
		}
		if (counter >= pSize) {
			return true;
		}
		else return false;
	}

	//if fragment is > 1, then find each hole, find smalleset hole, find smallest hole >= pSize
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
		//find smallest length
		for (int i = 0; i < fragmentCounter; ++i) {
			if (tempLengthHole[i] < min && tempLengthHole[i]>= pSize) {
				min = tempLengthHole[i];   //min hole
				startIndex = startPositionHolder[i];   //position of min hole 
			}
		}
		if (min >= pSize) {
			return true;
		}
		else return false;
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
	string alg = "best";

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

