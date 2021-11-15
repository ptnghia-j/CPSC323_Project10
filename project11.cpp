#include <iostream>
#include <string>
#include <math.h>

//-------------------------------------------------------------------------------------------------------------
//   Group names: Nghia Phan
//   Assignment : Project 10
//   Due date   : November 18, 2021
//   Purpose: write a program to trace input strings using LR Parsing Method
//   (1) (i+i)*i$ 
//   (2) (i*)$
//--------------------------------------------------------------------------------------------------------------

//create a stack 
class Stack {
	//two private fields: stack and counter
	private: 
    	//create a an array of 10 character as the stack
        std::string stack[10];
    	int counter = 0;
    
    //operations on the stack
    public:
        //create constructor
        Stack() {
            for (int i = 0; i < 10; i++) {
                stack[i] = ".";
            }
        }
    	//clear the stack by reseting the counter
    	void clearStack() {
    		counter = 0;
    	}
    	//check if the stack is empty
    	bool isEmpty() {
		return (counter == 0) ? true : false;
	}
	//check if the stack is full
	bool isFull() {
	    return (counter == 10) ? true: false;
	}
	//push new element to top of the stack if it is not full
	void push(std::string element) {
		if (!isFull()) {
			stack[counter] = element;
			counter++;
		}
		else {
			std::cout << "The stack is full, cannot push anymore" << std::endl;
		}
	}
	//pop element out of the stack if it is not empty
	std::string pop() {
		if (!isEmpty()) {
			counter--;
			return stack[counter];
		}
		else {
			std::cout << "The stack is empty, nothing to pop" << std::endl;
			return 0;
		}

	}
	//optional method to check elements in the stack
	//since the stack only use counter to locate values, some old values
	//may still in the stack but outside of the range  [0, counter]
    void printStack() {
      std::cout << "Stack: ";
      for (int i = 0; i < counter; i++) {
        std::cout << stack[i] << " ";
      }
      std::cout << std::endl;
    }        
};

/*  Create an LR Parsing Table

    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||states||                       Terminals                              ||     Non-terminals     ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||      ||  i   |   +   |   -   |   *   |   /   |   (   |   )   |   $   ||   E   |   T   |   F   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||0     ||  S5  |       |       |       |       |  S4   |       |       ||  1    |   2   |   3   ||         
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||1     ||      |  S6   |  S7   |       |       |       |       |  ACC  ||       |       |       ||        
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||2     ||      |  R3   |  R3   |  S8   |  S9   |       |  R3   |  R3   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||3     ||      |  R6   |  R6   |  R6   |  R6   |       |  R6   |  R6   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||4     ||  S5  |       |       |       |       |  S4   |       |       ||  10   |   2   |   3   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||5     ||      |  R8   |  R8   |  R8   |  R8   |       |  R8   |  R8   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||6     ||  S5  |       |       |       |       |  S4   |       |       ||       |  11   |   3   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||7     ||  S5  |       |       |       |       |  S4   |       |       ||       |  12   |   3   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||8     ||  S5  |       |       |       |       |  S4   |       |       ||       |       |  13   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||9     ||  S5  |       |       |       |       |  S4   |       |       ||       |       |  14   ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||10    ||      |  S6   |  S7   |       |       |       |  S15  |       ||       |       |       ||        
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||11    ||      |  R1   |  R1   |  S8   |  S9   |       |  R1   |  R1   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||12    ||      |  R2   |  R2   |  S8   |  S9   |       |  R2   |  R2   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||13    ||      |  R4   |  R4   |  R4   |  R4   |       |  R4   |  R4   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||14    ||      |  R5   |  R5   |  R5   |  R5   |       |  R5   |  R5   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
    ||15    ||      |  R7   |  R7   |  R7   |  R7   |       |  R7   |  R7   ||       |       |       ||
    ||------||------|-------|-------|-------|-------|-------|-------|-------||-------|-------|-------||
*/
//create the table of size 16x11 based on the LR Table
std::string table[16][11] = {
    {"S5","","","","","S4","","","1","2","3"},
    {"","S6","S7","","","","","ACC","","",""},
    {"","R3","R3","S8","S9","","R3","R3","","",""},
    {"","R6","R6","R6","R6","","R6","R6","","",""},
    {"S5","","","","","S4","","","10","2","3"},
    {"","R8","R8","R8","R8","","R8","R8","","",""},
    {"S5","","","","","S4","","","","11","3"},
    {"S5","","","","","S4","","","","12","3"},
    {"S5","","","","","S4","","","","","13"},
    {"S5","","","","","S4","","","","","14"},
    {"","S6","S7","","","","S15","","","",""},
    {"","R1","R1","S8","S9","","R1","R1","","",""},
    {"","R1","R2","S8","S9","","R2","R2","","",""},
    {"","R4","R4","R4","R4","","R4","R4","","",""},
    {"","R5","R5","R5","R5","","R5","R5","","",""},
    {"","R7","R7","R7","R7","","R7","R7","","",""}
};
 
/*
1. E->E+T; 2. E->E-T; 3.E->T; 4.T->T*F; 5.T->T/F; 6.T->F; 7.F->(E); 8.F->i; 
*/
//length of the right hand side of the rules
//rule numbers start from 1, so put dummy values at index 0
int lengthOfRHS[9] = {0, 3, 3, 1, 3, 3, 1, 3, 1};
std::string reducedState[9] = {"", "E", "E", "E", "T", "T", "T", "F", "F"};

std::string retrieveState(std::string currentState, std::string gotoFactor) {
    int row = 0;
    int col = 0;;

    //convert string currentState to int and store in row
    row = std::stoi(currentState);

    //find the column from the gotoFactor
    switch(gotoFactor[0]) {
        case 'i': col = 0; break;
        case '+': col = 1; break;
        case '-': col = 2; break;
        case '*': col = 3; break;
        case '/': col = 4; break;
        case '(': col = 5; break;
        case ')': col = 6; break;
        case '$': col = 7; break;
        case 'E': col = 8; break;
        case 'T': col = 9; break;
        case 'F': col = 10; break;
        default:
            std::cout << "Error: not valid gotoFactor" << std::endl;
            break;
    }
    //nonTerminalIndex is the row, and terminalIndex is the column
    return table[row][col];
}
 
std::string handleState(std::string word, int wordIndex, Stack *stack) {
    //get the next state from the table
    std::string topElement = stack->pop();
    std::string currChar = word.substr(wordIndex, 1);
    std::string gotoState = retrieveState(topElement, currChar);
    //if the state starts with S then it is a shift state
    if (gotoState[0] == 'S') {
        //Push the topElement to the stack, push currChar to the stack, push gotoState[1] to the stack
        stack->push(topElement);
        stack->push(currChar);
        stack->push(gotoState.substr(1));
        //print out the stack
        stack->printStack();
        
    } //else if state is a reduce state
    else if (gotoState[0] == 'R') {
        stack->push(topElement);
        //pop the stack twice as the number of characters in the ith right hand side rule 
        int ruleNumber = gotoState[1] - 48; // ascii value of the character 0 is 48
        int popSize = lengthOfRHS[ruleNumber];
        for (int k = 0; k < 2 * popSize; k++) {
            stack->pop();
        }
        //print out the stack
        stack->printStack();
        //pop topElement from the stack and store it in the topElement
        topElement = stack->pop();
        //get the gotoFactor from the reduced state corresponding to the number returned by gotoState[1]
        //which is guaranteed to be a non-terminal letter
        std::string gotoFactor = reducedState[gotoState[1] - 48];
        //retrieve the next state from the table
        gotoState = retrieveState(topElement, gotoFactor);
        //push the topElement to the stack, push gotoFactor, and nextState to the stack
        stack->push(topElement);
        stack->push(gotoFactor);
        stack->push(gotoState);
        //print out the stack
        stack->printStack();
    } //else if state is an accept state
    else if (gotoState == "ACC") { //do nothing,accept is handled in main function
    } //else if state is a reject state
    else if (gotoState == "") { //do nothing,accept is handled in main function
    }
    else {
        std::cout << "Error: not a valid gotoState" << std::endl;
    }
   return gotoState;
}

int main() {
  std::string wordsList[2] = {"(i+i)*i$", "(i*)$"};

	Stack *stack = (Stack*) malloc(sizeof(Stack));
    
    int wordListSize = sizeof(wordsList)/sizeof(wordsList[0]);
    for (int i = 0; i < wordListSize; i++) {
        //clear up the stack 
        stack->clearStack();
        std::string word = wordsList[i];

        //initial set up for the stack
        //push state 0 into the stack
        stack->push("0");
        bool done = false;
        int wordIndex = 0;
        bool isAccepted = false;
        while (!done) {
            std::string handledState = handleState(word, wordIndex, stack);
            if (handledState[0] == 'S') {
                wordIndex++;
            }
            else if (handledState[0] == 'R') {
              continue;
            }
            else if (handledState == "ACC") {
                isAccepted = true;
                done = true;
            }
            else if (handledState == "") {
                done = true;
            }
        }

        isAccepted ? std::cout << ">>>>> Accepted <<<<<" << std::endl 
                   : std::cout << ">>>>> Rejected <<<<<" << std::endl;
 
    }
    
	return 0;
}