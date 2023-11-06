#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * @brief Class for file exception
 * 
 */
class FileException : public std::exception {
    /**
     * @brief error message
     * 
     */
    std::string _err_msg;
public:
    /**
     * @brief Constructor
     * 
     */
    FileException(std::string err_msg) : _err_msg(err_msg) {}

    /**
     * @brief Return error message
     * 
     * @return const char *
     */
    const char *what() const noexcept override { return _err_msg.c_str(); }
};

/**
 * @brief Class for matrix exception
 * 
 */
class MatrixException : public std::exception {
    /**
     * @brief error message
     * 
     */
    std::string _err_msg;
public:
    /**
     * @brief Constructor
     * 
     */
    MatrixException(std::string err_msg) : _err_msg(err_msg) {}

    /**
     * @brief Return error message
     * 
     * @return const char *
     */
    const char *what() const noexcept override { return _err_msg.c_str(); }
};

/**
 * @brief Class for rational number exception
 * 
 */
class RationalNumberException : public std::exception {
    /**
     * @brief error message
     * 
     */
    std::string _err_msg;
public:
    /**
     * @brief Constructor
     * 
     */
    RationalNumberException(std::string err_msg) : _err_msg(err_msg) {}

    /**
     * @brief Return error message
     * 
     * @return const char *
     */
    const char *what() const noexcept override { return _err_msg.c_str(); }
};

#endif