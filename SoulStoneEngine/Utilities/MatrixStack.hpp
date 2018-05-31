#ifndef MATRIX_STACK
#define MATRIX_STACK
#include "Matrix44.hpp"
#include <vector>

class MatrixStack
{
	public:
		std::vector<Matrix44> m_matrixStack;
	
	private:
		std::vector<int> m_markerList;

	public:
		MatrixStack();
		void LoadIdentity();
		void PushMatrix(const Matrix44& matrix);
		void SaveCurrentMatrixStack();
		void RestoreToPreviousMatrixStack();
		void ClearMatrixStack();
		const float* StackTop();
};

#endif