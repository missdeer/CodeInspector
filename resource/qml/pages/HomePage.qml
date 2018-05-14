
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "../common"
import "../popups"

Flickable {
    id: flickable
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "Home"

    Pane {
        id: root
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            LabelHeadline {
                leftPadding: 10
                bottomPadding: 16
                text: qsTr("Introduce")
            }
            RowLayout {
                IconInactive {
                    imageName: modelData.icon
                    imageSize: 48
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Understand how your code is executed.")
                }
                Item {
                    implicitWidth: 96
                    implicitHeight: 96
                    Image {
                        // scale: Image.PreserveAspectFit
                        anchors.fill: parent
                        // anchors.verticalCenterOffset: -50
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/codeinspector.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            navPane.pushQtPage()
                        }
                    } // mouse
                }
            }

            HorizontalDivider {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft | Qt.AlignRight
                Layout.margins: 6
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Run compilers interactively and interact with the assembly.")
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Compiler Explorer is an interactive compiler. \nThe left-hand pane shows editable C, C++, Rust, Go, D, Haskell, Swift and Pascal code. \nThe right, the assembly output of having compiled the code with a given compiler and settings. \nMultiple compilers are supported.\n")
                }
            }

            HorizontalDivider {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft | Qt.AlignRight
                Layout.margins: 6
            }
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from Home Page"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from Home Page"))
    }
} // flickable
