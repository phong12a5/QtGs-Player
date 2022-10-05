import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: root
    visible: true
    width: 800
    height: 480
    title: qsTr("Hello World")

    property int screenLevel: 0
    property var sourceModel: [
        {"text": "USB Videos", "qml": "qrc:/USBVideos.qml"},
        {"text": "Streamming Videos", "qml": "qrc:/StreamingVideo.qml"},
        {"text": "Bluetooth Audios", "qml": ""},
        {"text": "Zing Musics", "qml": ""},
        {"text": "Something Videos", "qml": ""}
    ]

    ScreenMainLv1 {
        id: mainLv1
        width: root.width
        height: root.height
        model: sourceModel
        visible: mainLv2.selectedQmlSource == undefined
        onSelectedSourceChanged: {
            mainLv2.selectedQmlSource = selectedSource.qml
        }
    }

    ScreenMainLv2 {
        id: mainLv2
        width: root.width
        height: root.height
        onSelectedQmlSourceChanged: {
            console.log("onSelectedQmlSourceChanged: " + selectedQmlSource)
        }
    }
}
