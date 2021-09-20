#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable {
public:
	class Node;

	Node* head;
	Node* tail;
	int count;
public:
    SymbolTable() : head(NULL), tail(NULL), count(0) {};

	void insertCommand(string name, string type){
		if(this->count == 0){
			this->head = new Node(name, type);
			count++;
			return;
		}

		Node* temp = this->head;
		Node* p;
		while(temp != 0){
			if(temp->name == name)
				throw Redeclared("INSERT " + name +  " " + type);
			p = temp;
			temp = temp->next;
		}
		p->next = new Node(name, type);
		count++;
	}

	void assignCommand(string name, string var){
		string errorCommand = "ASSIGN " + name + " " + var;
		Node* temp = this->head;
		while(temp != 0){
			if(temp->name == name) break;
			temp = temp->next;
		}
		if(temp == 0) throw Undeclared(errorCommand); // chưa khai báo

		if(var[0] == 39 && var.back() == 39) { // maybe string
			if(temp->type != "string") throw TypeMismatch(errorCommand);
			for(int i = 1; i < var.size() - 1; i++) {
				if(var[i] >= 33 && var[i] <= 47)  throw TypeMismatch(errorCommand);
				if(var[i] >= 58 && var[i] <= 64)  throw TypeMismatch(errorCommand);
				if(var[i] >= 91 && var[i] <= 96)  throw TypeMismatch(errorCommand);
				if(var[i] >= 123)  throw TypeMismatch(errorCommand);
				temp->varString = var;
			}
		}

		else if (var[0] >= 48 && var[0] <= 57){ // maybe num
			if(temp->type != "number") throw TypeMismatch(errorCommand);
			for(int i = 0; i < var.size(); i++) {
				if(var[i] < 49 || var[i] > 57) throw TypeMismatch(errorCommand);
				temp->varNum = stoi(var);
			}
		}

		else if (var[0] >= 97 && var[0] <= 122){ // orther identifier
			Node* p = this->head;
			while(p != 0){
				if(p->name == var){
					if(temp->type != p->type) throw TypeMismatch(errorCommand);
					temp->varNum = p->varNum;
					temp->varString = p->varString;
				}
				p = p->next;
			}
			if(p == 0) throw Undeclared(errorCommand); // chưa khai báo
		}
	}

	void block(int& blockCheck, string str){
		if (str == "BEGIN") blockCheck++;
		else blockCheck--;
		if (blockCheck < 0) throw UnknownBlock();


	}

    void run(string filename){
		fstream input;
		input.open(filename, ios::in);
		string str;
		int blockCheck = 0;

		string commandList[] = {"INSERT", "ASSIGN", "BEGIN", "END", "LOOKUP", "PRINT", "RPRINT"};

		while (!input.eof()){
			string str;
			getline(input, str);
			
			string command;
			string identifierName;
			string type;
			// tìm vị trí dấu _ , tách lấy command, xóa đi.
			int pos = str.find(" ");
			command = str.substr(0, pos);
			str.erase(0, pos + 1);
			//tách lấy identifierName.
			pos = str.find(" ");
			identifierName = str.substr(0, pos);
			str.erase(0, pos + 1);
			//lấy type.
			type = str;

			if (command == commandList[0]) insertCommand(identifierName, type);
			else if(command == commandList[1]) assignCommand(identifierName, type);
			else if(command == commandList[2]) block(blockCheck, "BEGIN");
			else if(command == commandList[3]) block(blockCheck, "END");
			// else if(command == commandList[4])
			// else if(command == commandList[5])
			// else if(command == commandList[6])
			// else 

			if (command != commandList[2] && command != commandList[3])
				cout << "success" << endl;
		}
		if (blockCheck > 0) throw UnclosedBlock(blockCheck);
	}
	class Node{

	public: 
		string name;
		string type;
		int varNum;
		string varString;
		Node* next;

		Node(){};
		Node(string name, string type){
			this->name = name;
			this->type = type;
			this->next = 0;
		};
	};
};

#endif