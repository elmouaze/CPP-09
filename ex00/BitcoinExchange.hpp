#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <climits>
#include <exception>
#include <iomanip>
#include <cstdlib> 

class BitcoinExchange {
    
    std::map<std::string, float> exchange_db;
    public:
        BitcoinExchange();
        BitcoinExchange(const std::string& filename);
        ~BitcoinExchange();
        BitcoinExchange &operator=(const BitcoinExchange& other);
        BitcoinExchange(const BitcoinExchange& other);

        void fill_db();
        void parse_file(const std::string& filename);
        class FileOpenException : public std::exception {
        public:
             const char* what() const throw();
        };
        class InvalidFileFormatException : public std::exception {
        public:
            const char* what() const throw();
        };
    };

#endif