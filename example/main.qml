import QtQuick 2.5
import QtQuick.Window 2.2
import xyz.jandal.groupview 1.0

Window {
	id: root
	visible: true

	width: 640
	height: 480

	GroupView {
		id: groups
		anchors.fill: parent
		focus: true

		model: instancesModel

		headerComponent: Item {
			height: 25

			Canvas {
				id: decorationCanvas
				anchors.fill: parent
				anchors.margins: 5
				anchors.bottomMargin: 0
				onPaint: {
					var ctx = decorationCanvas.getContext('2d');
					ctx.lineWidth = 1;

					ctx.beginPath();
					ctx.moveTo(0, 5);
					ctx.arc(5, 5, 5, Math.PI, 3*Math.PI/2, false);
					ctx.lineTo(width - 5, 0);
					ctx.arc(width - 5, 5, 5, 3*Math.PI/2, 0, false);
					ctx.strokeStyle = "black";
					ctx.stroke();

					ctx.beginPath();
					ctx.moveTo(width, 5);
					ctx.lineTo(width, height);
					var rightGrad = ctx.createLinearGradient(width, 5, width, height);
					rightGrad.addColorStop(0.0, "black");
					rightGrad.addColorStop(1.0, "transparent");
					ctx.strokeStyle = rightGrad;
					ctx.stroke();

					ctx.beginPath();
					ctx.moveTo(0, 5);
					ctx.lineTo(0, height);
					var leftGrad = ctx.createLinearGradient(0, 5, 0, height);
					leftGrad.addColorStop(0.0, "black");
					leftGrad.addColorStop(1.0, "transparent");
					ctx.strokeStyle = leftGrad;
					ctx.stroke();
				}
			}

			Rectangle {
				id: buttonBorder
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 2
				anchors.left: parent.left
				anchors.leftMargin: 15
				width: 15
				height: 15
				border.width: 1
				border.color: "black"
				color: "transparent"

				Rectangle {
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					anchors.margins: 2
					width: 1
					color: "black"
					rotation: expanded ? 0 : 90
					Behavior on rotation { SmoothedAnimation {} }
				}
				Rectangle {
					anchors.verticalCenter: parent.verticalCenter
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.margins: 2
					height: 1
					color: "black"
				}

				MouseArea {
					onClicked: expanded = !expanded
					anchors.fill: parent
				}
			}

			Text {
				anchors.left: buttonBorder.right
				anchors.leftMargin: 5
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 2
				text: name
			}
		}

		itemComponent: Item {
			height: image.height + text.height + text.anchors.topMargin

			Image {
				id: image
				source: decoration
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: parent.top
				width: 50
				height: 50
			}

			Text {
				id: text
				text: display
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: image.bottom
				anchors.topMargin: 5
				width: parent.width
				wrapMode: Text.WordWrap
				horizontalAlignment: Text.AlignHCenter
			}
		}
	}

	ScrollBar {
		id: groupsScrollbar
		flickable: groups

		Item {
			Rectangle {
				anchors.fill: parent
				anchors.rightMargin: 1

				color: "darkGray"
				radius: 5
				opacity: hovered ? 0.7 : 0.0
				anchors.leftMargin: hovered ? 0 : 9

				Behavior on opacity { NumberAnimation { duration: 200 } }
				Behavior on anchors.leftMargin { SmoothedAnimation { duration: 1000 } }
			}
		}
	}
}

