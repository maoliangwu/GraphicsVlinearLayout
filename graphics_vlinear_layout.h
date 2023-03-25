
#ifndef GRAPHICS_VLINEAT_LAYOUT_H
#define GRAPHICS_VLINEAT_LAYOUT_H
#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>

class GraphicsVLinearLayout :public QGraphicsLayout
{
public:
	GraphicsVLinearLayout(QGraphicsLayoutItem* parent = nullptr);
	~GraphicsVLinearLayout();
	void Setexited(bool exited); virtual int count() const override;
	virtual void removeAt(int index)override;//不会删除元素
	virtual QGraphicsLayoutItem* itemAt(int index) const override;
	virtual void setGeometry(const QRectF& rect)override;
	void addItem(QGraphicsLayoutItem* item);
	void insertItem(int index, QGraphicsLayoutItem* item);
	void removeItem(QGraphicsLayoutItem* item);
	void removeItemWidthDestroy(QGraphicsLayoutItem* item);
	void removeItemsFlashback(int start, int count, bool del = true);
	void setSpacing(int spacing);
	void RemoveAllItemsWithoutDestroy();
	void RemoveAllItemsWithDestroy();
private:
	QSizeF minSize(const QSizeF& constraint) const;
	QSizeF prefSize() const;
	QSizeF maxSize() const;
	qreal spacing(Qt::Orientation o) const;
	qreal doLayout(const QRectF& geom, bool applyNewGeometry) const;
protected:
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;

private:
	bool exited{ false };
	QList<QGraphicsLayoutItem*> itemList;
	qreal m_spacing[2] = { 0, 0 };
};

#endif