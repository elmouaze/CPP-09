#include <iostream>
#include <vector>
#include <deque>
#include <exception>
#include <climits>
#include <algorithm>
#include <ctime>

class PmergeMe
{

    private:
        std::vector<int> _numbers;
        std::vector<int> _sortedNumbers;
        std::deque<int> _dequeNumbers;
        std::clock_t _DstartTime;
        std::clock_t _DendTime;
        std::clock_t _VstartTime;
        std::clock_t _VendTime;



    public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void magic(char **arg,int size);
    void _handle_vector(char **arg, int size);
    void _handle_deque(char **arg, int size);

    void _display();
    int convert_to_int(char *str);
    class InvalidArgumentException : public std::exception
    {
        public:
        const char *what() const throw();
    };
};