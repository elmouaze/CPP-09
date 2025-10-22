#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other)
{
    _numbers = other._numbers;
    _sortedNumbers = other._sortedNumbers;
    _dequeNumbers = other._dequeNumbers;
}
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _numbers = other._numbers;
        _sortedNumbers = other._sortedNumbers;
        _dequeNumbers = other._dequeNumbers;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}
void PmergeMe::magic(char **arg,int size)
{
    _parse(arg, size);
     _sort();
    _display();
}

void PmergeMe::_parse(char **arg,int size)
{
    for (int i = 1; i < size; i++)
    {
        int num = convert_to_int(arg[i]);
        _numbers.push_back(num);
    }
}
int PmergeMe::convert_to_int(char *str)
{
    unsigned long res = 0;

    for (int i = 0; str[i]; i++)
    {
        res = res * 10 + (str[i] - '0');
        if (!isdigit(str[i]) || res > INT_MAX)
        {
            throw InvalidArgumentException();
        }
    }
    return (static_cast<int>(res));
}

int jacobsthal(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return jacobsthal(n - 1) + 2 * jacobsthal(n - 2);
}
bool comparePairs(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return a.second < b.second;
}
void PmergeMe::_sort()
{
    _sortedNumbers = _numbers;
    if (_sortedNumbers.size() <= 1)
        return;

    // --- Step 1: Pair elements and sort each pair internally ---
    std::vector< std::pair<int, int> > pairs;
    for (size_t i = 0; i + 1 < _sortedNumbers.size(); i += 2)
    {
        int x = _sortedNumbers[i];
        int y = _sortedNumbers[i + 1];
        if (x > y)
            std::swap(x, y);
        pairs.push_back(std::make_pair(x, y));
    }

    // --- Step 2: If odd count, remember the leftover element ---
    bool hasOdd = (_sortedNumbers.size() % 2 != 0);
    int leftover = hasOdd ? _sortedNumbers.back() : 0;

    // --- Step 3: Sort pairs by the larger element (the second) ---
    std::sort(pairs.begin(), pairs.end(), comparePairs);

    // --- Step 4: Build main chain (larger elements) and pending (smaller ones) ---
    std::vector<int> mainChain;
    std::vector<int> pending;

    for (size_t i = 0; i < pairs.size(); ++i)
    {
        mainChain.push_back(pairs[i].second);
        pending.push_back(pairs[i].first);
    }

    // --- Step 5: Insert first pending element at start ---
    if (!pending.empty())
    {
        std::vector<int>::iterator it = std::upper_bound(mainChain.begin(), mainChain.end(), pending[0]);
        mainChain.insert(it, pending[0]);
    }

    // --- Step 6: Determine Jacobsthal insertion order ---
    std::vector<int> order;
    int j = 1;
    while (jacobsthal(j) < (int)pending.size())
        order.push_back(jacobsthal(j++));
    for (int i = 1; i <= (int)pending.size(); ++i)
    {
        if (std::find(order.begin(), order.end(), i) == order.end())
            order.push_back(i);
    }

    // --- Step 7: Insert pending elements following Jacobsthal order ---
    std::vector<bool> inserted(pending.size(), false);
    inserted[0] = true;

    for (size_t i = 0; i < order.size(); ++i)
    {
        int pos = order[i];
        if (pos <= 0 || pos > (int)pending.size() || inserted[pos - 1])
            continue;

        int val = pending[pos - 1];
        std::vector<int>::iterator it =
            std::upper_bound(mainChain.begin(), mainChain.end(), val);
        mainChain.insert(it, val);
        inserted[pos - 1] = true;
    }

    // --- Step 8: Insert leftover if there was one ---
    if (hasOdd)
    {
        std::vector<int>::iterator it =
            std::upper_bound(mainChain.begin(), mainChain.end(), leftover);
        mainChain.insert(it, leftover);
    }

    // --- Step 9: Write result back ---
    _sortedNumbers = mainChain;
}


const char *PmergeMe::InvalidArgumentException::what() const throw()
{
    return "Error: Invalid Argument.";
}

void PmergeMe::_display()
{
    std::cout << "Before: ";
    for (size_t i = 0; i < _numbers.size(); i++)
    {
        std::cout << _numbers[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "After: ";
    for (size_t i = 0; i < _sortedNumbers.size(); i++)
    {
        std::cout << _sortedNumbers[i] << " ";
    }
    std::cout << std::endl;
}