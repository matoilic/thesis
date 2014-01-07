import QtQuick 2.2
import QtQuick.Controls 1.1

ApplicationWindow {
    title: "Door-tracked augmented reality"
    id: mainWindow
    width: 320
    height: 480
    visible: true

    Text {
        id: lblTitle
        x: 8
        y: 8
        width: 304
        height: 22
        text: "Ardoor Settings"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        font.bold: true
        font.pixelSize: 18
    }

    CheckBox {
        id: chkFlipBackground
        height: 17
        text: "Flip Background (when using front camera)"
        checked: controller.flipBackground
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: lblTitle.bottom
        anchors.topMargin: 30
    }

    GroupBox {
        id: grpChessboardSize
        height: 102
        title: "Chessboard size"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: chkFlipBackground.bottom
        anchors.topMargin: 30

        TextField {
            id: txtWidth
            x: 86
            width: 80
            height: 20
            text: controller.boardWidth
            validator: IntValidator { bottom: 1; }
            placeholderText: ""
            anchors.top: lblWidth.bottom
            anchors.topMargin: -17
        }

        TextField {
            id: txtHeight
            width: 80
            height: 20
            text: controller.boardHeight
            placeholderText: ""
            validator: IntValidator {bottom: 1; }
            anchors.top: lblHeight.bottom
            anchors.topMargin: -17
            anchors.left: txtWidth.right
            anchors.leftMargin: -80
        }

        Label {
            id: lblWidth
            width: 70
            height: 14
            text: "Width:"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        Label {
            id: lblHeight
            width: 70
            height: 14
            text: "Height:"
            anchors.left: lblWidth.right
            anchors.leftMargin: -70
            anchors.top: lblWidth.bottom
            anchors.topMargin: 15
        }
    }

    Flow {
        id: buttonRow
        width: 290
        height: 33
        spacing: 6
        anchors.top: grpChessboardSize.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 15

        Button {
            id: btnSave
            x: 14
            y: 10
            width: 94
            text: "Save & quit"
            onClicked: {
                controller.flipBackground = chkFlipBackground.checked
                controller.boardWidth = txtWidth.text
                controller.boardHeight = txtHeight.text
                controller.saveSettings()
                Qt.quit()
            }
        }

        Button {
            id: btnQuit
            x: 108
            y: 20
            text: "Quit"
            onClicked: Qt.quit()
        }
    }


}
