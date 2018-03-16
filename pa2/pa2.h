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
	void worstAlg(string pName, int pSize, int i);
	void bestAlg(string pName, int pSize, int i);
	void pkill(string pName, int i);
	void pFrag(int i);
	int checkAvailMem(string pName);
	bool checkConsecSpaces(int pSize, string pName);
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