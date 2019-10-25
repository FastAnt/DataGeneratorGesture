import QtQuick 2.8
import QtQuick.Window 2.2
import QtMultimedia 5.9
import com.dataGen.ImageCollector 1.0
import QtWebSockets 1.0
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Item {
        anchors.fill: parent

        Camera {
            id: camera
            deviceId: QtMultimedia.availableCameras[1].deviceId
            captureMode : Camera.CaptureVideo

            viewfinder.resolution: Qt.size(640,480)

            viewfinder.minimumFrameRate: 60
            imageCapture {
                onImageCaptured: {
                    // Show the preview in an Image
                    photoPreview.source = preview
                }
            }
        }

        VideoOutput {
            id:videoOut
            source: camera
            focus : visible // to receive focus and capture key events when visible
            anchors.fill: parent

            Component.onCompleted:
            {
                collector.setCamera(videoOut)
            }
        }
    }

    Rectangle
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 70
        Text {
            anchors.centerIn: parent
            font { pointSize  : parent.height }
            text: hostIp
        }
    }
    Timer {
        id: recorder
        interval: 50; running: false; repeat: true
        property int framme: 0
        property int attemptID: 300000
        onTriggered:{
            collector.frammeID = (framme)
            collector.attemptID = attemptID
            collector.getFramme()
            framme = framme + 1
        }
    }
    ImageCollector{
        id: collector
        gesstureName: "SwipeLeft"
        attemptID: "300000"


    }

    MouseArea {
        anchors.fill: parent;

        onPressed:
        {
            recorder.start()
        }
        onReleased:
        {
            recorder.attemptID = recorder.attemptID +  1
            recorder.framme = 0
            recorder.stop()
        }
    }

    WebSocketServer {
        id: server
        listen: true
        port : 8080
        host: hostIp
        onClientConnected: {

            webSocket.onBinaryMessageReceived.connect(function(message) {


                var  jsonString = message
                var JsonObject= JSON.parse(jsonString);
                var gessture = JsonObject.gessture
                var status = JsonObject.status

                if(status === "record")
                {
                    recorder.start()
                    collector.gesstureName = gessture
                }
                else
                {
                    recorder.attemptID = recorder.attemptID +  1
                    recorder.framme = 0
                    recorder.stop()
                }


            });
        }
        onErrorStringChanged: {
            appendMessage(qsTr("Server error: %1").arg(errorString));
        }
        Component.onCompleted:
        {
            console.log("host ip is : " , hostIp)
        }
    }

}
