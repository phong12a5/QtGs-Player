import QtQuick 2.0
import UsbVideoPlayer 1.0
import QtMultimedia 5.0

Item {
    id: root


    property var sources: [
        "test-videos/download.mp4",
        "test-videos/sintel_trailer-480p.webm",
        "test-videos/download.mp4",
        "test-videos/sintel_trailer-480p.webm",
        "test-videos/download.mp4",
        "test-videos/sintel_trailer-480p.webm",
        "test-videos/download.mp4",
        "test-videos/sintel_trailer-480p.webm",
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
            usbVideoPlayer.stop()
            if(currentSource != undefined) {
                usbVideoPlayer.source = "filesrc location=" + currentSource + " ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0"
            }
        }

        UsbVideoPlayer{
            id:usbVideoPlayer
            onFrameHeightChanged: {
                reFitVideoView()
            }

            onFrameWidthChanged: {
                reFitVideoView()
            }

            function reFitVideoView(){
                if(videoView.width / videoView.height > usbVideoPlayer.frameWidth / usbVideoPlayer.frameHeight) {
                    // fit height
                    videoOut.height = videoView.height
                    videoOut.width =  usbVideoPlayer.frameWidth/ usbVideoPlayer.frameHeight * videoOut.height
                } else {
                    // fit width
                    videoOut.width = videoView.width;
                    videoOut.height = usbVideoPlayer.frameHeight/ usbVideoPlayer.frameWidth * videoOut.width
                }
            }
        }

        VideoOutput{
            id:videoOut
            source: usbVideoPlayer
            anchors.centerIn: parent
            fillMode: VideoOutput.Stretch
        }
    }
}
