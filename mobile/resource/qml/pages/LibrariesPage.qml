import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0
import com.dfordsoft.codeinspector 1.0
import "../common"

Flickable {
    property string name: "LibrariesPage"
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
                IconInactive {
                    imageName: modelData.icon
                    imageSize: 48
                }
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("Select used libraries")
                }
            }
            HorizontalDivider {
            }
            Repeater {
                model: api.libs
                ColumnLayout {
                    //anchors.right: parent.right
                    //anchors.left: parent.left
                    Layout.alignment: Qt.AlignLeft | Qt.AlignRight
                    RowLayout {
                        LabelSubheading {
                            topPadding: 6
                            leftPadding: 10
                            rightPadding: 10
                            wrapMode: Text.WordWrap
                            text: modelData.name
                        }
                        Image {
                            source: "qrc:/resource/image/black/x18/hyperlink.png"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: Qt.openUrlExternally(modelData.url)
                            }
                        }
                    }
                    Repeater {
                        id: library
                        model: modelData.versions
                        property string name: modelData.name
                        RowLayout {
                            Switch {
                                id: cb
                                topPadding: 6
                                leftPadding: 10
                                rightPadding: 10
                                text: modelData.version
                                checked: modelData.selected
                                onCheckedChanged: {
                                    if (modelData.selected != checked) {
                                        modelData.selected = checked
                                        api.librarySwitched()
                                    }
                                    if (checked) {
                                        console.log("add: " + library.name + " : "
                                                    + text + " - " + modelData.path)
                                    } else {
                                        console.log("remove: " + library.name + " : "
                                                    + text + " - " + modelData.path)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } // col layout
    } // root

    ScrollIndicator.vertical: ScrollIndicator {
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from LibrariesPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from LibrariesPage"))
    }
} // flickable
