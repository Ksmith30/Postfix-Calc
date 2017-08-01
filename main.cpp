#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
using namespace std;

// Returns true if given a function name
bool checkFunction(const string& temp)
{
    return (temp == "max" || temp == "min" || temp == "sin" || temp == "cos" ||
            temp == "tan");
}

// Returns if given string is an operator
bool isOperator(const string& temp)
{
    return (temp == "+" || temp == "-" || temp == "*" || temp == "/");
}

// Returns precedence of operators
int getPrecedence(const string &temp)
{
    if (temp == "+" || temp == "-")      return 1;
    else if (temp == "*" || temp == "/") return 2;
    else if (checkFunction(temp))        return 3;
    else                                 return 0;
}

// Returns if left Parentheses is found
bool findLeftParentheses(stack<string>& track, vector<string>& postfix)
{
    while (!track.empty())
    {
        if (track.top() == "(")
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
bool shuntingYard(const vector<string>& expression, const int startIndex,
        vector<string>& postfix)
{
    stack<string> track;
    
    for (int i = startIndex; i < expression.size(); ++i)
    {
        if (isOperator(expression[i]))
        {
            while (!track.empty() && 
                    (getPrecedence(track.top()) >= getPrecedence(expression[i])))
            {
                postfix.push_back(track.top());
                track.pop();
            }
            track.push(expression[i]);
        }
        else if (expression[i] == "(" || checkFunction(expression[i]))
            track.push(expression[i]);
        else if (expression[i] == ")")
        {
            if (!findLeftParentheses(track, postfix))
                return false;
        }
        else
            postfix.push_back(expression[i]);
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
    if      (op == "+")            return left + right;
    else if (op == "-")            return left - right;
    else if (op == "*")            return left * right;
    else if (op == "/")            return left / right;
    else if (op == "max")          return max(left, right);
    else if (op == "min")          return min(left, right);
    else if (op == "sin")          return sin(right);
    else if (op == "cos")          return cos(right);
    else if (op == "tan")          return tan(right);
    else                           return 0.0;
}

// Evaluates postfix expression and returns a result
bool evaluatePostfix(const vector<string>& postfix, const map<string, double>& variables,
        double& result)
{
    stack<double> numbers;
    
    // Iterate through postfix
    for (const string& i : postfix)
    {
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
        
        // Check if variable exists
        else if (variables.count(i) > 0)
            numbers.push(variables.at(i));
        
        // Numbers pushed directly to the stack
        else 
            numbers.push(atof(i.c_str()));
    }
    
    result = numbers.top();
    return !numbers.empty() && (numbers.size() == 1);
}

int main()
{  
    cout << "Enter Q to exit." << endl;
    string str;
    map<string, double> variables;
  
    while (true)
    {
        // Initialize 
        vector<string> expression;
        int startIndex = 0;
        string variableName = " ";
        cout << ">> ";
        
        // Get Expression
        getline(cin, str);
        if (str == "Q")
            break;
        stringstream ss(str);
        while (ss >> str)
            expression.push_back(str);
        
        // Check if variable
        for (const string& i : expression)
        {
            if (i == "=") 
            {
                startIndex = 2;
                variableName = expression[0];
            }
        }
               
        // Convert to postfix 
        vector<string> postfix;
        if (!shuntingYard(expression, startIndex, postfix) || expression.empty())
            cout << "Mismatched Parentheses." << endl;
        
        else 
        {
            // Simplify
            double answer = 0;
            
            if (!evaluatePostfix(postfix, variables, answer))
                cout << "Expression was malformed." << endl;
            
            // Return answer
            else
            {
                cout << answer << endl;
                if (variableName != " ")
                    variables[variableName] = answer;
            }
        }
    }
    return 0;
}

