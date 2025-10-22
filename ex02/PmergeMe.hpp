#include <iostream>
#include <vector>
#include <deque>
#include <exception>
#include <climits>
#include <algorithm>
class PmergeMe
{

    private:
        std::vector<int> _numbers;
        std::vector<int> _sortedNumbers;
        std::deque<int> _dequeNumbers;

    public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void magic(char **arg,int size);
    void _parse(char **arg,int size);
    void _sort();
    
    void _display();
    int convert_to_int(char *str);
    class InvalidArgumentException : public std::exception
    {
        public:
        const char *what() const throw();
    };
};