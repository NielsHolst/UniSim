/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MATRIX_H
#define UNISIM_MATRIX_H
#include <QVector>
#include <usbase/exception.h>

namespace UniSim {

template <class T>
class Matrix {
public:
    Matrix();
    Matrix(int numRows, int numColumns);
    void resize(int numRows, int numColumns);
    void fill(T value) { vector.fill(value); }
    void fill(int numRows, int numColumns, const QVector<T> &values);
    int numRows() const { return _numRows; }
    int numColumns() const { return _numColumns; }
    T at(int row, int col) const;
    T& operator()(int row, int col);
    bool operator==(const Matrix<T> &other) const { return vector == other.theVector(); }
    bool operator!=(const Matrix<T> &other) const { return !(*this == other); }
private:
    int index(int row, int col) const;
    const QVector<T>& theVector() const { return vector; }

    QVector<T> vector;
    int _numRows, _numColumns;
};

template <class T>
Matrix<T>::Matrix()
    : _numRows(0), _numColumns(0)
{
}

template <class T>
Matrix<T>::Matrix(int numRows, int numColumns)
{
    resize(numRows, numColumns);
}

template <class T>
void Matrix<T>::resize(int numRows, int numColumns)
{
    vector.resize(numRows*numColumns);
    _numRows = numRows;
    _numColumns = numColumns;
}

template <class T>
void Matrix<T>::fill(int numRows, int numColumns, const QVector<T> &values)
{
    if (numRows*numColumns != values.size()) {
        QString msg = "Number of rows (%1) and columns (%2) do not match the number of values (%3)";
        throw Exception(msg.arg(numRows).arg(numColumns).arg(values.size()));
    }
    resize(numRows, numColumns);
    vector = values;
}

template <class T>
T& Matrix<T>::operator()(int row, int col)
{
    return vector[index(row,col)];
}

template <class T>
T Matrix<T>::at(int row, int col) const
{
    return vector.at(index(row,col));
}

template <class T>
int Matrix<T>::index(int row, int col) const
{
    if (row < 0 || row >= _numRows) {
        QString msg = "Row index (%1) out of range (0..%2)";
        throw Exception(msg.arg(row).arg(_numRows));
    }
    if (col < 0 || col >= _numColumns) {
        QString msg = "Column index (%1) out of range (0..%2)";
        throw Exception(msg.arg(col).arg(_numColumns));
    }
    return row*_numColumns + col;
}

} //namespace

#endif
