#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
using namespace std;
const string addition = "+";
const string subtraction = "-";
const string multiplication = "*";
const string division = "/";
const string leftParentheses = "(";
const string rightParentheses = ")";

// Returns true if given a function name
bool checkFunction(const string& temp)
{
    return (temp == "max" || temp == "min" || temp == "sin" || temp == "cos" ||
            temp == "tan");
}

// Returns if given string is an operator
bool isOperator(const string& temp)
{
    return (temp == addition || temp == subtraction || temp == multiplication 
        || temp == division);
}

// Returns precedence of operators
int getPrecedence(const string &temp)
{
    if (temp == addition || temp == subtraction) return 1;
    else if (temp == multiplication || temp == division) return 2;
    else if (checkFunction(temp)) return 3;
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
        if (!isOperator(i) && i != leftParentheses && i != rightParentheses && 
                !checkFunction(i))
            postfix.push_back(i);
        else if (isOperator(i))
        {
            while (!track.empty() && (getPrecedence(track.top()) >= getPrecedence(i)))
            {
                postfix.push_back(track.top());
                track.pop();
            }
            track.push(i);
        }
        else if (i == leftParentheses || checkFunction(i))
            track.push(i);
        else if (i == rightParentheses)
        {
            if (!findLeftParentheses(track, postfix))
                return false;
        }
    }
    
    return !findLeftParentheses(track, postfix);
}

// Determine whether the input is 
bool checkUnary(const string& temp)
{
    return (temp == "sin" || temp == "cos" || temp == "tan");
}

// Performs operation on two numbers
double applyOperator(const string& op, double left, double right)
{
    // Perform necessary operation
    if      (op == addition)       return left + right;
    else if (op == subtraction)    return left - right;
    else if (op == multiplication) return left * right;
    else if (op == division)       return left / right;
    else if (op == "max")          return max(left, right);
    else if (op == "min")          return min(left, right);
    else if (op == "sin")          return sin(right);
    else if (op == "cos")          return cos(right);
    else if (op == "tan")          return tan(right);
    else                           return 0.0;
}

// Evaluates postfix expression and returns a result
bool evaluatePostfix(const vector<string>& postfix,
        double& result)
{
    stack<double> numbers;
    
    // Iterate through postfix
    for (const string& i : postfix)
    {
        // Check if it is operator
        if (isOperator(i) || checkFunction(i))
        {
            if (numbers.empty()) return false;
            double left, right;
            right = numbers.top();
            numbers.pop();
            if (!checkUnary(i))
            {
                if (numbers.empty()) return false;
                left = numbers.top();
                numbers.pop();
            }
            numbers.push(applyOperator(i, left, right));
        }
        
        // Numbers pushed directly to the stack
        else numbers.push(atof(i.c_str()));
    }
    
    result = numbers.top();
    return !numbers.empty() && (numbers.size() == 1);
}

int main()
{
    // Take Expression to be evaluated
    
    cout << "Enter Q to exit." << endl;
    string str;
  
    while (true)
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
        if (!shuntingYard(expression, postfix) || expression.empty())
            cout << "Mismatched Parentheses." << endl;
        
        else 
        {
            // Simplify
            double answer = 0;
            
            cout << "Postfix: " << endl;
            for (const string& i : postfix)
                cout << i << ' ';
            cout << endl;
            
            if (!evaluatePostfix(postfix, answer))
                cout << "Expression was malformed." << endl;

            // Return answer
            else cout << answer << endl;
        }
    }
    
    return 0;
}

