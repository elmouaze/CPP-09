#include "PmergeMe.hpp"

int main(int ac, char **argv)
{
    PmergeMe numbers;
    if (ac < 2)
    {
        std::cerr << "how to use: ./" << argv[0] << " <numbers :1 2 3 4 >" << std::endl;
        return 1;
    }
    try
    {
        numbers.magic(argv,ac);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}