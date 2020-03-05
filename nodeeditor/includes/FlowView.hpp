#pragma once

#include <QtWidgets/QGraphicsView>

namespace QtNodes {
	class FlowScene;
	class FlowView : public QGraphicsView {
		Q_OBJECT

	public:
		FlowView(QWidget *parent = Q_NULLPTR);
		FlowView(FlowScene *scene, QWidget *parent = Q_NULLPTR);

		FlowView(const FlowView &) = delete;
		FlowView operator=(const FlowView&) = delete;

		virtual ~FlowView() = default;

		QAction *clearSelectionAction() const;
		QAction *deleteSelectionAction() const;

		void setScene(FlowScene *scene);
		void openMenu(const QPoint &pos);

	public Q_SLOTS:
		void scaleUp();
		void scaleDown();

		void deleteSelectedNodes();

	protected:
		void contextMenuEvent(QContextMenuEvent *event) override;

		void wheelEvent(QWheelEvent *event) override;

		void keyPressEvent(QKeyEvent *event) override;
		void keyReleaseEvent(QKeyEvent *event) override;

		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;

		void drawBackground(QPainter* painter, const QRectF &r) override;
		void showEvent(QShowEvent *event) override;

		FlowScene *scene();

	private:
		QAction *_clearSelectionAction;
		QAction *_deleteSelectionAction;
		QPointF _clickPos;
		FlowScene *_scene;
	};
}
