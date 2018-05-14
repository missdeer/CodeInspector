
import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "../pages"

StackView {
    id: navPane
    property string name: "HomeNavPane"
    // index to get access to Loader (Destination)
    property int myIndex: index

    initialItem: HomePage{}

    Loader {
        id: homePageLoader
        active: true
        visible: false
        source: "../pages/HomePage.qml"
    }

    function pushQtPage() {
        navPane.push(homePageLoader.item)
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT HomeNavPane")
    }
    function cleanup() {
        console.log("CLEANUP HomeNavPane")
    }

} // navPane
