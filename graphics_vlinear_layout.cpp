#include "graphics_vlinear_layout.h"
#include <QGraphicsWidget>
GraphicsVLinearLayout::GraphicsVLinearLayout(QGraphicsLayoutItem* parent /* = nullptr */) :QGraphicsLayout(parent)
{

}

void GraphicsVLinearLayout::Setexited(bool exit)
{
	exited = exit;
}


GraphicsVLinearLayout::~GraphicsVLinearLayout()
{
	RemoveAllItemsWithDestroy();
}

void GraphicsVLinearLayout::RemoveAllItemsWithDestroy()
{
	auto items = itemList;

	RemoveAllItemsWithoutDestroy();
	foreach(auto item, items)
	{
		delete item;
		item = nullptr;
	}
	items.clear();
	updateGeometry();
	invalidate();
}

void GraphicsVLinearLayout::RemoveAllItemsWithoutDestroy()
{
	foreach(auto item, itemList)
	{
		if (item->ownedByLayout())
		{
			item->setParentLayoutItem(nullptr);
		}
	}
	itemList.clear();
	updateGeometry();
	invalidate();
}

void GraphicsVLinearLayout::removeItemWidthDestroy(QGraphicsLayoutItem* item)
{
	item->setParentLayoutItem(nullptr);
	itemList.removeOne(item);
	delete item;
	invalidate();
	updateGeometry();
}

void GraphicsVLinearLayout::removeItem(QGraphicsLayoutItem* item)
{
	item->setParentLayoutItem(nullptr);
	itemList.removeOne(item);
	invalidate();
	updateGeometry();
}

void GraphicsVLinearLayout::insertItem(int index, QGraphicsLayoutItem* layoutItem)
{
	itemList.indexOf(layoutItem);
	removeItem(layoutItem);
	itemList.insert(index, layoutItem);
	if (layoutItem->isLayout())
	{
	}
	else
	{
		if (QGraphicsItem* item = layoutItem->graphicsItem()) {
			QGraphicsItem* newParent = dynamic_cast<QGraphicsWidget*>(parentLayoutItem());
			QGraphicsItem* oldParent = item->parentItem();
			if (newParent && oldParent != newParent)
				item->setParentItem(newParent);
		}
	}
	updateGeometry();
	invalidate();
}

void GraphicsVLinearLayout::addItem(QGraphicsLayoutItem* layoutItem)
{
	itemList.append(layoutItem);
	if (layoutItem->isLayout())
	{
	}
	else
	{
		if (QGraphicsItem* item = layoutItem->graphicsItem()) {
			QGraphicsItem* newParent = dynamic_cast<QGraphicsWidget*>(parentLayoutItem());
			QGraphicsItem* oldParent = item->parentItem();
			if (newParent && oldParent != newParent)
				item->setParentItem(newParent);
		}
	}
	updateGeometry();
	invalidate();
}

int GraphicsVLinearLayout::count() const
{
	return itemList.size();
}

void GraphicsVLinearLayout::removeItemsFlashback(int start, int count, bool del)
{
	auto cnt = itemList.size();
	if (start < 0 || start >= cnt || count + start > cnt)
	{
		return;
	}
	QSet< QGraphicsLayoutItem*> items;
	int index = count + start - 1;
	for (; index >= 0; index--)
	{
		auto item = itemList.at(index);
		item->setParentLayoutItem(nullptr);
		items.insert(item);
		itemList.removeAt(index);
	}
	if (del)
	{
		for (auto item : items)
		{
			delete item;
		}
	}
	invalidate();
	updateGeometry();
}

void GraphicsVLinearLayout::removeAt(int index)
{
	if (index >= 0 && index < itemList.size())
	{
		auto item = itemList.at(index);
		itemList.removeOne(item);
		invalidate();
		updateGeometry();
	}
}

QGraphicsLayoutItem* GraphicsVLinearLayout::itemAt(int index) const
{
	return (index >= 0 && index < itemList.size()) ? itemList.at(index) : nullptr;
}

void GraphicsVLinearLayout::setSpacing(int spacing)
{
	m_spacing[1] = spacing;
}

qreal GraphicsVLinearLayout::doLayout(const QRectF& geom, bool applyNewGeometry) const
{
	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	const qreal maxw = geom.width() - left - right;

	qreal y = 0;
	QSizeF pref;
	for (QGraphicsLayoutItem* item : itemList) {
		pref = item->effectiveSizeHint(Qt::PreferredSize);

		qreal next_x = 0;
		if (applyNewGeometry)
			item->setGeometry(QRectF(QPointF(left + next_x, top + y), pref));
		y += pref.height() + spacing(Qt::Vertical);
	}
	return top + y + bottom;
}

qreal GraphicsVLinearLayout::spacing(Qt::Orientation o) const
{
	return m_spacing[int(o) - 1];
}
QSizeF GraphicsVLinearLayout::minSize(const QSizeF& constraint) const
{
	QSizeF size(0, 0);
	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	if (constraint.width() >= 0) {   // height for width
		const qreal height = doLayout(QRectF(QPointF(0, 0), constraint), false);
		size = QSizeF(constraint.width(), height);
	}
	else if (constraint.height() >= 0) {  // width for height?
	 // not supported
	}
	else {
		for (const QGraphicsLayoutItem* item : qAsConst(itemList))
			size = size.expandedTo(item->effectiveSizeHint(Qt::MinimumSize));
		size += QSizeF(left + right, top + bottom);
	}
	return size;
}

QSizeF GraphicsVLinearLayout::prefSize() const
{
	qreal left, right, top, bottom;
	getContentsMargins(&left, &top, &right, &bottom);

	qreal total_width = 0;
	qreal total_height = 0;
	for (const QGraphicsLayoutItem* item : qAsConst(itemList)) {
		QSizeF pref = item->effectiveSizeHint(Qt::PreferredSize);
		total_height += pref.height();
		total_width = qMax(pref.width(), total_width);
	}
	return QSizeF(total_width + left + right, total_height + top + bottom);
}

QSizeF GraphicsVLinearLayout::maxSize() const
{
	qreal maxWidth = 0;
	qreal totalHeight = 0;
	for (const QGraphicsLayoutItem* item : qAsConst(itemList)) {
		QSizeF pref = item->effectiveSizeHint(Qt::PreferredSize);
		maxWidth = qMax(maxWidth, pref.width());
		if (totalHeight > 0)
			totalHeight += spacing(Qt::Vertical);
		totalHeight += pref.height();
	}

	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	return QSizeF(left + maxWidth + right, top + totalHeight + bottom);
}
QSizeF GraphicsVLinearLayout::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
	QSizeF sh = constraint;
	switch (which) {
	case Qt::PreferredSize:
		sh = prefSize();
		break;
	case Qt::MinimumSize:
		sh = minSize(constraint);
		break;
	case Qt::MaximumSize:
		sh = maxSize();
		break;
	default:
		break;
	}
	return sh;
}

void GraphicsVLinearLayout::setGeometry(const QRectF& rect)
{
	QGraphicsLayout::setGeometry(rect);
	doLayout(rect, true);
}