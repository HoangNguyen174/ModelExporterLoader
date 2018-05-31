#include "BaseObject.hpp"

BaseObject::BaseObject()
{
	m_parent = nullptr;
	m_mesh = nullptr;
}

void BaseObject::Render()
{
	std::vector<BaseObject*> nodeStack;

	nodeStack.push_back(this);
	while( nodeStack.size() != 0 )
	{
		BaseObject* object = nodeStack.back();
		nodeStack.pop_back();
		Matrix44 temp;
		if( object->m_mesh != nullptr )
		{
			temp = object->GetWorldTransformation();
			object->m_mesh->m_toWorldTransformation = &temp;//;object->GetWorldTransformation();
			object->m_mesh->m_toWorldTransformationNoTranslation = &Matrix44::CancelTranslation( temp );//&object->GetWorlTransformationForDirection();
			object->m_mesh->m_localToParentTransformation = &object->m_localToParentTranformation;
			object->m_mesh->Render();
		}

		for( unsigned int childIndex = 0; childIndex < object->m_childList.size(); childIndex++ )
		{
			nodeStack.push_back(object->m_childList[childIndex]);
		}
	}

}


Matrix44 BaseObject::GetWorldTransformation()
{
	if( m_parent == nullptr )
		return m_localToParentTranformation;

	return m_parent->GetWorldTransformation() * m_localToParentTranformation;
}

Matrix44 BaseObject::GetWorlTransformationForDirection()
{
	Matrix44 temp;
	temp = Matrix44::CancelTranslation( m_localToParentTranformation );

	if( m_parent == nullptr )
	{
		return temp;
	}

	return m_parent->GetWorlTransformationForDirection() * temp;
}

