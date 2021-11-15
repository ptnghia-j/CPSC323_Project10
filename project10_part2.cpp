#include <iostream>
#include <string>
#include <math.h>

//-------------------------------------------------------------------------------------------------------------
//   Group names: Nghia Phan
// 	 Assignment : Project 10 - part 2
//   Due date   : November 18, 2021
//   Purpose: write a program to trace input strings
//   (1) a=(a+a)*a$ 
//   (2) a=a*(a-a)$
//   (3) a=(a+a)a$
//   Given the grammar: S->aW ; W-> =E; E->E+T; E->E-T; E->T; T->T*F; T->T/F; T->F; F->a; F->(E); 
//--------------------------------------------------------------------------------------------------------------

//create a stack 
class Stack {
	//two private fields: stack and counter
	private: 
    	//create a an array of 10 character as the stack
        char stack[10];
    	int counter = 0;
    
    //operations on the stack
    public:
        //create constructor
        Stack() {
            for (int i = 0; i < 10; i++) {
                stack[i] = 0;
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
		void push(int element) {
			if (!isFull()) {
				stack[counter] = element;
				counter++;
			}
			else {
				std::cout << "The stack is full, cannot push anymore" << std::endl;
			}
		}
		//pop element out of the stack if it is not empty
		int pop() {
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
            std::cout << std::endl;}        
};

/*  Create Predictive Parsing Table
    with terminals = {S, W, E, Q, T, R, F}
    and non-terminals = {a, +, -, *, /, (, ), $, =}
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |states|| a    | +     | -     | *     | /     | (     | )     | $     | =     |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |S     ||aW    |       |       |       |       |       |       |       |       |   
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |W     ||      |       |       |       |       |       |       |       | =E    |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |E     ||TQ    |       |       |       |       |TQ     |       |       |       |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |Q     ||      |+TQ    | -TQ   |       |       |       |lambda |lambda |       |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |T     ||FR    |       |       |       |       |FR     |       |       |       |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |R     ||      |lambda |lambda |*FR    |/FR    |       |lambda |lambda |       |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
    |F     ||a     |       |       |       |       |(E)    |       |       |       |
    |------||------|-------|-------|-------|-------|-------|-------|-------|-------|
*/
//create the table of size 5x8
std::string table[7][9] = {
    {"aW", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "=E"},
    {"TQ", "", "", "", "", "TQ", "", "", ""},
    {"", "+TQ", "-TQ", "", "", "", "lambda", "lambda", ""},
    {"FR", "", "", "", "", "FR", "", "", ""},
    {"", "lambda", "lambda", "*FR", "/FR", "", "lambda", "lambda", ""},
    {"a", "", "", "", "", "(E)", "", "", ""}
};

//function to retrieve the string for each goto state
std::string retrieveState(char nonTerminal, char terminal) {
    int terminalIndex = 0;
    int nonTerminalIndex = 0;

    //find the index of terminal
    switch(nonTerminal) {
        case 'S': nonTerminalIndex = 0; break;
        case 'W': nonTerminalIndex = 1; break;
        case 'E': nonTerminalIndex = 2; break;
        case 'Q': nonTerminalIndex = 3; break;
        case 'T': nonTerminalIndex = 4; break;
        case 'R': nonTerminalIndex = 5; break;
        case 'F': nonTerminalIndex = 6; break;
        default:
            std::cout << "Error: nonTerminal not found" << std::endl;
            break;
    }
    
    //find the index of non-terminal
    switch(terminal) {
        case 'a': terminalIndex = 0; break;
        case '+': terminalIndex = 1; break;
        case '-': terminalIndex = 2; break;
        case '*': terminalIndex = 3; break;
        case '/': terminalIndex = 4; break;
        case '(': terminalIndex = 5; break;
        case ')': terminalIndex = 6; break;
        case '$': terminalIndex = 7; break;
        case '=': terminalIndex = 8; break;
        default:
            std::cout << "Error: terminal not found" << std::endl;
            break;
    }
    //nonTerminalIndex is the row, and terminalIndex is the column
    return table[nonTerminalIndex][terminalIndex];
}

int main() {
    std::string wordsList[3] = {"a=(a+a)*a$", "a=a*(a-a)$", "a=(a+a)a$"};

	Stack *stack = (Stack*) malloc(sizeof(Stack));
    
    int wordListSize = sizeof(wordsList)/sizeof(wordsList[0]);
    for (int i = 0; i < wordListSize; i++) {
        //clear up the stack 
        stack->clearStack();
        std::string word = wordsList[i];

        //initial set up for the stack
        //push "$" to the stack
        stack->push('$');
        //push "E" to the stack
        stack->push('S');
        /*
        set variable wordIndex to 0
        While the stack is not empty
            1. Pop the top element from the stack and store it in a variable called topElement
               Print out the current character at word[wordIndex]
            2. Compare the topElement with the character at word[wordIndex]
                i. If they are the same, increase wordIndex by 1 then
                       print out the content of the stack and continue
                ii. If they are not the same, retrieve the string for the goto state
                    a. If the string is not "blank"
                        Check if the string is "lambda"
                            - If it is, print out the content of the stack and continue
                        If it is not, then push the string to the stack
                            Push the character to the stack in reverse order
                            Print out the content of the stack
                    b. if the string is "blank"
                        break out of the loop
        
        if wordIndex = word.length(), print "Accepted"; otherwise, print "Rejected"
        */
        int wordIndex = 0;
        while (!stack->isEmpty()) {
            char topElement = stack->pop();
            std::cout << std::endl;
            std::cout << "Current read character: " << word[wordIndex] << std::endl;
            if (topElement == word[wordIndex]) {
                std::cout << "Match input " << word[wordIndex] << std::endl;
                wordIndex++;
                stack->printStack();
            } else {
                std::string gotoState = retrieveState(topElement, word[wordIndex]);
                if (gotoState != "") {
                    if(gotoState == "lambda") {
                        std::cout << "Stack: ";
                        stack->printStack();
                        continue;
                  }
                    for (int i = gotoState.length() - 1; i >= 0; i--) {
                        stack->push(gotoState[i]);
                    }
                    stack->printStack();
                } else {
                    break;
                }
            }     
        }

        wordIndex == word.length() ? std::cout << ">>>>>>>> Word accepted <<<<<<<<<" << std::endl 
                                   : std::cout << ">>>>>>>> Word Rejected <<<<<<<<<" << std::endl;
        
    }

	return 0;
}