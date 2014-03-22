import QtQuick 1.1

/*
Rectangle
{
    id: appWindow
    width: 300
    height: 450

    ListModel
    {
        id: myModel

        ListElement{ number: "1" }
        ListElement{ number: "2" }
        ListElement{ number: "3" }
        ListElement{ number: "4" }
        ListElement{ number: "5" }
        ListElement{ number: "6" }
        ListElement{ number: "7" }
        ListElement{ number: "8" }
    }

    Component
    {
        id: myDelegate
        Item
        {
            id: item
            width: myList.width; height: 40
            Row
            {
                Text{ id: text; text: number; font.pixelSize: item.height}
            }

            MouseArea
            {
                id: dragArea
                anchors.fill: parent
                property int positionStarted: 0
                property int positionEnded: 0

                drag.axis: Drag.YAxis

                onPressed:
                {
                    myList.currentIndex = index

                    item.z = 2
                    positionStarted = item.y
                    dragArea.drag.target = item
                    item.opacity = 0.5

                    drag.maximumY = (myList.height - item.height - 1),
                    drag.minimumY = 0
                }
                onPositionChanged:
                {
                    positionEnded = item.y;
                    //console.debug(myList.currentIndex + "  " + myList.indexAt(mouseX, mouseY))
                }

                onReleased:
                {
                    item.opacity = 1;
                    myList.currentIndex = index
                    item.y = positionStarted
//                    item.z = 1,
//                    myModel.move(index,0,1),
//                    myList.interactive = true
                    dragArea.drag.target = null
                }
            }
        }
    }

    ListView
    {
        id: myList
        snapMode: ListView.SnapToItem
        interactive: false
        anchors.fill: parent
        highlight: Rectangle{ color: "lightblue";}
        model: myModel

        delegate: myDelegate
    }
}
*/

/*
Rectangle
{
    id: rectangle1
    width: 500;  height: 480
    color: "#222222"
    ListModel
    {
        id: dndModel

        ListElement{ title: "GiftAdorn011001" ; icon:"images/GiftAdorn011001.jpg";gridId: 0}
        ListElement{ title: "GiftAdorn011002" ; icon:"images/GiftAdorn011002.jpg";gridId: 1}
        ListElement{ title: "GiftAdorn011003" ; icon:"images/GiftAdorn011003.jpg";gridId: 2}
        ListElement{ title: "GiftAdorn011005" ; icon:"images/GiftAdorn011005.jpg";gridId: 3}
        ListElement{ title: "GiftAdorn011006" ; icon:"images/GiftAdorn011006.jpg";gridId: 4}
        ListElement{ title: "GiftAdorn011008" ; icon:"images/GiftAdorn011008.jpg";gridId: 5}
        ListElement{ title: "GiftAdorn011009" ; icon:"images/GiftAdorn011009.jpg";gridId: 6}
        ListElement{ title: "GiftAdorn011020" ; icon:"images/GiftAdorn011020.jpg";gridId: 7}
    }

    Component
    {
        id: iconItem
        Item {
            id: main
            width: grid.cellWidth; height: grid.cellHeight
            Row
            {
            Image {
                id: item; parent: loc
                x: main.x + 5; y: main.y + 5
                width: main.width - 10; height: main.height - 10;
                fillMode: Image.PreserveAspectFit; smooth: true
                source: icon
                Rectangle {
                    anchors.fill: parent;
                    border.color: "#326487"; border.width: 6
                    color: "transparent"; radius: 5
                    visible: item.state == "active"
                }
                Behavior on x { enabled: item.state != "active"; NumberAnimation { duration: 400; easing.type: Easing.OutBack } }
                Behavior on y { enabled: item.state != "active"; NumberAnimation { duration: 400; easing.type: Easing.OutBack } }
                SequentialAnimation on rotation {
                    NumberAnimation { to:  2; duration: 60 }
                    NumberAnimation { to: -2; duration: 120 }
                    NumberAnimation { to:  0; duration: 60 }
                    running: loc.currentId != -1 && item.state != "active"
                    loops: Animation.Infinite; alwaysRunToEnd: true
                }
                states: State {
                    name: "active"; when: loc.currentId == gridId
                    PropertyChanges { target: item; x: loc.mouseX - width/2; y: loc.mouseY - height/2; scale: 0.5; z: 10 }
                }
                transitions: Transition { NumberAnimation { property: "scale"; duration: 200} }
            }
            }
        }
    }

    GridView
    {
            id: grid
            anchors.fill: parent
            keyNavigationWraps: true
            highlightFollowsCurrentItem: false
            contentHeight: 400
            interactive: false
//            cellWidth: 120; cellHeight: 120;
            model: dndModel
            delegate: iconItem


            MouseArea
            {
                anchors.fill: parent
                property int currentId: -1                       // Original position in model
                property int newIndex: 0                            // Current Position in model

                property int index: grid.indexAt(mouseX, mouseY) // Item underneath cursor

                id: loc
                onPressed:
                {
                    index = grid.indexAt(mouseX, mouseY)
                    console.debug(index)
                    if( index != -1)
                        currentId = dndModel.get(index).gridId
                }
                onReleased:
                {
                    currentId = -1
                    //index = -1;
                }
                onMousePositionChanged:
                {
                    if (loc.currentId != -1 && index != -1 && index != newIndex )
                    {
                        console.debug( "from: " + newIndex +", to:"+ index + "  " + loc.currentId)
                        dndModel.move(newIndex, newIndex = index, 1)
                    }
                }
            }
        }
}
*/

Rectangle
{
    id: rectangle1
    width: 300;  height: 400
    color: "#5c5a5a"
    ListModel
    {
        id: dndModel

        ListElement{ number: 0; title: "GiftAdorn011001" ; icon:"images/GiftAdorn011001.jpg";}
        ListElement{ number: 1; title: "GiftAdorn011002" ; icon:"images/GiftAdorn011002.jpg";}
        ListElement{ number: 2; title: "GiftAdorn011003" ; icon:"images/GiftAdorn011003.jpg";}
        ListElement{ number: 3; title: "GiftAdorn011005" ; icon:"images/GiftAdorn011005.jpg";}
        ListElement{ number: 4; title: "GiftAdorn011006" ; icon:"images/GiftAdorn011006.jpg";}
        ListElement{ number: 5; title: "GiftAdorn011008" ; icon:"images/GiftAdorn011008.jpg";}
        ListElement{ number: 6; title: "GiftAdorn011009" ; icon:"images/GiftAdorn011009.jpg";}
        ListElement{ number: 7; title: "GiftAdorn011020" ; icon:"images/GiftAdorn011020.jpg";}
    }

    Component
    {
        id: dndDelegate

        Item
        {
            id: rowItem; width: rowItem.ListView.view.width; height: 68; anchors.margins: 20; z:1;
            Item
            {
                id:item; z:1
                Rectangle{color: "black"; opacity: index % 2 ? 0.2 : 0.4; height: 68; width: rowItem.width; y:1}
                Rectangle
                {
                    x: 4; y: 4; width: 61; height: 61; color: "white"; smooth: true
                    Image{id: dndImage; source: icon; width:58; height: 58; x:2; y:2}
                }
                Column
                {
                    x: 72; width: rowItem.ListView.view.width - 95; y: 15; spacing: 2
                    Text{ id:itemTitle; text: title; font.bold: true;color: "#fbf2f2"; }
                    Text{ id:itemId; text: number; color: "#26e6ec";}
                }
            }
        }// Item
    }

    Component
    {
        id: itemHighlight
        Rectangle
        {
            id: wrapper
            width: dndList.width
//            height: dndList.currentItem.height
            color: GridView.isCurrentItem ? "lightblue" : "#48d93e"
            radius: 5
//            x: dndList.currentItem.x
//            y: dndList.currentItem.y
            Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }
    }

    ListView
    {
        id: dndList
        height: 600
        width:300
        anchors.fill: parent
        anchors.margins: 10
        anchors.topMargin: 10
        anchors.bottomMargin: 44
        y:10

        model: dndModel
        delegate:dndDelegate

        highlight:itemHighlight
        currentIndex: -1
        property int newIndex: -1

        MouseArea
        {
            id: coords

            height:600
            width: dndList.width
            //            anchors.left: parent.left
            //            anchors.right: parent.right
            //            anchors.top: parent.top
            //                    anchors.bottom: parent.bottom

            hoverEnabled: true

            acceptedButtons: Qt.LeftButton
            drag.axis: Drag.YAxis

            property int mX: (mouseX < 0) ? 0 : ((mouseX < dndList.width - dndList.width) ? mouseX : dndList.width - dndList.width)
            property int mY: (mouseY < 0) ? 0 : ((mouseY < dndList.height - 68) ? mouseY : dndList.height - 68)
            property int index: parseInt(mX/dndList.width) + parseInt(mY/68)  //item underneath cursor

            //            property int index: dndList.indexAt(mouseX, mouseY)
            property int activeIndex:-1
            property bool isDrag: false

            onPressed:
            {
                console.debug(dndList.height)
                isDrag = false;
                if( index !== -1 )
                {
                    dndList.currentIndex = index;
                    dndList.currentItem.z = 10;
                    coords.drag.target = dndList.currentItem;
                    activeIndex = index;
                }
            }
            onReleased:
            {
                if( index !== -1 && isDrag === true)
                {
                    if( index === 0)
                    {
                        dndList.currentItem.y = 0;//dndModel.get(index+1).y+68;
                        console.debug(index)
                    }
                    else
                        dndList.currentItem.y = parseInt(index)*68;
                    dndList.currentItem.x = 0;
                    dndList.currentItem.z = 1
                    dndList.currentIndex = -1
                    coords.drag.target = null
                    isDrag = false;
                    dndList.interactive = true;
                }
                else
                {
                    dndList.currentItem.y = parseInt(index)*68;
                    //                    dndList.currentItem.x = 0;
                    dndList.currentItem.z = 1
                    dndList.currentIndex = -1
                    coords.drag.target = null
                    isDrag = false;
                    dndList.interactive = true;
                }

            }
            onPositionChanged:
            {
                if (drag.active && index !== -1 && index !== activeIndex)
                {
                    isDrag = true;
                    console.debug(index+ ", "+activeIndex)
                    dndList.interactive = false;
                    dndModel.move(activeIndex, activeIndex = index, 1);
                }
            }
        }
    }

}
