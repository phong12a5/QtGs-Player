import QtQuick 2.0

Item {
    id: root


    property var sources: [
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm"
    ]

    ListView {
        id: listSource
        width: root.width /3
        height: root.height
        model: sources
        orientation: ListView.Vertical
        delegate: Item{
            id: dlg
            width: listSource.width
            height: 100

            Text {
                id: txt
                text:modelData
                anchors.centerIn: parent
                wrapMode: Text.WordWrap
                width: parent.width
            }

            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: "#00e600"
            }
        }

        Rectangle {
            height: parent.height
            width: 2
            anchors.right: parent.right
            color: "grey"
            opacity: 0.5
        }
    }
}
