/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Main
*/

#include "Generator.hpp"

int main(int, char **av)
{
	return engineGenerator::Generator::generate(av[1], av[2]);
}
