#include "BitcoinExchange.hpp"
#include <cmath> // std::signbit

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
    {
        this->exchange_db = other.exchange_db;
    }
    return *this;
}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
    this->exchange_db = other.exchange_db;
}
void BitcoinExchange::fill_db()
{
    std::ifstream file("data.csv");
    if (!file.is_open())
    {
        throw FileOpenException();
    }
    std::string line;
    std::getline(file, line);
    if (line != "date,exchange_rate")
    {
        file.close();
        throw InvalidFileFormatException();
    }
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string date;
        float value;
        if (std::getline(ss, date, ',') && ss >> value)
        {
            exchange_db[date] = value;
        }
    }
    file.close();
}
bool check_digit(const std::string &num)
{
    for (int i = 0; i < (int)num.length(); i++)
    {
        if (!isdigit(num[i]))
            return false;
    }
    return true;
}

bool check_date(const std::string &date)
{
   
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    std::string _year, _month, _day;
    _year = date.substr(0, 4);
    _month = date.substr(5, 2);
    _day = date.substr(8, 2);
    if (!check_digit(_year) || !check_digit(_month) || !check_digit(_day))
        return false;

    std::istringstream ss_2(date);
    int year, month, day;
    char delimiter;
    ss_2 >> year >> delimiter >> month >> delimiter >> day;

    if (month < 1 || month > 12 || day < 1 || year < 1) 
        return false;
    int days_in_month;
    switch (month)
    {
        case 2:
            days_in_month = 28;
            if ((year % 4 == 0 && year % 100) || (year % 400 == 0))
                days_in_month = 29;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            days_in_month = 30;
            break;
        default:
            days_in_month = 31;
    }

    return (day <= days_in_month);
}

bool check_value(std::string &value)
{
    int decimal = 0, i = 0;
    if (!isdigit(value[0]) && value[0] != '-' && value[0] != '+')
        return false;
    if (value[0] == '-' || value[0] == '+')
        i++;

    for (; i < (int)value.size(); i++)
    {
        if (!isdigit(value[i]) && value[i] != '.')
            return false;
        if (value[i] == '.')
        {
            ++decimal;
            if (decimal > 1 || i == (int)value.size() - 1 || !isdigit(value[i - 1]))
                return false;
        }
    }
    return true;
}

void BitcoinExchange::parse_file(const std::string &filename)
{

    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        throw FileOpenException();
    }
    std::string line;
    std::getline(file, line);
    if (line != "date | value")
    {
        file.close();
        throw InvalidFileFormatException();
    }
    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        std::string date;
        std::string value;

        if (std::getline(ss, date, '|'))
        {
            std::stringstream _date(date);
            _date >> date;
            ss >> value;
            if (ss.fail()  || !ss.eof() || value.empty() || !check_value(value))
            {
                std::cout << "Error: bad input => " << line << std::endl;
                continue;
            }
            if (date.empty() || !check_date(date))
            {
                std::cout << "Error: bad input => " << date << std::endl;
                continue;
            }
            std::stringstream ss_3(value);
            double val; 
            ss_3 >> val;
            if (ss_3.fail())
            {
                std::cout << "Error: bad input => " << value << std::endl;
                continue;
            }
            if (std::signbit(val)) // true for negatives, including -0.0
            {
                std::cout << "Error: not a positive number." << std::endl;
                continue;
            }
            if (val > 1000.0)
            {
                std::cout << "Error: too large a number." << std::endl;
                continue;
            }
            std::map<std::string, float>::iterator it = exchange_db.find(date);
            std::cout << std::fixed;
            if (it != exchange_db.end())
            {
                double exchange_rate = static_cast<double>(it->second);
                double result = val * exchange_rate;
                std::cout << date << " => " << val << " = " << result << std::endl;
            }
            else
            {
                std::map<std::string, float>::iterator it = exchange_db.lower_bound(date);
                if (it == exchange_db.begin())
                {
                    std::cout << "Error: no exchange rate available for this date." << std::endl;
                }
                else
                {
                    --it;
                    double exchange_rate = static_cast<double>(it->second);
                    double result = val * exchange_rate;
                    std::cout << date << " => " << val << " = " << result << std::endl;
                }
            }
        }
    }
    file.close();
}

BitcoinExchange::BitcoinExchange(const std::string &filename)
{
    fill_db();
    parse_file(filename);
}

const char *BitcoinExchange::FileOpenException::what() const throw()
{
    return "Error: could not open file.";
}
const char *BitcoinExchange::InvalidFileFormatException::what() const throw()
{
    return "Error: invalid file format.";
}