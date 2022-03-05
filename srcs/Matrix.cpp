#include "../includes/Matrix.hpp"

// Constructor
Matrix::Matrix(int x, int y) {
    this->x_size = x;
    this->y_size = y;

    this->data = new double[x * y]();
}

// Destructor
Matrix::~Matrix() {
}

// Get a case value with x and y position
double Matrix::get(int x, int y) const {
    return this->data[y * x_size + x];
}

// Set a case with x and y specified
void Matrix::set(int x, int y, double value) {
    this->data[y * x_size + x] = value;
}

// Return Y size
unsigned char Matrix::getY() const {
    return this->y_size;
}

// Return X size
unsigned char Matrix::getX() const {
    return this->x_size;
}

// Debug my matrix
void Matrix::print() const {
    for (int i = 0; i < y_size; i++) {
        std::cout << '[';
        for (int j = 0; j < x_size; j++) {
            std::cout << ' ' << this->data[i * x_size + j] << ' ';
        }
        std::cout << ']' << std::endl;
    }
}

// Add matrix with current and return a new matrix
Matrix &Matrix::add(Matrix &matrix) {
//    if (matrix.getY() != this->y_size && matrix.getX() != this->x_size)
//        return

	Matrix *result = new Matrix(this->x_size, this->y_size);

    for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(x, y) + matrix.get(x, y));
	return *result;
}

// Multiply matrix with current and return a new matrix
Matrix &Matrix::dot(Matrix &matrix) {
//	if (matrix.getY() != this->y_size && matrix.getX() != this->x_size)
//		return ;

	Matrix *result = new Matrix(this->x_size, this->y_size);

	for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(x, y) * matrix.get(x, y));
	return *result;
}

// Transpose current matrix and return a new reference
Matrix &Matrix::transpose() {
	Matrix *result = new Matrix(this->y_size, this->x_size);

	for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++) {
			result->set(x, y, this->get(y, x));
		}
	return *result;
}

// Overload operator add
Matrix Matrix::operator+(Matrix &left, const Matrix &right) {

	for (int y = 0; y < left.getY(); y++)
		for (int x = 0; x < left.getX(); x++)
			left->set(x, y, right.get(x, y) + left.get(x, y));

	return left;
}



