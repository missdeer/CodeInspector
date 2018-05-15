import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    property string name: "ExamplesPage"
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.height
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    clip: true

    Pane {
        id: root
        anchors.fill: parent
        padding: 24

        ColumnLayout {
            id: theContent
            anchors.right: parent.right
            anchors.left: parent.left
            RowLayout {
                id: subjectLayout
                IconInactive {
                    imageName: modelData.icon
                    imageSize: 48
                }
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("Load an example")
                }
            }
            HorizontalDivider {}

            Repeater {
                model: api.examples
                RowLayout {
                    LabelTitle {
                        verticalAlignment: Qt.AlignVCenter
                        topPadding: 10
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: modelData
                    }
                    Button {
                        id: control
                        text: qsTr("Load")
                        contentItem: Text {
                            text: control.text
                            font: control.font
                            opacity: enabled ? 1.0 : 0.3
                            color: control.down ? "#17a81a" : "#21be2b"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 90
                            implicitHeight: 20
                            opacity: enabled ? 1 : 0.3
                            border.color: control.down ? "#17a81a" : "#21be2b"
                            border.width: 1
                            radius: 8
                        }
                        onClicked: {
                            api.loadExample(index)
                            api.closeConfiguration()
                        }
                    }
                }
            }
        } // col layout

    } // root

    ScrollIndicator.vertical: ScrollIndicator { }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SettingsPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SettingsPage"))
    }

} // flickable
