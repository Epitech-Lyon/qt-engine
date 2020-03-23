/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAbstractItemModel
*/

#include "EAbstractItemModel.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EAbstractItemModel::init(AObject *object)
{
	EObject::init(object);
}

template<> libraryObjects::LibraryFunction *libraryObjects::EAbstractItemModel::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::EAbstractItemModel::code(AObject *object)
{
	return EObject::code(object);
}
