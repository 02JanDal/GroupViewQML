import QtQuick 2.0
import xyz.jandal.groupview.internal 1.0

ListView {
	id: list

	property int itemWidth: 64
	property int itemPadding: 5

	property Component headerComponent
	property Component itemComponent

	property Transition itemMove
	property Transition itemAdd
	property Transition itemPopulate

	GroupViewHelper {
		id: helper

		model: list.model
		maxColumns: Math.floor((width - itemPadding) / (itemWidth + itemPadding))

		onCurrentObjectChanged: {
			if (hasCurrent) {
				console.debug("current: " + dataForIndex(currentIndex, "display"));
			} else {
				console.debug("current cleared");
			}
		}
	}

	function mapCoordinates(from, to) {
		return to.mapFromItem(from, 0, 0, from.width, from.height);
	}

	Keys.onLeftPressed: helper.moveCursor(GroupViewHelper.Left)
	Keys.onRightPressed: helper.moveCursor(GroupViewHelper.Right)
	Keys.onUpPressed: helper.moveCursor(GroupViewHelper.Up)
	Keys.onDownPressed: helper.moveCursor(GroupViewHelper.Down)

	Rectangle {
		id: highlight
		parent: list.contentItem
		opacity: helper.currentObject ? 0.5 : 0.0
		property rect pos: (helper.currentObject && parent) ? list.mapCoordinates(helper.currentObject, parent)
															: Qt.rect(parent.width/2, parent.height/2, 0, 0)
		property real padding: 2
		x: pos.x - padding
		y: pos.y - padding
		width: pos.width + padding *2
		height: pos.height + padding *2
		radius: 5
		z: -1

		Behavior on x { SmoothedAnimation { duration: 500 } }
		Behavior on y { SmoothedAnimation { duration: 500 } }
		Behavior on width { SmoothedAnimation { duration: 500 } }
		Behavior on height { SmoothedAnimation { duration: 500 } }
		Behavior on opacity { SmoothedAnimation { duration: 1000; easing.type: Easing.InCubic } }

		border.color: "darkBlue"
		border.width: 1
		color: "lightSteelBlue"
	}

	delegate: GroupViewDropArea {
		id: group
		height: header.height + grid.height + grid.anchors.margins
		width: list.width
		groupViewHelper: helper
		dropTargetIndex: __modelIndex

		Rectangle {
			readonly property Item nextTo: group.showIndicatorNextTo

			id: dropIndicator
			parent: nextTo ? nextTo : group
			color: "black"
			width: list.itemPadding
			height: nextTo ? nextTo.height : list.itemWidth
			anchors.right: nextTo ? nextTo.left : undefined
			anchors.left: nextTo ? undefined : grid.right
			anchors.bottom: nextTo ? undefined : grid.bottom
			visible: group.showIndicator

			states: [
				State {
					when: !parent.nextTo
					ParentChange { target: dropIndicator; parent: grid }
					AnchorChanges { target: dropIndicator; anchors.right: grid.right; anchors.bottom: grid.bottom }
				},
				State {
					when: !!parent.nextTo
					ParentChange { target: dropIndicator; parent: dropIndicator.nextTo }
					AnchorChanges { target: dropIndicator; anchors.right: dropIndicator.nextTo.left }
				}
			]
		}

		property bool expanded: true

		states: [
			State {
				name: "open"
				PropertyChanges { target: grid; height: grid.implicitHeight }
			},
			State {
				name: "closed"
				PropertyChanges { target: grid; height: 0 }
			}
		]
		state: expanded ? "open" : "closed"
		transitions: Transition {
			SmoothedAnimation { target: grid; velocity: 250; properties: "height" }
		}

		Loader {
			id: header
			sourceComponent: list.headerComponent
			width: parent.width

			property string name: display
			property alias expanded: group.expanded
		}

		Flow {
			id: grid
			anchors.top: header.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			clip: true
			anchors.margins: list.itemPadding
			spacing: list.itemPadding

			move: list.itemMove
			add: list.itemAdd
			populate: list.itemPopulate

			Repeater {
				model: __childrenList
				delegate: MouseArea {
					id: dragArea
					width: list.itemWidth
					height: loader.height

					drag.target: loader

					onClicked: helper.clicked(__modelIndex)

					readonly property Item item: loader

					Loader {
						id: loader
						sourceComponent: list.itemComponent

						anchors.horizontalCenter: parent.horizontalCenter
						anchors.verticalCenter: parent.verticalCenter
						width: dragArea.width

						Drag.active: dragArea.drag.active
						Drag.source: loader
						Drag.hotSpot.x: width/2
						Drag.hotSpot.y: height/2
						states: [
							State {
								when: loader.Drag.active
								ParentChange {
									target: loader
									parent: list
								}
								AnchorChanges {
									target: loader
									anchors.horizontalCenter: undefined
									anchors.verticalCenter: undefined
								}
							}
						]

						property string display: model.display
						property var decoration: model.decoration
						readonly property var modelIndex: __modelIndex
						readonly property bool isCurrent: helper.currentIndex === __modelIndex

						Component.onCompleted: helper.registerObject(__modelIndex, this)
						Component.onDestruction: helper.unregisterObject(this)
					}
				}
			}
		}
	}
}
