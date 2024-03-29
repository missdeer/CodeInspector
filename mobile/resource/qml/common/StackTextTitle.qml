
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

ToolBar {
    id: titleToolBar
    property alias text: titleLabel.text

    RowLayout {
        focus: false
        spacing: 6
        anchors.fill: parent
        ToolButton {
            enabled: navPane.depth > 1
            focusPolicy: Qt.NoFocus
            Image {
                id: backImageImage
                visible: navPane.depth > 1
                anchors.centerIn: parent
                source: "qrc:/resource/image/"+iconOnPrimaryFolder+"/arrow_back.png"
            }
            onClicked: {
                navPane.popOnePage()
            }
        }
        LabelTitle {
            id: titleLabel
            text: "ekke"
            // leftPadding: 16
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: textOnPrimary
        }
        ToolButton {
            focusPolicy: Qt.NoFocus
            Image {
                id: buttonImage
                anchors.centerIn: parent
                source: "qrc:/resource/image/"+iconOnPrimaryFolder+"/more_vert.png"
            }
            onClicked: {
                optionsMenu.open()
            }
            Menu {
                id: optionsMenu
                modal:true
                dim: false
                closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
                x: parent.width - width
                transformOrigin: Menu.TopRight
                MenuItem {
                    text: isDarkTheme? qsTr("Light Theme") : qsTr("Dark Theme")
                    onTriggered: {
                        themePalette = appUI.swapThemePalette()
                    }
                }
                MenuItem {
                    text: qsTr("Select Primary Color")
                    onTriggered: {
                        popup.selectAccentColor = false
                        popup.open()
                    }
                }
                MenuItem {
                    text: qsTr("Select Accent Color")
                    onTriggered: {
                        popup.selectAccentColor = true
                        popup.open()
                    }
                }
                onAboutToHide: {
                    appWindow.resetFocus()
                }
            } // end optionsMenu

        } // end ToolButton
    } // end RowLayout
} // end ToolBar


