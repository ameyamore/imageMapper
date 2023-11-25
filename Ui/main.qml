import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 5.6
import QtPositioning 5.6



Window {
    width: Screen.width
    height: Screen.height
    visible: true
    property var gpslist: []
    property int start: 0
    property int end: -1
    property int xi: 0
    property int yi: 0

    Component.onCompleted: {
        gpslist = metaDataParser.getImageRecords();
        for(let point of gpslist)
        {
            routeRequest.addWaypoint(point)
        }
        routemodel.update();
        end = gpslist.length - 1

    }

    function displayImage(index)
    {
        xi = 0;
        yi = 0;
        if(index === start)
        {
            xi = -29/2
            return "qrc:/icons/start.png"
        }
        else if(index === end)
        {
            yi = 44 * -1
            return "qrc:/icons/end.png"
        }
        else
        {
            xi = 0
            yi = 0
            return "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
        }
    }

    RouteQuery {
        id:routeRequest

    }
    RouteModel {
        id:routemodel
        plugin: Plugin {
            name: "osm"
        }
        query:routeRequest
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
            id:tracedRoutes
            model: routemodel
            delegate: MapRoute {
                route: routeData
                line.color: "blue"
                line.width: 5
                smooth: true
                opacity: 0.8
            }
        }
        MapItemView {
            id:tracedimageMarkers
            model: gpslist
            delegate: MapQuickItem {
                coordinate: gpslist[index]
                sourceItem: Image {
                    id: pinImage
                    source: displayImage(index)
                    x:xi
                    y:yi
                    MouseArea
                    {
                        anchors.fill: pinImage
                        onClicked: {
                            io.zoomLevel = 1.2 * io.zoomLevel
                            io.center = gpslist[index]
                            console.log(pinImage.x," ",pinImage.y)
                        }
                    }
                }
            }
        }
        MapRoute {
            line.color: "red"
            line.width: 3
            route: Route {
                path: gpslist
            }
        }
    }
}
