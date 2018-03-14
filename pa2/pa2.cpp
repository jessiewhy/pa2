#include <iostream>
#include <string>
using namespace std;

struct node
{
	int data;
	node *next;
};

class Page
{
	string programName;
};

class list
{
private:
	node * head, * tail;
public:
	list()
	{
		head = NULL;
		tail = NULL;
	}
	void createnode(int value);   //same logic used for insert at end of tail scenario 
	void display();
	void insert_start(int value);   //insert at start of head scenario
	void insert_position(int pos, int value);   //insert at specific position scenario
	void delete_first();   //delete head first scenario
	void delete_last();   //delete tail scenario
	void delete_position(int pos);   //delete frm position scenario
};

void list::delete_position(int pos)
{
	node *current = new node;
	node *previous = new node;
	current = head;
	for (int i = 1; i < pos; i++)
	{
		previous = current;
		current = current->next;
	}
	previous->next = current->next;
}

void list::delete_last() {
	node *current = new node;
	node *previous = new node;
	current = head;
	//traverse until you reach tail of linkedlist 
	while (current->next != NULL)
	{
		previous = current;
		//current at tail of linkedlist
		current = current->next;
	}
	//set new tail of linkedlist
	tail = previous;
	previous->next = NULL;
	//delete current tail of linkedlist
	delete current;
}

void list::delete_first()
{
	node *temp = new node;
	//temp points to current head
	temp = head;
	//curr head points to second element
	head = head->next;
	//delete temp/"current head"
	delete temp;
}

void list::insert_position(int pos, int value) {
	node *pre = new node;
	node *cur = new node;
	node *temp = new node;
	cur = head;
	//iterate to get to desired position
	for (int i = 1; i < pos; i++) {
		pre = cur;
		cur = cur->next;
	}

	temp->data = value;
	//pre next field has address of new node 
	pre->next = temp;
	//new next field has address of current node 
	temp->next = cur;
}

//assigning new data value to current head position 
void list::insert_start(int value)
{
	node *temp = new node;
	temp->data = value;
	temp->next = head;
	head = temp;
}

//temp node, passes address of head node to it
void list::display()
{
	node *temp = new node;
	temp = head;
	//iterate and print each node until we reach tail
	int fCounter = 0;
	while (temp != NULL)
	{
		std::cout << temp->data << " ";
		temp = temp->next;
		++fCounter;
		if (fCounter%8 == 0) {
			std::cout<<endl;
		}
	}
}

void list::createnode(int value)
{
	node *temp = new node;
	temp->data = value;
	temp->next = NULL;
	//one node, then it is both head and tail of linkedlist
	if (head == NULL) {
		head = temp;
		tail = temp;
		temp = NULL;
	}
	//adding new node to end of tail of current linkedlist
	else
	{
		tail->next = temp;
		tail = temp;
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

int main()
{
	list obj;
	for (int i = 0; i < 32; ++i) {
		obj.createnode(i);
	}
	int myInput;
	int * myChoice = &myInput;
	do {
		myMenu(myChoice);
		switch (*myChoice) {
		case 1: {
			string pName;
			int pSize;
			cout << "Program name: ";
			cin >> pName;
			cout << "Program size (KB) - ";
			cin >> pSize;
			int pageSize;
			if (pSize % 4 != 0) {
				pageSize = (pSize / 4) + 1;
				cout << "Page size: " << pageSize << endl;
			}
			else {
				pageSize = pSize / 4;
				cout << "Page size: " << pageSize << endl;
			}
			for (int i = 0; i < pageSize; ++i) {
				obj.insert_start(0);
				obj.delete_last();
			}
			obj.display();
			cout << endl;
			*myChoice = 0;
			break;
		}
		case 2: {
			//kill program
			return 0;
		}
		case 3: {
			//fragmentation
			return 0;
		}
		case 4: {
			//print memory
			return 0;
		}
		case 5: {
			//exit 
			return 0;
		}
		}
	} while ((*myChoice!=0));
	system("pause");
	return 0;
}

