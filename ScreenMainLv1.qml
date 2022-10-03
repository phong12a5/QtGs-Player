import QtQuick 2.0

ListView {
    id: root

    property var selectedSource: undefined

    anchors.fill: root
    orientation: ListView.Horizontal
    delegate: Rectangle {
        id: dlg
        width: root.width / root.model.length
        height: root.height
        border.width: 2
        color: "#B0C4DE"
        border.color: "grey"

        Text {
            anchors.centerIn: parent
            text: modelData.text
        }

        MouseArea {
            anchors.fill: parent


            onClicked: {
                selectedAnimation.start()
            }

        }

        PropertyAnimation {
            id: selectedAnimation
            target: dlg
            easing.type: Easing.InCubic
            property: "color"
            from: "#66ff66"
            to: "#B0C4DE"
            duration: 500
            onStopped: {
                selectedSource = modelData
            }
        }
    }
}
