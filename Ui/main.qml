import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.5


Window {
    width: Screen.width
    height: Screen.height
    visible: true
    property var gpslist: []
    property int start: 0
    property int end: -1
    property int xi: 0
    property int yi: 0
    property bool markerVisible: true
    property bool routeVisible: true
    property bool polylineVisible: true

    Component.onCompleted: {
        gpslist = metaDataParser.getImageRecords();
        end = gpslist.length - 1

        for(let point of gpslist)
        {
            routeRequest.addWaypoint(point)
        }
        routemodel.update()
        na.start()
    }

    function displayImage(index)
    {
        xi = 0;
        yi = 0;
        if(index === start)
        {
            xi = -29/2
            yi = 44 * -1
            return "qrc:/icons/start.png"
        }
        else if(index === end)
        {
            xi = -29/2
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
        routeOptimizations: RouteQuery.ShortestRoute

    }
    RouteModel {
        id:routemodel
        plugin: Plugin {
            name: "osm"
            PluginParameter { name: "osm.mapping.providersrepository.address"; value: "file:///home/amey/Codebase/map/imageMapper/API/qt-osm-map-providers/" }
        }
        query:routeRequest
    }

    Map {
        id:io
        anchors.fill: parent
        zoomLevel: 17
        center:gpslist[1]
        bearing: 0
        tilt: 45
        plugin: Plugin {
            name: "osm"
            PluginParameter { name: "osm.mapping.providersrepository.address"; value: "file:///home/amey/Codebase/map/imageMapper/API/qt-osm-map-providers/" }
            PluginParameter { name: "osm.mapping.highdpi_tiles"; value: true }
        }

        Row{
            id:controlBoxes
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 60
            spacing: 150
            CheckBox {
                id:markerVisibilitySelection
                height: 10
                width: 10
                checked: true
                onCheckStateChanged: {
                    markerVisible = !markerVisible
                }
                Text {
                    text: "Markers visibility"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.bottom
                    anchors.topMargin: 15
                }
            }
            CheckBox {
                id:routeVisibilitySelection
                height: 10
                width: 10
                checked: true
                onCheckStateChanged: {
                    routeVisible = !routeVisible
                }
                Text {
                    text: "Routes visibility"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.bottom
                    anchors.topMargin: 15
                }
            }
            CheckBox {
                id:polylineVisibilitySelection
                height: 10
                width: 10
                checked: true
                onCheckStateChanged: {
                    polylineVisible = !polylineVisible
                }
                Text {
                    text: "Polylines visibility"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.bottom
                    anchors.topMargin: 15
                }
            }
        }

        activeMapType: supportedMapTypes[4]
        MapItemView {
            id:tracedRoutes
            visible: routeVisible
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
            visible: markerVisible
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
            id:polyLineRoute
            visible: polylineVisible
            line.color: "red"
            line.width: 6
            opacity: 0.5
            route: Route {
                path: gpslist
            }
        }
    }
    NumberAnimation {
        id: na    //ID of the QML Animation type
        target: io    //The target item on which the animation should run
        property: "bearing"    //The property of the target item which should be changed by the animator to show effect
        duration: 100000    //The duration for which the animation should run
        from: io.bearing    //The initial numeric value of the property declared in 'property'
        to: 2000    //The final numeric value of the property declared in 'property'
        easing.type: Easing.InOutQuad
    }
}
