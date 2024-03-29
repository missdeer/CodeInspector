
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

ToolButton {
    id: favMenuButton
    Layout.alignment: Qt.AlignHCenter
    focusPolicy: Qt.NoFocus
    implicitHeight: 56
    implicitWidth: 56
    Column {
        spacing: 0
        topPadding: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 24
            width: 24
            Image {
                height: 24
                width: 24
                verticalAlignment: Image.AlignTop
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/resource/image/"+iconFolder+"/menu.png"
            }
        }
    }
    onClicked: {
        navigationBar.open()
        // see QTBUG-59293
        favMenuBugfix = !favMenuBugfix
    }
}
