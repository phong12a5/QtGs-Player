import QtQuick 2.0

Item {
    property var selectedQmlSource: undefined

    Loader {
        anchors.fill: parent
        source: selectedQmlSource == undefined? "" : selectedQmlSource
    }

    Rectangle {
        width: 60
        height: 30
        anchors.top: parent.top; anchors.topMargin: 5
        anchors.right: parent.right; anchors.rightMargin: 5
        color: "transparent"
        visible: selectedQmlSource != undefined
        border.width: 1
        border.color: mouseArea.pressed? "red" : "grey"

        Text {
            text: qsTr("Back")
            anchors.centerIn: parent
            color: "Black"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                selectedQmlSource = undefined
            }
        }
    }
}
