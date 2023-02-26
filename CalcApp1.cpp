//I've now come to realize how great pointers are for data management, however, I am too lazy. I'll practice it in the future though.

#include <iostream>
#include <stack>
#include <string>
#include <ctype.h>
#include <math.h>

using namespace std;

//formats the string without spaces and adds characters to allow for proper calculation, does not put in postfix notation
string Format(string input) {

    struct {
        string woSpaces = "";
        string output = "";
    }formatVars;//used for the format function, contains two strings. one for woSpaces and one for the final output

    //removes the spaces from a string
    for (int i = 0; i < input.size(); i++) {
        if (input[i] != ' ')
            formatVars.woSpaces += input[i];
    }

    //adds '*' before and after '(' and ')' respectively, if there is a digit there. also adds a 0 infront of - and +, if they are at the start of the string
    for (int i = 0; i < formatVars.woSpaces.size(); i++) {

        if (i == 0 && (formatVars.woSpaces[i] == '-' || formatVars.woSpaces[i] == '+')) {
            formatVars.output.push_back('0');
            formatVars.output.push_back(formatVars.woSpaces[i]);
        }

        else if (i != 0 && formatVars.woSpaces[i] == '(') {
            if (isdigit(formatVars.woSpaces[i - 1]) || formatVars.woSpaces[i - 1] == ')')
                formatVars.output.push_back('*');
            formatVars.output.push_back(formatVars.woSpaces[i]);
        }

        else if (i != 0 && formatVars.woSpaces[i - 1] == ')') {
            if (isdigit(formatVars.woSpaces[i]) || formatVars.woSpaces[i] == '(')
                formatVars.output.push_back('*');
            formatVars.output.push_back(formatVars.woSpaces[i]);
        }

        else formatVars.output.push_back(formatVars.woSpaces[i]);
    }

    return formatVars.output;
}

//Determines if character is '+', '-', '*'... or any symbol you want to check for.
bool IsExpression(char input) {
    if (input == '*' || input == '/' || input == '^' || input == '+' || input == '-' || input == '%')
        return true;
    else return false;
}


//Checks for errors in the string beforehand, to make sure the math expressions do not crash. True means it has an error, false means it doesn't
bool ErrorCheck(string input) {

    input = Format(input);
    int counter = 0;

    // checks first char if its a symbol or a ')'
    if (IsExpression(input[0]) || input[0] == ')') {
        cout << "first char check" << endl;
        // *cout << input[0] << endl;
        // *cout << "" + isdigit(input[0]) << endl;
        return true;
    }

    //checks last string char if its a symbol or '('
    if (IsExpression(input[input.size() - 1]) || input[input.size()] == '(') { 
        cout << "last char check" << endl;
        return true;
    }
    
    //checks if there are any extra parantheses or if using wrong parantheses
    for (int i = 0; i < input.size(); i++) {
        
        // these two statements were just used to see what was happening under the hood bc i cant be bothered to learn the debugger. maybe one day...
        // cout << input[i] << endl;
        // cout << counter << endl;
        
        if (input[i] == '(')
            counter++;
        else if (input[i] == ')') {
            counter--;
            if (counter < 0) {
                cout << "wrong parantheses order" << endl;
                return true;
            }
        }
    }
    if (counter > 0 || counter < 0) {
        cout << "extra parantheses" << endl;
        return true;
    }

    //checks for double expressions
    for (int i = 1; i < input.size(); i++) {
        if (IsExpression(input[i - 1]) && IsExpression(input[i])) {
            cout << "double expressions" << endl;
            return true;
        }
    }

    //checks if the string even has numbers
    counter = 0; //resets counter to be used again.
    for (int i = 0; i < input.size(); i++) {
        if(isdigit(input[i]))
            counter++;
    }
    if (counter == 0) {
        cout << "has nums" << endl;
        return true;
    }

    //checks for a parantheses without numbers in it
    counter = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '(') {

        }
    }

    return false;


}

//creates a stack from a string input. allows for proper
stack<string> String2Stack(string input) {
    stack<string> initializedStack; //used to contain each num and symbol just have their own string container they're in
    stack<string> outputStack;
    string tmp = "";

    for (int i = 0; i < input.size(); i++) {
        if (isdigit(input[i]) || input[i] == '.')
            tmp += input[i];
        else {
            if (!tmp.empty())
                initializedStack.push(tmp);
            tmp = input[i];
            initializedStack.push(tmp);
            tmp = ""; //resets the tmp string holder for the next set of characters
        }
    }

    if (!tmp.empty())
        initializedStack.push(tmp);

    
    while (!initializedStack.empty()) {
        outputStack.push(initializedStack.top());
        initializedStack.pop();
    }
    
    return outputStack;
}

//Checks if PEMDAS is happening correctly.
bool Precedence(string input1, string input2) {
    int counter = 0;

    //checks 1st input
    if (input1[0] == '+' || input1[0] == '-')
        counter += 1;
    else if (input1[0] == '*' || input1[0] == '/' || input1[0] == '%')
        counter += 2;
    else
        counter += 3;

    //checks 2nd input
    if (input2[0] == '+' || input2[0] == '-')
        counter -= 1;
    else if (input2[0] == '*' || input2[0] == '/' || input2[0] == '%')
        counter -= 2;
    else
        counter -= 3;

    if (counter > 0)
        return false;

    return true;


}

//takes a stack of infix strings to create a stack of postfix strings
stack <string> IntoPost(stack <string> input) {
    stack <string> postFix; //holds the postfix notation of the string, however, then needs to be put in another stack.
    stack <string> tmpHld; //holds thing for the time being

    while (!input.empty()) {

        if (isdigit(input.top()[0])) {
            postFix.push(input.top());
            input.pop();
        }
        else {
            if (input.top()[0] == '(') {
                tmpHld.push(input.top());
                input.pop();
            }

            else if (IsExpression(input.top()[0]) || input.top()[0] == '(') {
                while (!tmpHld.empty() && tmpHld.top()[0] != '(' && Precedence(input.top(), tmpHld.top())) {
                    postFix.push(tmpHld.top());
                    tmpHld.pop();
                }
                tmpHld.push(input.top());
                input.pop();
            }
            else if (input.top()[0] == ')') {
                input.pop();
                while (!tmpHld.empty() && tmpHld.top()[0] != '(') {
                    postFix.push(tmpHld.top());
                    tmpHld.pop();
                }
                tmpHld.pop();
            }
            else
                cout << "Error Occurred at >> " + input.top();
        }
    }

    while (!tmpHld.empty()) {
        if (tmpHld.top()[0] == '(' || tmpHld.top()[0] == ')')
            tmpHld.pop();
        else {
            postFix.push(tmpHld.top());
            tmpHld.pop();
        }
    }

    while (!postFix.empty()) {
        tmpHld.push(postFix.top());
        postFix.pop();
    }

    return tmpHld;
}

//returns null, means it had an error. probably a non identifiable character in input string
long double PostMath(stack <string> input) {
    stack<long double> output;
    long double tmpHld1 = 0;
    long double tmpHld2 = 0;

    while (!input.empty()) {
        if (isdigit(input.top()[0])) {
            output.push(stod(input.top()));
            input.pop();
        }
        else if (input.top()[0] == '^') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(pow(tmpHld1, tmpHld2));
        }
        else if (input.top()[0] == '*') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(tmpHld1 * tmpHld2);
        }
        else if (input.top()[0] == '/') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(tmpHld1 / tmpHld2);
        }
        else if (input.top()[0] == '+') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(tmpHld1 + tmpHld2);
        }
        else if (input.top()[0] == '-') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(tmpHld1 - tmpHld2);
        }
        else if (input.top()[0] == '%') {
            input.pop();
            tmpHld2 = output.top();
            output.pop();
            tmpHld1 = output.top();
            output.pop();
            output.push(int(tmpHld1) % int(tmpHld2));
        }
        
        else {
            cout << "Error in postMath" << endl;
            break;
        }
    }

    return output.top();
}

//calculates the inputted string, formats it to postfix, does math and returns a double.
long double Calculate(string input) {
    input = Format(input);
    return PostMath(IntoPost(String2Stack(input)));
}

//creates a string of the postfix notation of the entered string
string IntoPostResults(string input) {
    string output = "";
    stack<string> tmp = IntoPost(String2Stack(Format(input)));

    while (!tmp.empty()) {
        output += tmp.top() + " ";
        tmp.pop();
    }
    
    return output;
}

int main()
{

    string mathInput = "25^2"; //receives user input
    string diagnostics = ""; //creates a string with the postfix, formater and calculation to check individual functions

    //main part of the calculator app.
    cout << "\t\t~~CALCULATOR~~\n\t\tType \'stop\' to end program" << endl;
    while (mathInput.compare("STOP") != 0) {

        cout << "Input>>\t";
        getline(cin, mathInput);

        if (mathInput.empty())
            cout << "Output>>" << endl;
        else if (mathInput.compare("STOP") == 0)
            break;
        else if (ErrorCheck(mathInput))
            cout << "Output>> Error, issues with math syntax!" << endl;
        else {
            cout << "Output>>\t" + to_string(Calculate(mathInput)) << endl;
            diagnostics += "Formatted String>> " + Format(mathInput) + "\tInto PostFix Notation >> " + IntoPostResults(mathInput) + "\tCalculation Results >> " + to_string(Calculate(mathInput)) +"\n";
        }

    }

    cout << "GOODBYE!\n Print Diagnostic Screen?\nEnter 'Y' for yes, 'N' for no\nInput>>";
    getline(cin, mathInput);
    if(mathInput[0] == 'Y')
        cout << "Printing...\n" + diagnostics << endl; //prints out the diagnostic string


    return 0;
}
