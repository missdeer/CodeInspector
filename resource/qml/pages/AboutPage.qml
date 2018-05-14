
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    id: flickable
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "About"

    Pane {
        id: root
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            RowLayout{
                IconInactive {
                    imageName: modelData.icon
                    imageSize: 48
                }
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("About")
                }
            }
            HorizontalDivider {}
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Copyright (C) 2017 - 2018 CodeInspector. All rights reserved.")                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Homepage: <a href='https://www.codeinspector.com'>https://www.codeinspector.com</a>\n")
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Twitter: <a href='https://twitter.com/missdeerme'>@missdeerme</a>\n")
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Email: <a href='mailto:missdeer@gmail.com'>missdeer@gmail.com</a>\n")
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
            HorizontalDivider {}
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from ABOUT"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from ABOUT"))
    }
} // flickable
