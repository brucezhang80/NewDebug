import QtQuick 1.1

Rectangle
{
    id: tagRect
    width: 100
    height: 0

    BorderImage
    {
        id: tag_image
        x: 5
        y: 2
        width: 80
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        z: 2
        anchors.topMargin: 0
        clip: false
        smooth: true
        scale: 1
        anchors.top: parent.top
        opacity: 1
        source: "images/tag.png"

        Text
        {
            id: tag_text
            x: 15
            y: 11
            width: 50
            height: 26
            color: "#138f17"
            text: "A Tag"
            z: 2
            anchors.horizontalCenterOffset: 0
            anchors.top: parent.top
            anchors.topMargin: 11
            anchors.horizontalCenter: parent.horizontalCenter
            font.strikeout: false
            font.underline: false
            font.italic: false
            font.bold: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }

        MouseArea
        {
            id: mouse_area1
            x: 7
            y: 11
            width: 58
            height: 26
            anchors.top: parent.top
            anchors.topMargin: 11
            onClicked:
            {
                if( appview.state == "hide")
                {
                    appview.state = "";
                    apptag2.anchors.topMargin = 415;
                }
                else
                {
                    appview.state = "hide";
                    apptag2.anchors.topMargin = tagRect.anchors.topMargin+30;
                }
            }
        }
    }


}
