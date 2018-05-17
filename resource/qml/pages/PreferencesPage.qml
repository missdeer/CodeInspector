
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    property string name: "PreferencesPage"
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
                    text: qsTr("Preferences")
                }
            }
            HorizontalDivider {}
            RowLayout {
                Switch {
                    focusPolicy: Qt.NoFocus
                    leftPadding: 12
                    text: qsTr("Remember last session")
                    checked: api.rememberLastSession
                    onCheckedChanged: api.rememberLastSession = checked
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: "Default language"
                }
                ComboBox {
                    model: api.languages
                    currentIndex: api.defaultLanguageIndex
                    onCurrentIndexChanged: api.defaultLanguageIndex = currentIndex
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: "Auto refresh interval(sec)"
                }
                SpinBox {
                    id: sbAutoRefreshInterval
                    value: api.autoRefreshInterval
                    from: 500
                    to: 5000
                    stepSize: 100

                    property int decimals: 1
                    property real realValue: value / 1000

                    validator: DoubleValidator {
                        bottom: Math.min(sbAutoRefreshInterval.from, sbAutoRefreshInterval.to)
                        top:  Math.max(sbAutoRefreshInterval.from, sbAutoRefreshInterval.to)
                    }

                    textFromValue: function(value, locale) {
                        return Number(value / 1000).toLocaleString(locale, 'f', sbAutoRefreshInterval.decimals)
                    }

                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text) * 1000
                    }

                    onValueChanged: api.autoRefreshInterval = value
                }
            }
            RowLayout {
                LabelSubheading {
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: "Editor zoom factor(%)"
                }
                SpinBox {
                    id: sbZoomFactor
                    value: api.editorZoomFactor
                    from: 50
                    to: 1000
                    stepSize: 10

                    onValueChanged: api.editorZoomFactor = value
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
        console.log(qsTr("Init done from PreferencesPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from PreferencesPage"))
    }

} // flickable
