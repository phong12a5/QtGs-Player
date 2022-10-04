import QtQuick 2.0

Item {
    id: root


    property var sources: [
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm",
        "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm"
    ]

    ListView {
        id: listSource
        width: root.width /3
        height: root.height
        model: sources
        orientation: ListView.Vertical
        delegate: Rectangle {
            id: dlg
            width: listSource.width
            height: 100
            border.width: listSource.currentIndex == index? 2 : 0
            border.color: listSource.currentIndex == index? "red" : "transparent"

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

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    VideoPlayer.stopMedia()
                    listSource.currentIndex = index
                }
            }
        }

        Rectangle {
            height: parent.height
            width: 6
            anchors.right: parent.right
            color: "grey"
            radius: 3
        }
    }

    Item {
        id: videoView
        width: root.width - listSource.width
        height: root.height
        anchors.right: root.right

        property var currentSource: sources[listSource.currentIndex]
        property bool started: false
        onCurrentSourceChanged: {
            started = false
            console.log("onCurrentSourceChanged: " + currentSource)
        }

        Rectangle {
            id: playBtn
            width: 100
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            color: "grey"
            border.width: 1
            border.color: playBtnMouse.pressed? "blue" : "black"

            property bool running: false

            Text {
                id:lable
                text: parent.running? qsTr("Pause") : qsTr("Play")
                anchors.centerIn: parent
            }

            MouseArea {
                id: playBtnMouse
                anchors.fill: parent
                onClicked: {
                    if(!videoView.started) {
                        VideoPlayer.playMedia(videoView.currentSource)
                        videoView.started = true
                    } else {
                        VideoPlayer.playPause();
                    }

                    parent.running = !parent.running
                }
            }
        }
    }
}
