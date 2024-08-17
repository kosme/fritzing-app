/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2019 Fritzing

Fritzing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fritzing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fritzing.  If not, see <http://www.gnu.org/licenses/>.

********************************************************************/


#include <QApplication>
#include <QMouseEvent>
#include <QMenu>
#include <QStylePainter>
#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QStyleOptionTabV2>
#else
#include <QStyleOptionTab>
#endif
#include <QMimeData>

#include "stacktabbar.h"
#include "stacktabwidget.h"
#include "../partsbinpalettewidget.h"
#include "../partsbinview.h"
#include "../../debugdialog.h"


StackTabBar::StackTabBar(StackTabWidget *parent) : QTabBar(parent) {
	setAcceptDrops(true);
	this->setUsesScrollButtons(true);
	//this->setContentsMargins(0,0,0,0);
	//this->setTabsClosable(true);
	setMovable(true);
	m_parent = parent;
	setProperty("current","false");
	setExpanding(false);
	setElideMode(Qt::ElideRight);
	setIconSize(QSize(32, 32));

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));

	m_dragMoveTimer.setSingleShot(true);
	m_dragMoveTimer.setInterval(250);
	connect(&m_dragMoveTimer, SIGNAL(timeout()), this, SLOT(setIndex()));
}

bool StackTabBar::mimeIsAction(const QMimeData* m, const QString& action) {
	if(m != nullptr) {
		QStringList formats = m->formats();
		return formats.contains("action") && (m->data("action") == action);
	} else {
		return false;
	}
}

void StackTabBar::dragEnterEvent(QDragEnterEvent* event) {
	// Only accept if it's an part-reordering request
	const QMimeData *m = event->mimeData();
	if (mimeIsAction(m, "part-reordering")) {
		event->acceptProposedAction();
	}
}

void StackTabBar::dragLeaveEvent(QDragLeaveEvent *event) {
	Q_UNUSED(event);
	m_dragMoveTimer.stop();
}

void StackTabBar::dragMoveEvent(QDragMoveEvent* event) {
	const QMimeData *m = event->mimeData();
	int index = tabAt(event->position().toPoint());
	if ((event->source() != this) && mimeIsAction(m,"part-reordering")) {
		auto* bin = qobject_cast<PartsBinPaletteWidget*>(m_parent->widget(index));
		if((bin != nullptr) && bin->allowsChanges()) {
			event->acceptProposedAction();
			m_dragMoveTimer.setProperty("index", index);
			if (!m_dragMoveTimer.isActive()) {
				DebugDialog::debug("starting drag move timer");
				m_dragMoveTimer.start();
			}
			//DebugDialog::debug(QString("setting index %1").arg(index));
		}
	}
}

void StackTabBar::dropEvent(QDropEvent* event) {
	int toIndex = tabAt(event->position().toPoint());

	const QMimeData *m = event->mimeData();
	if(mimeIsAction(m, "part-reordering")) {
		auto* bin = qobject_cast<PartsBinPaletteWidget*>(m_parent->widget(toIndex));
		if((bin != nullptr) && bin->allowsChanges()) {
			bin->currentView()->dropEventAux(event,true);
		}
	}

	event->acceptProposedAction();
}

void StackTabBar::showContextMenu(const QPoint &point)
{
	if (point.isNull()) return;

	int tabIndex = this->tabAt(point);
	auto* bin = qobject_cast<PartsBinPaletteWidget*>(m_parent->widget(tabIndex));
	if (bin == nullptr) return;

	setCurrentIndex(tabIndex);

	QMenu * contextMenu = bin->binContextMenu();
	if (contextMenu != nullptr) {
		contextMenu->exec(this->mapToGlobal(point));
	}
	delete contextMenu;
}


void StackTabBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QStylePainter painter(this);

	for(int i = 0; i < this->count(); ++i)
	{
		QStyleOptionTab option;
		initStyleOption(&option, i);
		option.shape = RoundedNorth;
		option.text = "";
		painter.drawControl(QStyle::CE_TabBarTab, option);
	}
}

void StackTabBar::setIndex() {
	if (sender() == nullptr) return;

	bool ok = false;
	int index = sender()->property("index").toInt(&ok);
	if (!ok) return;

	setCurrentIndex(index);
}
