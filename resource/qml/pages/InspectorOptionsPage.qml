
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    property string name: "Inspector Options"
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
            LabelHeadline {
                leftPadding: 10
                text: qsTr("Inspector Options")
            }
            IconInactive {
                imageName: modelData.icon
                imageSize: 48
            }
            HorizontalDivider {}
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/binary.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    topPadding: 6
                    leftPadding: 12
                    text: qsTr("Compile to binary and disassemble the output")
                    checked: api.binary
                    onCheckedChanged: api.binary = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/label.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Filter unused labels from the output")
                    checked: api.labels
                    onCheckedChanged: api.labels = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/directives.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Filter all assembler directives from the output")
                    checked: api.directives
                    onCheckedChanged: api.directives = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/comment.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Remove all lines which are only comments from the output")
                    checked: api.commentOnly
                    onCheckedChanged: api.commentOnly = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/trim.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Trim intra-line whitespace")
                    checked: api.trim
                    onCheckedChanged: api.trim = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/intel.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Output disassembly in intel syntax")
                    checked: api.intel
                    onCheckedChanged: api.intel = checked
                }
            }
            RowLayout {
                Image {
                    source: "qrc:/resource/image/black/demangle.png"
                    width: 24
                    height: 24
                    verticalAlignment: Image.AlignTop
                }
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Demangle output")
                    checked: api.demangle
                    onCheckedChanged: api.demangle = checked
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
        console.log(qsTr("Init done from InspectorOptionPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from InspectorOptionPage"))
    }

} // flickable
