
import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
Item {
    implicitWidth: 8
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    // https://www.google.com/design/spec/components/dividers.html#dividers-types-of-dividers
    Rectangle {
        id: theRectangle
        width: 1
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: dividerOpacity
        color: dividerColor
    }
}
