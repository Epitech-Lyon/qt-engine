/*
** EPITECH PROJECT, 2019
** forward_coding_journey
** File description:
** NodePainterDelegate
*/

#pragma once

#include <QPainter>

#include "NodeGeometry.hpp"
#include "NodeDataModel.hpp"

namespace QtNodes {
	/// Class to allow for custom painting
	class NodePainterDelegate {
	public:
		virtual ~NodePainterDelegate() = default;
		virtual void paint(QPainter *painter, NodeGeometry const &geom, NodeDataModel const *model) = 0;
	};
}
