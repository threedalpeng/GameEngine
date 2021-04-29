#include "ServiceLocator.h"

ServiceLocator::ServiceLocator()
{
}

std::unordered_map<std::type_index, void*> ServiceLocator::services = std::unordered_map<std::type_index, void*>();