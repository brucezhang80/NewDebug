// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import QtQuick 1.0

Rectangle
{
    id: mainRect
    width: 100
    height: 600
    color: "#024008"
    radius: 10
    smooth: false
    border.width: 4
    border.color: "#48d93e"

    signal quit();
    MouseArea
    {
        z: 3
        hoverEnabled: false
        anchors.fill: parent
        acceptedButtons:Qt.RightButton
        onReleased:
        {
            mainRect.quit();
        }
    }
    Rectangle
    {
        id: spinRect
        color:"#024008"
        x: 64
        y: 5
        width: 26
        height: 30
        radius: 2
        anchors.rightMargin: 10
        anchors.right: parent.right
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 5
        smooth: false
        BorderImage
        {
            id: keepTop
            x: 2
            y: 2
            width: 28
            height: 31
            anchors.horizontalCenter: parent.horizontalCenter
            clip: false
            smooth: true
            source: "images/pushpin1.png"
            states:State
            {
                name: "spin"
                PropertyChanges {target: keepTop; source:"images/pushpin2.png"}
            }
            MouseArea
            {
                x: 2
                y: -1
                width: 28
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                z: 2
                hoverEnabled: true
                acceptedButtons:Qt.LeftButton
                onReleased:
                {
                    if( keepTop.state == "spin")
                        keepTop.state = "";
                    else
                        keepTop.state = "spin";
                }
                onEntered:
                {
                    spinRect.color = "#48d93e";
                }
                onExited:
                {
                    spinRect.color = "#024008";
                }
            }
        }
    }
    AppTag
    {
        id:apptag
        width: 100
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        AppView
        {
            id: appview
            width: 100
            anchors.top: parent.top
            anchors.topMargin: 44
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    AppTag
    {
        id:apptag2
        x: 0
        y: 415
        width: 100
        anchors.horizontalCenterOffset: 0
        z: 4
        anchors.top: parent.top
        anchors.topMargin: 415
        anchors.horizontalCenter: parent.horizontalCenter
    }

}
