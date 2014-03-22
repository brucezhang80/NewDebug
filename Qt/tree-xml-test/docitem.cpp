#include "docitem.h"
#include <QVariant>
#include <QStringList>

#include <QDebug>


DocItem::DocItem(QDomNode &node, int row, DocItem *parent)
    :m_domNode(node), m_rowNumber(row), m_parentItem(parent)
{

}

DocItem::~DocItem()
{
    QHash<int,DocItem*>::iterator it;
    for (it = m_childItems.begin(); it != m_childItems.end(); ++it)
        delete it.value();
}

DocItem *DocItem::child(int row)
{
    if (m_childItems.contains(row))
        return m_childItems[row];

    if( !m_domNode.hasChildNodes() )
    {
        return 0;
    }

    if (row >= 0 && row < m_domNode.childNodes().count())
    {
        QDomNode childNode = m_domNode.childNodes().item(row);
        if( childNode.nodeType() == QDomNode::ProcessingInstructionNode )
            return 0;
//        qDebug() << childNode.nodeName();
       // if( childNode.nodeName() != "data" )
        {
            DocItem *childItem = new DocItem(childNode, row, this);
            m_childItems[row] = childItem;
            return childItem;
        }
    }

    return 0;
}

DocItem *DocItem::parent()
{

    return m_parentItem;
}

QDomNode DocItem::node() const
{
    return m_domNode;
}

int DocItem::row() const
{
    return m_rowNumber;
}

QVariant DocItem::data(int column)
{
    if( node().nodeType() == QDomNode::TextNode )
    {
        QStringList valueList = node().nodeValue().split("|");
//        qDebug() << valueList.at(0)  << node().nodeValue();
        return  valueList.at(0);
    }

    QDomNamedNodeMap attributeMap = node().attributes();
    QDomNode node_attr =  attributeMap.item(column);
    if( node_attr.isAttr())
    {
        return  node_attr.nodeValue();
    }

    if( m_domNode.isElement() )
    {
        QDomElement e = m_domNode.toElement();
//        qDebug() << e.tagName() << e.attribute("name");

        QDomNodeList list = e.childNodes();
        for(int i=0; i<list.count(); i++)
        {
            QDomNode node = list.at(i);
//            if(node.isElement())
//            qDebug() << m_domNode.toElement().tagName()
//                     << m_domNode.toElement().text();
        }
    }

    return QVariant();
}

