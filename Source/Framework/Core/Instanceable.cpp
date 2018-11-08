//
// Instanceable.cpp
//
// @author Roberto Cano
//

#include "Instanceable.hpp"

using namespace Framework::Core;
using namespace Framework::Types;

int Instanceable::gInstanceNumber = 0;

const InstanceId& Instanceable::getInstanceId() const
{
	return _instanceId;
}

void Instanceable::generateInstanceId(const std::string& classId)
{
	gInstanceNumber++;

	_instanceId = classId + std::to_string(gInstanceNumber);
	_name = _instanceId;
}

void Instanceable::setName(const std::string& name)
{
	_name = name;
}

const std::string& Instanceable::getName() const
{
	return _name;
}