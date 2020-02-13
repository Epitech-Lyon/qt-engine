/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#include "LibraryObject.hpp"
#include "AObject.hpp"

libraryObjects::LibraryObject::LibraryObject(const QString &classHierarchy, const QIcon &icon, Constructor constructor, const QMap<QString, Functions> &functions)
    : _classHierarchy(classHierarchy)
    , _className(_classHierarchy.split("::").last())
    , _icon(icon)
    , _constructor(constructor)
    , _functions(functions)
{
}

libraryObjects::AObject *libraryObjects::LibraryObject::functionAdd(AObject *parent, LibraryObject *child, int position) const
{
    auto function = this->function(child->classHierarchy()).first;

    return function ? function(parent, child, position) : nullptr;
}

void libraryObjects::LibraryObject::functionRemove(AObject *parent, AObject *child) const
{
    auto function = this->function(child->classHierarchy()).second;

    if (function)
        function(parent, child);
}

libraryObjects::LibraryObject::Functions libraryObjects::LibraryObject::function(const QString &childClassHierarchy) const
{
    for (auto acceptedClassHierarchy : _functions.keys())
        if (childClassHierarchy.startsWith(acceptedClassHierarchy))
            return _functions[acceptedClassHierarchy];
    return Functions(nullptr, nullptr);
}
