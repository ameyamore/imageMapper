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
        for(let x of gpslist)
        {
            console.log(x)
        }
    }

    Map {
        id:io
        anchors.fill: parent
        zoomLevel: 14
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
                    id: image2
                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
                }
            }
        }
    }
}
