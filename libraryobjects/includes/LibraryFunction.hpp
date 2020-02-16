/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryFunction
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <functional>

namespace libraryObjects {
	class AObject;
	class LibraryObject;

	class LibraryFunction {
	public:
		LibraryFunction() = default;
		~LibraryFunction() = default;

	// Drag functions
	public:
		typedef std::function<bool (AObject *, int, AObject *)> FunctionDragAdd;
		typedef std::function<bool (AObject *, AObject *)> FunctionDragRemove;
		typedef struct FunctionDrag {
			FunctionDrag() : isValid(false) {}
			FunctionDrag(const QString &functionAddName, FunctionDragAdd functionAdd, const QString &functionRemoveName, FunctionDragRemove functionRemove)
				: isValid(true), functionAddName(functionAddName), functionAdd(functionAdd), functionRemoveName(functionRemoveName), functionRemove(functionRemove) {}
			FunctionDrag(const FunctionDrag &other)
				: isValid(true), functionAddName(other.functionAddName), functionAdd(other.functionAdd), functionRemoveName(other.functionRemoveName), functionRemove(other.functionRemove) {}
			FunctionDrag &operator=(const FunctionDrag &other)
			{
				isValid = other.isValid;
				functionAddName = other.functionAddName;
				functionAdd = other.functionAdd;
				functionRemoveName = other.functionRemoveName;
				functionRemove = other.functionRemove;
				return *this;
			}

			bool isValid;
			QString functionAddName;
			FunctionDragAdd functionAdd;
			QString functionRemoveName;
			FunctionDragRemove functionRemove;
		} FunctionDrag;

		void addFunctionDrag(const QString &acceptedFor, const FunctionDrag &functionDrag) { _functionsDrag[acceptedFor] = functionDrag; }
		FunctionDrag functionDragFor(const QString &classHierarchy) const;

	private:
		QMap<QString, FunctionDrag> _functionsDrag;

	// Menu functions
	public:
		typedef std::function<void (AObject *)> FunctionMenuSignature;
		typedef struct FunctionMenu {
			FunctionMenu() : isValid(false) {}
			FunctionMenu(const QString &functionName, FunctionMenuSignature function)
				: isValid(true), functionName(functionName), function(function) {}
			FunctionMenu(const FunctionMenu &other)
				: isValid(true), functionName(other.functionName), function(other.function) {}
			FunctionMenu &operator=(const FunctionMenu &other)
			{
				isValid = other.isValid;
				functionName = other.functionName;
				function = other.function;
				return *this;
			}

			bool isValid;
			QString functionName;
			FunctionMenuSignature function;
		} FunctionMenu;
		
		void addFunctionMenu(const FunctionMenu &functionMenu) { _functionsMenu.push_back(functionMenu); }
		QList<FunctionMenu> functionsMenu() const { return _functionsMenu; }

	private:
		QList<FunctionMenu> _functionsMenu;
	};
}
