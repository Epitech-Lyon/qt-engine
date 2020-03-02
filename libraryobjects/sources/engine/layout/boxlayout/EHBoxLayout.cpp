/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EHBoxLayout
*/

#include "EHBoxLayout.hpp"
#include "EBoxLayout.hpp"

template<> void libraryObjects::EHBoxLayout::init(AObject *object)
{
	EBoxLayout::init(object);
}

template<> QIcon libraryObjects::EHBoxLayout::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EHBoxLayout::libraryFunction()
{
	return EBoxLayout::libraryFunction();
}
