#include "RPN.hpp"

RPN::RPN()
{
    expression = "";
}
RPN::RPN(const std::string &expr)
{
    expression = expr;
}
RPN::~RPN()
{
}
RPN::RPN(const RPN &other)
{
    expression = other.expression;
}
RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
    {
        expression = other.expression;
    }
    return *this;
}

void RPN::calculate()
{
    std::stack<long long> st;
    std::stringstream ss(expression);
    for (size_t i = 0; i < expression.size(); ++i)
    {
        char c = expression[i];

        if (c == ' ' || c == '\t')
            continue;

        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            st.push(static_cast<long long>(c - '0'));
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (st.size() < 2)
            {
                std::cout << "Error" << std::endl;
                return;
            }
            long long rhs = st.top();
            st.pop();
            long long lhs = st.top();
            st.pop();

            long long res = 0;
            switch (c)
            {
            case '+':
                res = lhs + rhs;
                break;
            case '-':
                res = lhs - rhs;
                break;
            case '*':
                res = lhs * rhs;
                break;
            case '/':
                if (rhs == 0)
                {
                    std::cout << "Error" << std::endl;
                    return;
                }
                res = lhs / rhs;
                break;
            }
            st.push(res);
            continue;
        }

        std::cout << "Error" << std::endl;
        return;
    }

    if (st.size() != 1)
    {
        std::cout << "Error" << std::endl;
        return;
    }

    std::cout << st.top() << std::endl;
}