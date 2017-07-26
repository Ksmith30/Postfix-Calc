// PF2 Project #5
// ID: 010711584
/// Name: Kyle Smith

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
using namespace std;
const string addition = "+";
const string subtraction = "-";
const string multiplication = "*";
const string division = "/";
const string leftParentheses = "(";
const string rightParentheses = ")";

// Returns if given string is an operator
bool checkOperator(const string& temp)
{
    return (temp == addition || temp == subtraction || temp == multiplication 
        || temp == division);
}

// Returns precedence of operators
int getPrecedence(const string &temp)
{
    if (temp == addition || temp == subtraction) return 1;
    else if (temp == multiplication || temp == division) return 2;
    else return 0;
}

// Returns if left Parentheses is found
bool findLeftParentheses(stack<string>& track, vector<string>& postfix)
{
    while (!track.empty())
    {
        if (track.top() == leftParentheses)
        {
            track.pop();
            return true;
        }   
        postfix.push_back(track.top());
        track.pop();  
    }
    return false;
}

// Djikstra's shunting yard that takes an expression and converts it to a postfix
// expression
bool shuntingYard(const vector<string>& expression,
        vector<string>& postfix)
{
    stack<string> track;
    
    for (const string& i : expression)
    {
        // Check if operator
        if (!checkOperator(i) && i != leftParentheses && i != rightParentheses)
            postfix.push_back(i);
        else if (checkOperator(i))
        {
            while (!track.empty() && (getPrecedence(track.top()) >= getPrecedence(i)))
            {
                postfix.push_back(track.top());
                track.pop();
            }
            track.push(i);
        }
        else if (i == leftParentheses)
            track.push(i);
        else if (i == rightParentheses)
        {
            if (!findLeftParentheses(track, postfix))
                return false;
        }
    }
    
    return !findLeftParentheses(track, postfix);
}

// Converts number from string to double
double convertNumber(stack<string>& temp)
{
    if (!temp.empty())
    {
        string sNumber = temp.top();
        double number = stod(sNumber);
        return number;
    }
    return double();
}

// Performs operation on two numbers
void performOperation(stack<string>& numbers, const string& temp)
{
    double answer = 0;
    
    // Pop numbers off the stack if operand
    double number2 = convertNumber(numbers);
    numbers.pop();
    double number1 = convertNumber(numbers);
    numbers.pop();

    // Perform necessary operation
    if (temp == addition)
        answer = number1 + number2;
    else if (temp == subtraction)
        answer = number1 - number2;
    else if (temp == multiplication)
        answer = number1 * number2;
    else if (temp == division)
        answer = number1 / number2;

    // Convert answer to string and push to stack
    string sAnswer = to_string(answer);
    numbers.push(sAnswer);
}

// Evaluates postfix expression and returns a result
bool evaluatePostfix(const vector<string>& postfix,
        double& result)
{
    stack<string> numbers;
    
    // Iterate through postfix
    for (const string& i : postfix)
    {
        // Push number to stack
        if (!checkOperator(i)) numbers.push(i);
          
        else
        {
            if (numbers.size() > 1)
                performOperation(numbers, i);
            else return false;
        }
    }
    
    // Check for incorrect evaluation
    if (numbers.size() > 1 || numbers.empty())
        return false;
    
    result = convertNumber(numbers);
    return true;
}

int main()
{
    // Take Expression to be evaluated
    
    cout << "Enter Q to exit." << endl;
    string str;
  
    while (str != "Q")
    {
        vector<string> expression;
        cout << ">> ";
        getline(cin, str);
        if (str == "Q")
            break;
        stringstream ss(str);
        while (ss >> str)
            expression.push_back(str);
        
        // Convert to postfix 
        vector<string> postfix;
        if (!shuntingYard(expression, postfix))
            cout << "Mismatched Parentheses." << endl;
        
        else 
        {
            // Simplify
            double answer = 0;

            if (!evaluatePostfix(postfix, answer))
                cout << "Expression was malformed." << endl;

            // Return answer
            else cout << answer << endl;
        }
    }
    
    return 0;
}


