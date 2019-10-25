import QtQuick 2.10
import QtQuick.Window 2.10
import QtWebSockets 1.0
import QtQuick.Controls 2.0
Window {
    visible: true
    width: 360
    height: 720
    title: qsTr("Hello World")
    color: "#121212"
    Rectangle
    {
        id: connectionStatus
        width: parent.width/1.5
        anchors.bottom: button.top
        anchors.bottomMargin: 30
        color: "red"
        height: 70
        property string status: "not connect"
        Text {
            anchors.centerIn: parent
            text: parent.status
        }
    }
    Rectangle {
        id: button
        x: parent.width/2 - width/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        width: parent.width/1.5
        height: parent.width/1.5
        color: "#bb8afa"
        radius: height

        Text {
            anchors.centerIn: parent
            width: 160
            height: 140
            color: "#111111"
            text: qsTr("Record")
            font.pixelSize: 40
        }



        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                button.color = "#d5667a"
                var  jsonString = '{"status":' + '"record"' +',' +
                        '"gessture":' + '"' + gessture.text +  '"'
                        +'}'
                //console.log(jsonString)
                var JsonObject= JSON.parse(jsonString);
                socket.sendBinaryMessage(jsonString)
                console.log(jsonString)
            }
            onReleased:
            {
                button.color = "#bb8afa"
                var  jsonString = '{"status":' + '"stop"' +',' +
                        '"gessture":' + '"' + gessture.text +  '"'
                        +'}'
                //console.log(jsonString)
                var JsonObject= JSON.parse(jsonString);

                socket.sendBinaryMessage(jsonString)
                console.log(jsonString)
            }

        }
        WebSocket {
            id: socket
            //url: "ws://192.168.43.255:8000"
            onTextMessageReceived: {
                console.log("receive")
            }
            onStatusChanged:
            {
                if (socket.status == WebSocket.Error) {
                    console.log(socket.url)
                    console.log("Error: " + socket.errorString)
                    connectionStatus.status = "not connect"
                    connectionStatus.color = "red"
                    connectorTimer.start()
                } else if (socket.status == WebSocket.Open)
                {
                    connectorTimer.stop();
                    connectionStatus.status = "connected"
                    connectionStatus.color = "green"
                } else if (socket.status == WebSocket.Closed)
                {
                    connectionStatus.status = "not connect"
                    connectionStatus.color = "red"
                    connectorTimer.start()

                }
            }
            active: false
        }
        Timer {
            id: connectorTimer
            interval: 1000; running: true; repeat: true
            onTriggered:
            {

                socket.active = !socket.active
            }
        }
    }

    TextEdit {
        id: gessture
        anchors.left: parent.left
        anchors.top : parent.top
        anchors.topMargin : 30
        width: parent.width
        height: 60
        text: "gesture name"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 24
        color: "#DDDDDD"
        font { pointSize  : parent.height }

    }

    TextField
    {
        id: ipStr
        anchors.left: parent.left
        anchors.top : gessture.bottom
        anchors.topMargin : 30
        width: parent.width
        height: 60
        placeholderText: "ip"
        onEditingFinished:
        {
            socket.url =  "ws://" + ipStr.text + ":" + portStr.text
            console.log(ipStr.text )
            console.log(socket.url)
        }
    }
    TextField
    {
        id: portStr
        anchors.left: parent.left
        anchors.top : ipStr.bottom
        anchors.topMargin : 30
        width: parent.width
        height: 60
        placeholderText: "port"
        onEditingFinished:
        {
            socket.url =  "ws://" + ipStr.text + ":" + portStr.text
            var textSample = "ws://" + ipStr.text + ":" + portStr.text

            console.log(portStr.text)
            console.log(socket.url)
            console.log(textSample)
        }
    }



}
