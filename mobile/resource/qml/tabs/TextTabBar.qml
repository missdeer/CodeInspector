
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0
import "../common"

TabBar {
    id: myTabBar
    Layout.fillWidth: true
    currentIndex: 0
    onCurrentIndexChanged: {
        console.log("Tab Bar current index changed: "+ currentIndex)
        navPane.currentIndex = currentIndex
    }
    Repeater {
        model: tabButtonModel
        TabButton {
            focusPolicy: Qt.NoFocus
            text: modelData.name
            width: tabBarIsFixed? myTabBar.width / tabButtonModel.length  : Math.max(112, myTabBar.width / tabButtonModel.length)
        }
    } // repeater

}
