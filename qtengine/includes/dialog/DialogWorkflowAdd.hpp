/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogWorkflowAdd
*/

#pragma once

#include "DialogBase.hpp"
#include "Types.hpp"

namespace qtengine {
	class DialogWorkflowAdd : public DialogBase {
		Q_OBJECT

	public:
		DialogWorkflowAdd(QWidget *parent = nullptr);
		~DialogWorkflowAdd() = default;

		Types::Access access() const { return _access; }
		Types::Type type() const { return _type; }
		QString name() const { return _name; }

	private:
		QWidget *initBody();
		Types::Access _access;
		Types::Type _type;
		QString _name;
	};
}
