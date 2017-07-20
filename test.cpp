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
    else return 0;
}
        
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

bool shuntingYard(const vector<string>& expression, vector<string>& postfix)
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

bool evaluatePostfix(const vector<string>& postfix, double& result)
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

bool test(vector<string>& expression, double& answer)
{
    vector<string> postfix;
    if (!shuntingYard(expression, postfix))
    {
        cout << "Mismatched parentheses" << endl;
        return false;
    }
    cout << "Postfix: ";
    for (const string& i : postfix)
        cout << i << ' ';
    cout << endl;
    if (!evaluatePostfix(postfix, answer))
    {
        cout << "The expression is invalid." << endl;
        return false;
    }
    return true;
}

int main()
{
    // Test: 3 * (4 / 5) + 2
    double answer = 0;
    cout << "Testing 3 * (4 / 5) - 2: " << endl;
    vector<string> expression = {"3", "*", "(", "4", "/", "5", ")", "-", "2"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: (3.2 + 5) / 7 * 2.1
    cout << "Testing (3.2 + 5) / 7 * 2.1: " << endl;
    expression = {"(", "3.2", "+", "5", ")", "/", "7", "*", "2.1"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: ( 3.2 + 9
    cout << "Testing ( 3.2 + 9:" << endl;
    expression = {"(", "3.2", "+", "9"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: 2.3 + 8 )
    cout << "Testing 2.3 + 8 ):" << endl;
    expression = {"2.3", "+", "8", ")"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: ( ( ( 1 + ( 2 * 4 ) / 3 ) ) * 2 )
    cout << "Testing ( ( ( 1 + ( 2 * 4 ) / 3 ) ) * 2 )" << endl;
    expression = {"(", "(", "(", "1", "+", "(", "2", "*", "4", ")", "/", "3", ")"
            , ")", "*", "2", ")"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: 4 + 4 5
    cout << "Testing 4 + 4 5" << endl;
    expression = {"4", "+", "4", "5"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: * 2.9 / 9 + 8
    cout << "Testing * 2.9 / 9 + 8:" << endl;
    expression = {"*", "2.9", "/", "9", "+", "8"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
    
    // Test: 4 + + 3
    cout << "Testing 4 + + 3" << endl;
    expression = {"4", "+", "+", "3"};
    test(expression, answer);
    cout << "Answer: " << answer << endl;
    cout << endl;
}

