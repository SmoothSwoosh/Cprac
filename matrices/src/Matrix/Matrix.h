#ifndef MATRIX_H
#define MATRIX_H

#include <utility>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <tuple>
#include "iostream"
#include "../Exceptions/Exceptions.h"
#include "../Rational_number/Rational_number.h"

using Slice_coords = std::tuple<int, int, int, int>;
using Cell = std::tuple<int, int>;
using Dimensions = std::tuple<int, int>;

/**
 * @brief Matrix_coords class
 * 
 */
class Matrix_coords {
    /**
     * @brief Left corner coords
     */
    Cell _left_corner;
    /**
     * @brief Right corner coords
     */
    Cell _right_corner;
public:
    /**
     * @brief Constructor
     * 
     */
    Matrix_coords() {
        _left_corner = {0, 0};
        _right_corner = {0, 0};
    }

    /**
     * @brief Constructor from indices
     * 
     */
    Matrix_coords(int r1, int c1, int r2, int c2) {
        _left_corner = {r1, c1};
        _right_corner = {r2, c2};
    }

    /**
     * @brief Get coords
     * 
     * @return Slice_coords slicing coords
     */
    Slice_coords get_coords() const {
        auto& [r1, c1] = _left_corner;
        auto& [r2, c2] = _right_corner;
        return {r1, c1, r2, c2};
    }
};

/**
 * @brief Matrix_row_coord class
 * 
 */
class Matrix_row_coord {
    /**
     * @brief Coord
     */
    int _coord;
public:
    /**
     * @brief Constructor
     * 
     */
    Matrix_row_coord(int coord = 0) : _coord(coord) {}

    /**
     * @brief Get coords
     * 
     * @return int
     */
    int get_coords() const {
        return _coord;
    }
};

/**
 * @brief Matrix_row_coord class
 * 
 */
class Matrix_column_coord {
    /**
     * @brief Coord
     */
    int _coord;
public:
    /**
     * @brief Constructor
     * 
     */
    Matrix_column_coord(int coord = 0) : _coord(coord) {}

    /**
     * @brief Get coords
     * 
     * @return int
     */
    int get_coords() const {
        return _coord;
    }
};

template<typename T>
class Matrix;

/**
 * @brief Enum for different types of slices
 * 
 */
enum class Proxy_type {
    ROW,
    COLUMN,
    FULL,
};

/**
 * @brief Proxy class
 * 
 * @tparam T 
 */
template<typename T>
class Matrix_proxy {
    /**
     * @brief Matrix<T> pointer
     */
    Matrix<T>* _matrix;
    /**
     * @brief Slicing coords
     */
    Matrix_coords _slicing{};
    /**
     * @brief type of proxy
     */
    Proxy_type _proxy_type;
public:
    /**
     * @brief Constructor from Matrix_row_coord
     * 
     * @param matrix proxy matrix
     * @param matrix_row matrix_row_coord
     */
    Matrix_proxy(Matrix<T>* matrix, Matrix_row_coord matrix_row) :
                 _matrix(matrix), _proxy_type(Proxy_type::ROW) {
        matrix->add_proxy(this);
        auto [n, m] = matrix->get_dimentions();
        auto row = matrix_row.get_coords();
        _slicing = Matrix_coords{row, 0, row, m - 1};
    }

    /**
     * @brief Constructor from Matrix_column_coord
     * 
     * @param matrix proxy matrix
     * @param matrix_column matrix_column_coord
     */
    Matrix_proxy(Matrix<T>* matrix, Matrix_column_coord matrix_column) :
                 _matrix(matrix), _proxy_type(Proxy_type::COLUMN) {
        matrix->add_proxy(this);
        auto [n, m] = matrix->get_dimentions();
        auto column = matrix_column.get_coords();
        _slicing = Matrix_coords{0, column, n - 1, column};
    }

    /**
     * @brief Constructor from Matrix_coords
     * 
     * @param matrix proxy matrix
     * @param coords Matrix_coords coords
     */
    Matrix_proxy(Matrix<T>* matrix, Matrix_coords coords) :
                 _matrix(matrix), _proxy_type(Proxy_type::FULL) {
        matrix->add_proxy(this);
        auto [n, m] = matrix->get_dimentions();
        auto [r1, c1, r2, c2] = coords.get_coords();
        if (r1 == -1) {
            r1 = 0;
        }
        if (c1 == -1) {
            c1 = 0;
        }
        if (r2 == -1) {
            r2 = n - 1;
        }
        if (c2 == -1) {
            c2 = m - 1;
        }
        _slicing = Matrix_coords{r1, c1, r2, c2};
    }

    /**
     * @brief Access operator
     * 
     * @param i first index
     * @param j second index
     * @return T& 
     */
    T& operator()(int i, int j) {
        if (_matrix == nullptr) {
            throw MatrixException("Proxy nullptr!");
        }
        auto [r1, c1, r2, c2] = _slicing.get_coords();
        if (!(r1 + i > r2 || c1 + j > c2)) {
            return _matrix->operator()(r1 + i, c1 + j);
        } else {
            throw MatrixException("Indices of slice are wrong!");
        }
    }

    /**
     * @brief Remove proxy
     * 
     */
    void remove() {
        _matrix = nullptr;
    }
};

/**
 * @brief Class template for matrices
 * 
 * @tparam T 
 */
template<typename T>
class Matrix {
    /**
     * @brief Data type std::map<Cell, T>
     */
    using Data = std::map<Cell, T>;
    /**
     * @brief data of matrix
     */
    Data _data{};
    /**
     * @brief epsilon
     */
    double _eps = 0.0001;
    /**
     * @brief matrix dimentions
     */
    Dimensions _dimentions{0, 0};
    /**
     * @brief set of proxies
     */
    std::set<Matrix_proxy<T>*> _proxies{};

    void delete_zeros() {
        std::set<Cell> zeros;
        for (const auto& [key, value] : _data) {
            if (double(value) < _eps && double(value) > -_eps) {
                zeros.insert(key);
            }
        }

        for (const auto& key : zeros) {
            _data.erase(key);
        }
    }

public:
    /**
     * @brief Destructor
     * 
     */
    ~Matrix() {
        for (auto proxy: _proxies) {
            proxy->remove();
        }
    };

    /**
     * @brief Constructor
     * 
     * @param n first dimention
     * @param m second dimention
     * @param value value
     * @param eps epsilon
     * @param identity if needs identity matrix
     */
    Matrix(int n = 0, int m = 0, T value = 0, double eps = 0.0001, bool identity = false) {
        _dimentions = {n, m};
        _eps = eps;

        if (identity) {
            if (value == 0) value = 1;
            for (int i = 0; i < n; ++i) {
                _data[{i, i}] = value;
            }
        } else if (!(double(value) < eps && double(value) > -eps)) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    _data.insert({{i, j}, value});
                }
            }
        }
    }

    /**
     * @brief Constructor from file
     * 
     * @param filename filename
     */
    Matrix(const std::string& filename) {
        std::ifstream in;
        in.open(filename);

        if (!in.is_open()) {
            throw FileException("Can not open file!");
        }

        std::string line;
        bool got_dimentions = false;
        while (std::getline(in, line)) {
            std::stringstream ss;
            ss << line;

            std::string word;
            if (!(ss >> word) || word[0] == '#') { 
                continue; 
            }
            
            if (!got_dimentions) {
                if (word != "matrix") { 
                    throw FileException("Wrong input - expected matrix!");
                }
                ss >> word;
                if (word != "rational") { 
                    throw FileException("Wrong input - expected rational!");
                }
                int n, m;
                if (!(ss >> n >> m)) {
                    throw FileException("Wrong input - expected dimentions!");
                }
                if (ss >> word && word[0] != '#') {
                    throw FileException("Wrong input - does not expect comment!");
                }
                got_dimentions = true;
                _dimentions = {n, m};
            } else {
                int i = std::stoi(word), j;
                if (!(ss >> j)) { 
                    throw FileException("Wrong input - expected index!");
                }
                std::string num;
                while (ss >> word) {
                    num += ' ';
                    num += word;
                }
                T value{num.c_str()};

                auto [n, m] = _dimentions;
                if (i < 0 || i >= n || j < 0 || j >= m) {
                    throw FileException("Wrong input - indices are out of range!");
                }

                if (double(value) < _eps && double(value) > -_eps) {
                    _data.erase({i, j});
                } else {
                    _data[{i, j}] = value;
                }
            }
        }
        in.close();
        delete_zeros();
    }

    /**
     * @brief Constructor from dimentions
     * 
     * @param dimentions matrix dimentions
     */
    explicit Matrix(const Dimensions& dimentions) : _dimentions(dimentions) {};

    /**
     * @brief Copy constructor
     * 
     * @param other matrix to copy
     */
    Matrix(const Matrix& other) {
        _dimentions = other._dimentions;
        _data = other._data;
    }

    /**
     * @brief Assignment operator
     * 
     * @param other matrix to copy
     * @return Matrix& 
     */
    Matrix& operator=(const Matrix& rhs) {
        _dimentions = rhs._dimentions;
        _data = rhs._data;
        return *this;
    }

    /**
     * @brief Move constructor
     * 
     * @param other matrix to move
     */
    Matrix(Matrix&& other) noexcept {
        _dimentions = std::move(other._dimentions);
        _data = std::move(other._data);
    }

    /**
     * @brief Move assignment operator
     * 
     * @param other matrix to move
     * @return Matrix& 
     */
    Matrix& operator=(Matrix&& rhs) noexcept {
        _dimentions = std::move(rhs._dimentions);
        _data = std::move(rhs._data);
        return *this;
    }

    /**
     * @brief += operator
     * 
     * @param rhs matrix with whom sum
     * @return Matrix&
     */
    Matrix& operator+=(const Matrix& rhs) {
        if (_dimentions != rhs._dimentions) {
            throw MatrixException("Dimentions of matrices are not equal!");
        }
        for (const auto& [key, value] : rhs._data) {
            _data[key] += value;
        }
        delete_zeros();
        return *this;
    }

    /**
     * @brief -= operator
     * 
     * @param rhs matrix with whom minus
     * @return Matrix&
     */
    Matrix& operator-=(const Matrix& rhs) {
        *this += -rhs;
        delete_zeros();
        return *this;
    }

    /**
     * @brief *= operator
     * 
     * @param rhs matrix with whom multiply
     * @return Matrix& 
     */
    Matrix& operator*=(const Matrix& rhs) {
        auto [lhs_n, lhs_m] = _dimentions;
        auto [rhs_n, rhs_m] = rhs._dimentions;

        if (lhs_m != rhs_n) {
            throw MatrixException("Dimentions are not compatible!");
        }

        Matrix res{lhs_n, rhs_m};
        for (const auto& [left_coords, left_value] : _data) {
            auto [left_r, left_c] = left_coords;
            for (const auto& [right_coords, right_value] : rhs._data) {
                auto [right_r, right_c] = right_coords;
                if (left_c == right_r) {
                    res._data[{left_r, right_c}] += left_value * right_value; 
                }
            }
        }

        res.delete_zeros();

        return *this = res;
    }

    /**
     * @brief unary - operator
     * 
     * @return Matrix
     */
    Matrix operator-() const {
        Matrix copy = *this;
        for (const auto& [key, value] : copy._data) {
            copy._data[key] = -copy._data[key];
        }
        copy.delete_zeros();
        return copy;
    }

    /**
     * @brief + operator
     * 
     * @param lhs left matrix
     * @param rhs right matrix
     * @return Matrix
     */
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
        lhs += rhs;
        lhs.delete_zeros();
        return lhs;
    }

    /**
     * @brief - operator
     * 
     * @param lhs left matrix
     * @param rhs right matrix
     * @return Matrix
     */
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
        lhs -= rhs;
        lhs.delete_zeros();
        return lhs;
    }

    /**
     * @brief * operator
     * 
     * @param lhs left matrix
     * @param rhs right matrix
     * @return Matrix
     */
    friend Matrix operator*(Matrix lhs, const Matrix& rhs) {
        lhs *= rhs;
        lhs.delete_zeros();
        return lhs;
    }

    /**
     * @brief Matrix * value operator
     * 
     * @param lhs left matrix
     * @param value value of type T
     * @return Matrix
     */
    friend Matrix operator*(const Matrix& lhs, T value) {
        Matrix copy{lhs};
        copy *= value;
        copy.delete_zeros();
        return copy;
    }

    /**
     * @brief value * Matrix operator
     * 
     * @param value value of type T
     * @param rhs right matrix
     * @return Matrix
     */
    friend Matrix operator*(T value, const Matrix& lhs) {
        Matrix copy{lhs};
        copy *= value;
        copy.delete_zeros();
        return copy;
    }

    /**
     * @brief *= value operator
     * 
     * @param value value of type T
     * @return Matrix&
     */
    Matrix& operator*=(T value) {
        for (const auto& [key, other] : _data) {
            _data[key] *= value;
        }
        delete_zeros();
        return *this;
    }

    /**
     * @brief transpose operator
     * 
     * @return Matrix
     */
    Matrix operator~() const {
        auto [n, m] = _dimentions;
        Dimensions d = {m, n};
        Matrix transpose{d};
        for (const auto& [key, value] : _data) {
            auto [r1, c1] = key;
            transpose._data[{c1, r1}] = value;
        }
        return transpose;
    }

    /**
     * @brief == operator
     * 
     * @param lhs left matrix
     * @param rhs right matrix
     * @return true 
     * @return false 
     */
    friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
        return lhs._dimentions == rhs._dimentions && lhs._data == rhs._data;
    }

    /**
     * @brief != operator
     * 
     * @param lhs left matrix
     * @param rhs right matrix
     * @return true 
     * @return false 
     */
    friend bool operator!=(const Matrix& lhs, const Matrix& rhs) {
        return !(lhs == rhs);
    }

    /**
     * @brief Get the dimentions of matrix
     * 
     * @return Dimentions
     */
    Dimensions get_dimentions() const {
        return _dimentions;
    }

    /**
     * @brief Get data from matrix
     * 
     * @return Data
     */
    Data get_data() const {
        return _data;
    }

    /**
     * @brief Access operator
     * 
     * @param i first index
     * @param j second index
     * @return T& 
     */
    T& operator()(int i, int j) {
        auto [n, m] = _dimentions;
        if (i < 0 || j < 0 || i >= n || j >= m) {
            throw MatrixException("Indices of slice are wrong!");
        }
        return _data[{i, j}];
    }

    /**
     * @brief Slice operator
     * 
     * @param coords coords of slice
     * @return Matrix_proxy<T> 
     */
    Matrix_proxy<T> operator[](const Matrix_coords& coords) {
        auto [r1, c1, r2, c2] = coords.get_coords();
        auto [n, m] = _dimentions;
        if (c1 >= n || c2 >= m) {
            throw MatrixException("Indices of slice are wrong!");
        }
        return Matrix_proxy<T>(this, coords);
    }

    /**
     * @brief Add proxy
     * 
     * @param proxy proxy
     */
    void add_proxy(Matrix_proxy<T>* proxy) {
        _proxies.insert(proxy);
    }

    /**
     * @brief Remove proxy
     * 
     * @param proxy proxy
     */
    void remove_proxy(Matrix_proxy<T>* proxy) {
        _proxies.erase(proxy);
    }

    /**
     * @brief Overload of ostream
     * 
     * @param os ostream
     * @param rhs matrix to output
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix& rhs) {
        return os << rhs.to_string();
    }

    /**
     * @brief Output matrix as a string
     * 
     * @return std::string 
     */
    std::string to_string() const {
        std::ostringstream ss;
        auto [n, m] = _dimentions;
        ss << "matrix " << "rational " << n << " " << m << "\n";
        for (const auto& [key, value] : _data) {
            auto [r1, c1] = key;
            ss << r1 << " " << c1 << " " << value << std::endl;
        }
        return ss.str();
    }
};

#endif