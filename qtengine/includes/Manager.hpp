/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Manager
*/

#pragma once

namespace qtengine {
	class Manager {
	public:
		~Manager();
		static Manager *instance();

	private:
		Manager();
	};
}
