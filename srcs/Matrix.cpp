#include "../includes/Matrix.hpp"

// Constructor
Matrix::Matrix(int height, int width) {
    this->x_size = width;
    this->y_size = height;

    this->data = new double[height * width]();
}

// Constructor per copy
Matrix::Matrix(Matrix const &matrix) {
	this->x_size = matrix.getWidth();
	this->y_size = matrix.getHeight();

    this->data = new double[this->x_size * this->y_size]();
	for (int x = 0; x < this->x_size; x++)
		for (int y = 0; y < this->y_size; y++)
			this->set(x, y, matrix.get(x, y));
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
int Matrix::getHeight() const {
    return this->y_size;
}

// Return X size
int Matrix::getWidth() const {
    return this->x_size;
}

// Debug my matrix
void Matrix::print() const {
	std::cout << "Matrix : " << std::endl;
    for (int i = 0; i < y_size; i++) {
        std::cout << '[';
        for (int j = 0; j < x_size; j++) {
            std::cout << ' ' << this->data[i * x_size + j] << ' ';
        }
        std::cout << ']' << std::endl;
    }
}

// Add matrix with current and return a new matrix
Matrix *Matrix::add(Matrix &matrix) {
	if (matrix.getHeight() != this->y_size && matrix.getWidth() != this->x_size)
		throw InvalidSizeError();

	Matrix *result = new Matrix(this->x_size, this->y_size);

    for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(x, y) + matrix.get(x, y));
	return result;
}

// Multiply current matrix by matrix and return a new matrix
Matrix *Matrix::dot(Matrix &matrix) {
	if (this->x_size != matrix.getHeight())
		throw InvalidSizeError();

	Matrix *result = new Matrix(this->y_size, matrix.getWidth());

	for (int i = 0; i < this->y_size; i++)
		for (int j = 0; j < matrix.getWidth(); j++) {
			double tmp = 0;
			for (int k = 0; k < this->getWidth(); k++)
				tmp += this->get(i, k) * matrix.get(k, j);
			result->set(i, j, tmp);
		}
	return result;
}

// Multiply current matrix by a scalar and return a new matrix
Matrix *Matrix::dot(double scalar) {
	Matrix *result = new Matrix(this->x_size, this->y_size);

	for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(x, y) * scalar);
	return result;
}

// Transpose current matrix and return a new reference
Matrix *Matrix::transpose() {
	Matrix *result = new Matrix(this->y_size, this->x_size);

	for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(y, x));
	return result;
}

// Overload operator add with a matrix
Matrix *Matrix::operator+(const Matrix &rhs) {
	if (rhs.getHeight() != this->y_size && rhs.getWidth() != this->x_size)
		throw InvalidSizeError();

	Matrix *result = new Matrix(this->x_size, this->y_size);

    for (int y = 0; y < this->y_size; y++)
		for (int x = 0; x < this->x_size; x++)
			result->set(x, y, this->get(x, y) + rhs.get(x, y));
	return result;
}

// Create a identity matrice
Matrix *Matrix::identity(int w, int h, double value) {
	Matrix *m = new Matrix(w, h);

	for (int i = 0; i < (w < h ? w : h); i++)
		m->set(i, i, value);
	return m;
}

Matrix *Matrix::rrf() {
	Matrix *m = new Matrix(*this);
	int i = 0;
	int lead = 0;
	int row_count = getHeight();
	int col_count = getWidth();

	for (int r = 0; r < row_count; r++) {
		if (col_count <= lead)
			return new Matrix(*m);
		i = r;
		while (m->get(lead, i) == 0) {
			i++;
			if (row_count == i) {
				i = r;
				lead++;
				if (col_count == lead)
					return new Matrix(*m);
			}
		}
		for (int j = 0; j < col_count; j++) {
			double tmp = m->get(j, i);
			m->set(j, i, m->get(j, r));
			m->set(j, r, tmp);
		}
		if (m->get(lead, r) != 0) {
			double backup = m->get(lead, r);
			for (int j = 0; j < col_count; j++)
				m->set(j, r, m->get(j, r) / backup);
		}
		for (i = 0; i < row_count; i++) {
			if (i != r) {
				double backup = m->get(lead, i);
				for (int j = 0; j < col_count; j++) {
					m->set(j, i, m->get(j, i) - backup * m->get(j, r));
				}
			}
		}
		lead++;
	}
	return m;
}

Matrix *Matrix::inverse() {
	if (getWidth() != getHeight())
		throw new InvalidSizeError();
	Matrix *identity = Matrix::identity(getWidth(), getHeight(), 1);
	Matrix *merge = new Matrix(getWidth() * 2, getHeight());
	for (int x = 0; x < getWidth(); x++) {
		for (int y = 0; y < getHeight(); y++) {
			merge->set(x, y, get(x, y));
			merge->set(x + getWidth(), y, identity->get(x, y));
		}
	}
	Matrix *row_echelon = merge->rrf();
	Matrix *result = new Matrix(getWidth(), getHeight());
	for (int x = 0; x < getWidth(); x++)
		for (int y = 0; y < getHeight(); y++)
			result->set(x, y, row_echelon->get(x + getWidth(), y));
	return result;
}
