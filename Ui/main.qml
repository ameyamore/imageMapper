import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 5.6
import QtPositioning 5.6

Window {
    width: Screen.width
    height: Screen.height
    visible: true

    Map {
        id:io
        anchors.fill: parent
        zoomLevel: 14
        plugin: Plugin {
            name: "osm"
        }
        activeMapType: supportedMapTypes[0]
        MapQuickItem {
            id: marker
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: QtPositioning.coordinate(-27.5, 153.1)

            sourceItem: Image {
                id: image
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
            }
        }
    }
}
