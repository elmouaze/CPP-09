#include "PmergeMe.hpp"
#include <algorithm>
#include <cctype>
#include <climits>

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

template <typename T>
static bool is_sorted_nondec(const T &v)
{
    if (v.size() < 2)
        return true;
    for (size_t i = 1; i < v.size(); ++i)
        if (v[i - 1] > v[i])
            return false;
    return true;
}

int jacobsthal(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;  
    return jacobsthal(n - 1) + 2 * jacobsthal(n - 2);
}

template <typename T>
static void swapBlocks(T &v, size_t a, size_t b, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        std::swap(v[a + i], v[b + i]);
}

template <typename T>
static bool lessByMax(const T &v, size_t blk, size_t left_start, size_t right_start)
{
    return (v[left_start + blk - 1] < v[right_start + blk - 1]);
}

template <typename T>
static void insertBlock(std::vector<size_t> &mainIdx, size_t bIdx,
                        T &v, size_t blk,
                        std::vector<size_t>::iterator boundIt)
{
    std::vector<size_t>::iterator first = mainIdx.begin();
    std::vector<size_t>::iterator last = boundIt;
    while (first < last)
    {
        std::vector<size_t>::iterator mid = first + (last - first) / 2;
        if (lessByMax(v, blk, bIdx, *mid))
            last = mid;
        else
            first = mid + 1;
    }
    mainIdx.insert(first, bIdx);
}

template <typename T>
static void sortRec(T &v, size_t blk)
{
    const size_t total = v.size();
    if (blk == 0)
        return;

    const size_t elem_count = total / blk;
    if (elem_count < 2)
        return;

    for (size_t p = 0; p + 1 < elem_count; p += 2)
    {
        size_t L = p * blk;
        size_t R = (p + 1) * blk;
        if (v[L + blk - 1] > v[R + blk - 1])
            swapBlocks(v, L, R, blk);
    }

    sortRec(v, blk * 2);

    const size_t pair_count = elem_count / 2;
    const bool has_odd = (elem_count % 2 != 0);

    std::vector<size_t> main_idx;
    if (elem_count >= 2)
    {
        main_idx.push_back(0 * blk);
        main_idx.push_back(1 * blk);
    }
    for (size_t k = 1; k < pair_count; ++k)
        main_idx.push_back((2 * k + 1) * blk);

    std::vector<size_t> pending_idx;
    for (size_t k = 1; k < pair_count; ++k)
        pending_idx.push_back((2 * k) * blk);
    if (has_odd)
        pending_idx.push_back((elem_count - 1) * blk);

    if (!pending_idx.empty())
    {
        size_t b_idx = pending_idx[0];
        size_t a_idx = b_idx;
        if (b_idx + blk <= (elem_count * blk))
            a_idx = b_idx + blk;
        std::vector<size_t>::iterator bound_it = std::find(main_idx.begin(), main_idx.end(), a_idx);
        std::vector<size_t>::iterator limit_it = main_idx.end();
        if (bound_it != main_idx.end())
            limit_it = bound_it + 1;
        insertBlock(main_idx, b_idx, v, blk, limit_it);
    }

    std::vector<int> jac_order;
    int j = 1;
    while (jacobsthal(j) < (int)pending_idx.size())
    {
        jac_order.push_back(jacobsthal(j));
        ++j;
    }
    for (int i = 1; i <= (int)pending_idx.size(); ++i)
        if (std::find(jac_order.begin(), jac_order.end(), i) == jac_order.end())
            jac_order.push_back(i);

    std::vector<bool> inserted_flags(pending_idx.size(), false);
    if (!pending_idx.empty())
        inserted_flags[0] = true;

    for (size_t t = 0; t < jac_order.size(); ++t)
    {
        int pos = jac_order[t];
        if (pos <= 0 || pos > (int)pending_idx.size() || inserted_flags[pos - 1])
            continue;
        const size_t b_idx = pending_idx[pos - 1];
        size_t a_idx = b_idx;
        if (b_idx + blk <= (elem_count * blk))
            a_idx = b_idx + blk;
        std::vector<size_t>::iterator bound_it = std::find(main_idx.begin(), main_idx.end(), a_idx);
        std::vector<size_t>::iterator limit_it = main_idx.end();
        if (bound_it != main_idx.end())
            limit_it = bound_it + 1;
        insertBlock(main_idx, b_idx, v, blk, limit_it);
        inserted_flags[pos - 1] = true;
    }

    T snapshot = v;
    const size_t head = elem_count * blk;
    size_t off = 0;
    for (size_t i = 0; i < main_idx.size(); ++i)
        for (size_t t = 0; t < blk; ++t)
            v[off++] = snapshot[main_idx[i] + t];
    for (size_t i = head; i < total; ++i)
        v[i] = snapshot[i];
}

void PmergeMe::magic(char **arg, int size)
{
    _handle_vector(arg, size);
    _handle_deque(arg, size);
    _display();
}

void PmergeMe::_handle_vector(char **arg, int size)
{
    _VstartTime = std::clock();
    for (int i = 1; i < size; i++)
    {
        int num = convert_to_int(arg[i]);
        _numbers.push_back(num);
    }
    _sortedNumbers = _numbers;
    if (_sortedNumbers.size() > 1)
        sortRec(_sortedNumbers, 1);

    _VendTime = std::clock();

}
void PmergeMe::_handle_deque(char **arg, int size)
{
    _DstartTime = std::clock();
    for (int i = 1; i < size; i++)
    {
        int num = convert_to_int(arg[i]);
        _dequeNumbers.push_back(num);
    }
    if (_dequeNumbers.size() > 1)
        sortRec(_dequeNumbers, 1);
    _DendTime = std::clock();
}

int PmergeMe::convert_to_int(char *str)
{
    unsigned long res = 0;
    for (int i = 0; str[i]; i++)
    {
        if (!isdigit((unsigned char)str[i]))
            throw InvalidArgumentException();
        res = res * 10 + (str[i] - '0');
        if (res > INT_MAX)
            throw InvalidArgumentException();
    }
    return (static_cast<int>(res));
}


const char *PmergeMe::InvalidArgumentException::what() const throw()
{
    return "Error: Invalid Argument.";
}

void PmergeMe::_display()
{
    std::cout << "Before : ";
    for (size_t i = 0; i < _numbers.size(); i++)
        std::cout << _numbers[i] << " ";
    std::cout << std::endl;

    std::cout << "After  : ";
    for (size_t i = 0; i < _sortedNumbers.size(); i++)
        std::cout << _sortedNumbers[i] << " ";
    std::cout << std::endl;
    std::cout << "Time to process a range of "<< _numbers.size() << " elements with std::vector : " << static_cast<double>(_VendTime - _VstartTime) << " us" << std::endl;
    std::cout << "Time to process a range of "<< _dequeNumbers.size() << " elements with std::deque  : " << static_cast<double>(_DendTime - _DstartTime) << " us" << std::endl;
}