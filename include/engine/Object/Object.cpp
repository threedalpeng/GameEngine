#include "Object.h"

ObjectId Object::_idCount = 0;

Object::Object() {
	_objectId = _idCount++;
}

Object::~Object()
{
}

ObjectId Object::objectId() {
	return _objectId;
}