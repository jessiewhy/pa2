#ifndef pa2_hpp
#define pa2_hpp

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class Page
{
private:
	string pName;
	int pSize;
	Page * next;
public:
	Page(string pName, int pSize);
	void createPage(string value);
	void display(int pIndex);
	void worstAlg(string pName, int pSize);
	void bestAlg(string pName, int pSize);
	void pKill(string pName, int i);
	void pFrag(int i);
	bool checkProgramRunning(string pName);
	bool checkHolesWorst(string pName, int pSize);
	bool checkHolesBest(string pName, int pSize);
};

class list
{
private:
	Page * head, *tail;
public:
	list();
	void insert(string value);
	void display();
	void pFrag();
	void add(string pName, int pSize, string pAlg);
	void pKill(string pName);
	
};

#endif /* pa2_hpp */#pragma once