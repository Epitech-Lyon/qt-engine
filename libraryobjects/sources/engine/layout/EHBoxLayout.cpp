/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EHBoxLayout
*/

#include "EHBoxLayout.hpp"
#include "EBoxLayout.hpp"

template<> QIcon libraryObjects::EHBoxLayout::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EHBoxLayout::libraryFunction()
{
	return EBoxLayout::libraryFunction();
}
