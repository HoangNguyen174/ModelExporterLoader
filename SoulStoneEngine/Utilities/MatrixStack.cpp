#include "MatrixStack.hpp"

MatrixStack::MatrixStack()
{
	Matrix44 temp = Matrix44::CreateIdentity();
	m_matrixStack.push_back(temp);
}

void MatrixStack::LoadIdentity()
{
	m_matrixStack.push_back(Matrix44::CreateIdentity());
}

void MatrixStack::PushMatrix(const Matrix44& matrix)
{
	Matrix44 topOfStack = m_matrixStack.back();

	topOfStack = topOfStack.TransformMatrix(matrix);
	//m_matrixStack.pop_back();
	m_matrixStack.push_back(topOfStack);
}

void MatrixStack::SaveCurrentMatrixStack()
{
	int index;
	index = m_matrixStack.size() - 1;
	m_markerList.push_back(index);
}

void MatrixStack::RestoreToPreviousMatrixStack()
{
	if(m_markerList.size() > 0)
	{
		for(int i = m_matrixStack.size() - 1; i > m_markerList.back(); i-- )
			m_matrixStack.pop_back();
		m_markerList.pop_back();
	}
}

const float* MatrixStack::StackTop()
{
	return m_matrixStack.back().m_matrix;
}

void MatrixStack::ClearMatrixStack()
{
	m_matrixStack.clear();
	std::vector<Matrix44>().swap(m_matrixStack);
	m_markerList.clear();
	std::vector<int>().swap(m_markerList);
}