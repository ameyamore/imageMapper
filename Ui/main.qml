import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 5.6
import QtPositioning 5.6



Window {
    width: Screen.width
    height: Screen.height
    visible: true
    property var gpslist: []

    Component.onCompleted: {
        gpslist = metaDataParser.getImageRecords();

    }

    Map {
        id:io
        anchors.fill: parent
        zoomLevel: 17
        center:gpslist[0]
        plugin: Plugin {
            name: "osm"
        }
        activeMapType: supportedMapTypes[0]
        MapItemView {
            id: marker2
            model: gpslist
            delegate: MapQuickItem {
                coordinate: gpslist[index]
                sourceItem: Image {
                    id: pinImage
                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
                    MouseArea
                    {
                        anchors.fill: pinImage
                        onClicked: {
                            io.zoomLevel = 1.2 * io.zoomLevel
                            io.center = gpslist[index]
                        }
                    }
                }
            }
        }
        MapRoute {
            line.color: "blue"
            line.width: 3
            route: Route {
                path: gpslist
            }
        }
    }
}
