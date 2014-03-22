import QtQuick 1.1

Rectangle
{
    id:appviewRect
    width: 100
    height: 0

    ListModel
    {
        id: appModel
        ListElement{name: "Music"; portrait: "images/Music.png"}
        ListElement{name: "Internetvalt"; portrait: "images/Internetvalt.png"}
        ListElement{name: "Games"; portrait: "images/Games.png"}
        ListElement{name: "Player"; portrait: "images/Player.png"}
        ListElement{name: "Photoshop"; portrait: "images/Photoshop.png"}
    }

    Component
    {
        id: appDelegate
        Item
        {
            id: item
            width: grid_view.cellWidth; height: grid_view.cellHeight
            Image{id: appIcon; source: portrait; anchors.top: parent.top; anchors.topMargin: -6; anchors.horizontalCenterOffset: 0; anchors.horizontalCenter: parent.horizontalCenter; sourceSize.height: 0; sourceSize.width: 0; fillMode: Image.PreserveAspectFit; }
            Text{color: grid_view.isCurrentItem ? "red" : "#ffffff"; text:name; horizontalAlignment: Text.AlignHCenter; font.family: "微软雅黑"; anchors.topMargin: -10; anchors.top: appIcon.bottom; anchors.horizontalCenter: appIcon.horizontalCenter}

            MouseArea
            {
                hoverEnabled: true
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked:
                {
                    console.log("irregular area clicked "+ name)

                }
                onHoveredChanged:
                {
                    //console.log(name)
                    grid_view.currentIndex = index
                }
            }
        }
    }

    Component
    {
        id: itemHighlight
        Rectangle
        {
            id: wrapper
            width: grid_view.cellWidth
            height: grid_view.cellHeight
            color: GridView.isCurrentItem ? "lightsteelblue" : "#48d93e"
            radius: 5
            x: grid_view.currentItem.x
            y: grid_view.currentItem.y
            Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }
    }

    GridView
    {
        id: grid_view
        width: 80
        height: 324
        clip: true
        smooth: true
        flow: GridView.LeftToRight
        interactive: true
        contentHeight: 100
        boundsBehavior: Flickable.DragAndOvershootBounds
        anchors.top: parent.top
        anchors.topMargin: 0
        highlightMoveDuration: 500
        contentWidth: 20
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        snapMode: GridView.SnapOneRow
        cellHeight: 80
        cellWidth: 80

        model: appModel
        delegate: appDelegate
        highlightFollowsCurrentItem:true
        highlight: itemHighlight
        focus: true
        currentIndex: 0
        keyNavigationWraps: true
        Component.onCompleted: positionViewAtIndex(count-1, GridView.Beginning)
    }

    states:
    [
        State
        {
            name: "hide"
            PropertyChanges{target: grid_view; height: 0;}
        }
    ]

    transitions: Transition
    {
    NumberAnimation { target: grid_view; property: "height"; duration: 400; easing.type: Easing.InOutQuad }
    }
}
