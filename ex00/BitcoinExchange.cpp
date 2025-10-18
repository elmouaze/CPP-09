#include "BitcoinExchange.hpp"

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
        // make custom exception for invalid db file format
        throw FileOpenException();
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

bool check_date(const std::string &old_date)
{
    std::istringstream ss(old_date);

    std::string date;
    ss >> date;
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    std::string _year, _month, _day;
    _year = date.substr(0, 4);
    _month = date.substr(5, 2);
    _day = date.substr(8, 2);
    if (!check_digit(_year) || !check_digit(_month) || !check_digit(_day))
        return false;

    std::istringstream ss_2(old_date);
    int year, month, day;
    char delimiter;
    ss_2 >> year >> delimiter >> month >> delimiter >> day;

    if (month < 1 || month > 12 || day < 1 || day > 31)
        return false;
    if (month < 1 && month > 12)
        return false;
    if (year < 1)
        return false;
    return true;
}

bool check_value(std::string &value)
{
    int decimal = 0;
    if (!isdigit(value[0]) && value[0] != '-' && value[0] != '+')
        return false;
    if (value[0] == '-' || value[0] == '+')
        value = value.substr(1);
    for (int i = 0; i < (int)value.size(); i++)
    {
        if (!isdigit(value[i]) && value[i] != '.')
        {
            return false;
        }
        if (value[i] == '.')
        {
            decimal++;
            if (decimal > 1)
            {
                return false;
            }
            if (i == (int)value.size() - 1)
            {
                return false;
            }
            break;
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
        // throw input file format exception
    }
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string date;

        std::string value;
        if (std::getline(ss, date, '|'))
        {
            ss >> value;
            if (ss.fail() || value.empty() || !check_value(value))
            {
                std::cout << "Error: bad input => " << line << std::endl;
                continue;
            }
            if (date.empty() || !check_date(date))
            {
                std::cout << "Error: bad input => " << date << std::endl;
                continue;
            }
            std::istringstream ss_3(value);
            float val;
            ss_3 >> val;
            if (ss_3.fail())
            {
                std::cout << "Error: bad input => " << value << std::endl;
                continue;
            }
            if (val < 0)
            {
                std::cout << "Error: not a positive number." << std::endl;
                continue;
            }
            if (val >= static_cast<float>(INT_MAX) )
            {
                std::cout << "Error: too large a number." << std::endl;
                continue;
            }
            else
            {
                std::map<std::string, float>::iterator it = exchange_db.find(date);
                if (it != exchange_db.end())
                {
                    float exchange_rate = it->second;
                    float result = val * exchange_rate;
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << date << " => " << val << " = " << result << std::endl;
                }
                else
                {
                    // find closest previous date
                    std::map<std::string, float>::iterator it = exchange_db.lower_bound(date);
                    if (it == exchange_db.begin())
                    {
                        std::cout << "Error: no exchange rate available for this date." << std::endl;
                    }
                    else
                    {
                        --it;
                        float exchange_rate = it->second;
                        float result = val * exchange_rate;
                        std::cout << std::fixed << std::setprecision(2);
                        std::cout << date << " => " << val << " = " << result << std::endl;
                    }
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
    std::map<std::string, float>::iterator it;
    //    for (it = exchange_db.begin(); it != exchange_db.end(); ++it)
    //    {
    //        std::cout << std::fixed << std::setprecision(2);
    //        std::cout << it->first << " => " << it->second << std::endl;
    //    }
}
