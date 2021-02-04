import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12

import cxl.custom 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle{
        anchors.fill: parent
        QMLPlayer{
            anchors.fill: parent
            Component.onCompleted: start()
            Timer{
                repeat: true
                interval: 10
                running: true
                onTriggered: {
                    parent.update()
                }
            }
        }
    }
}
