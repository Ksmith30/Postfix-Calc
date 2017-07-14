// PF2 Project #4
// ID: 010711584
/// Name: Kyle Smith

#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;
const string addition = "+";
const string subtraction = "-";
const string multiplication = "*";
const string division = "/";
const string leftParentheses = "(";
const string rightParentheses = ")";

bool checkOperator(const string& temp)
{
    return (temp == addition || temp == subtraction || temp == multiplication 
        || temp == division);
}

int getPrecedence(const string &temp)
{
    if (temp == addition || temp == subtraction)
        return 1;
    else if (temp == multiplication || temp == division)
        return 2;
    else
        return 0;
}
    
bool shuntingYard(const vector<string>& expression, vector<string>& postfix)
{
    stack<string> track;
    for (const string& i : expression)
    {
        // Check if operator
        if (checkOperator(i) && !track.empty())
        {
            if (getPrecedence(track.top()) >= getPrecedence(i))
            {
                postfix.push_back(track.top());
                track.pop();
            }
            track.push(i);
        }
        else if (checkOperator(i) && track.empty())
            track.push(i);
        else if (i == leftParentheses)
            track.push(i);
        else if (i == rightParentheses)
        {
            while (track.top() != leftParentheses)
            {
                postfix.push_back(track.top());
                track.pop();
                
                // Check for missing right parentheses
                if (track.empty())
                    return false;
            }
            track.pop();
        }
        else 
            postfix.push_back(i);            
    }
    
    while (!track.empty())
    {
        if (track.top() == leftParentheses)
            return false;
        postfix.push_back(track.top());
        track.pop();
    }
    return true;
}

void performOperation(stack<string>& numbers, const string& temp)
{
    double answer = 0;
    
    // Pop numbers off the stack if operand
    string sNumber2 = numbers.top();
    double number2 = stod(sNumber2);
    numbers.pop();
    string sNumber1 = numbers.top();
    double number1 = stod(sNumber1);
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


bool evaluatePostfix(const vector<string>& postfix, double& result)
{
    stack<string> numbers;
    
    // Iterate through postfix
    for (const string& i : postfix)
    {
        // Check if number or operator
        if (!checkOperator(i))
            numbers.push(i);
        else if (numbers.empty())
            return false;
        else  performOperation(numbers, i);
    }
    
    // Check for incorrect evaluation
    if (numbers.size() > 1 || numbers.empty())
        return false;
    
    string sResult = numbers.top();
    result = stod(sResult);
    return true;
}

int main()
{
    // Take Expression to be evaluated
//    vector<string> expression;
    
//    string token;
//    cout << "Enter the expression to be evaluated (Enter + CTRL-D to end): " << endl;
//    while (cin >> token)
//        expression.push_back(token);
//    
//    // Print out infix expression
//    for (const auto& i : expression)
//        cout << i << ' ';
//    cout << endl;
    
    // Convert to postfix 
    vector<string> postfix;
    vector<string> expression = {"3", "*", "(", "4", "/", "5", ")", "+", "2"};
    if (!shuntingYard(expression, postfix))
    {
        cout << "The expression is invalid.";
        return -1;
    }
    
    // Simplify
    double answer = 0;
    
    if (!evaluatePostfix(postfix, answer))
    {
        cout << "The expression is invalid.";
        return -1;
    }
    
    // Return answer
    cout << "Answer: " << answer << endl;
    
    return 0;
}

