import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../common"

ToolBar {
    id: myTitleBar
    property alias text: titleLabel.text

    RowLayout {
        focus: false
        spacing: 6
        anchors.fill: parent
        Item {
            width: 4
        }

        ToolButton {
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/resource/image/" + iconOnPrimaryFolder + "/menu.png"
            }
            onClicked: {
                navigationBar.open()
            }
        }
        LabelTitle {
            id: titleLabel
            text: qsTr("CodeInspector")
            leftPadding: 6
            rightPadding: 6
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: textOnPrimary
        }
        ToolButton {
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/resource/image/" + iconOnPrimaryFolder + "/return.png"
            }
            onClicked: {
                api.closeConfiguration()
            }
        }
    } // end RowLayout
} // end ToolBar
