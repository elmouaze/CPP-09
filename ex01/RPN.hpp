#include <iostream>
#include <stack>
#include <string>
#include <sstream>
class RPN
{
    private:
        
        std::string expression;
    public:
        RPN();
        RPN(const std::string &expr);
        ~RPN();
        RPN(const RPN &other);
        RPN &operator=(const RPN &other);

        void calculate();

};