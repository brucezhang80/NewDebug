#ifndef DOCITEM_H
#define DOCITEM_H

#include <QDomNode>
#include <QHash>

class DocItem
{
public:
    DocItem(QDomNode &node, int row, DocItem *parent = 0);
    ~DocItem();

    DocItem *child(int row);
    DocItem *parent();
    QDomNode node() const;
    int row() const;
    QVariant data(int column);

private:
    QDomNode            m_domNode;
    QHash<int,DocItem*> m_childItems;
    DocItem            *m_parentItem;
    int                 m_rowNumber;
};

#endif // DOCITEM_H
