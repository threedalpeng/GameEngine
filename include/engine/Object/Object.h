#pragma once
#include "cg/cgut.h"

using ObjectId = unsigned long long;

class Object
{
public:
	Object();
	virtual ~Object();

	ObjectId objectId();

private:
	ObjectId _objectId;
	static ObjectId _idCount;
};