import QtQuick 2.0

Item {
    property var selectedQmlSource: undefined

    Loader {
        anchors.fill: parent
        source: selectedQmlSource == undefined? "" : selectedQmlSource
    }
}
