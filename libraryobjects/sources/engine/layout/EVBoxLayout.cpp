/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EVBoxLayout
*/

#include "EVBoxLayout.hpp"
#include "EBoxLayout.hpp"

template<> QIcon libraryObjects::EVBoxLayout::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EVBoxLayout::libraryFunction()
{
	return EBoxLayout::libraryFunction();
}