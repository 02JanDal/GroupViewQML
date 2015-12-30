/* Copyright 2015 Jan Dalheimer <jan@dalheimer.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import QtQuick 2.0

Item {
	id: root

	property Flickable flickable
	default property Component delegate
	property int orientation: Qt.Vertical

	parent: flickable
	anchors.fill: parent

	readonly property bool __isVertical: orientation === Qt.Vertical
	readonly property real __position: __isVertical ? flickable.visibleArea.yPosition : flickable.visibleArea.xPosition
	readonly property real __size: __isVertical ? flickable.height : flickable.width
	readonly property real __ratio: __isVertical ? flickable.visibleArea.heightRatio : flickable.visibleArea.widthRatio
	readonly property bool __isActive: __isVertical ? (flickable.draggingVertically || flickable.flickingVertically)
													: (flickable.draggingHorizontally || flickable.flickingHorizontally)

	Loader {
		id: handle
		visible: !isNaN(__position)

		y: __position * __size
		height: __isVertical ? __ratio * __size : 10
		width: __isVertical ? 10 : __ratio * __size
		anchors.right: parent.right

		Binding { target: parent; property: "y"; value: __position * __size; when: __isVertical }
		Binding { target: parent; property: "x"; value: __position * __size; when: !__isVertical }
		Binding { target: parent; property: "anchors.right"; value: parent.parent.right; when: __isVertical }
		Binding { target: parent; property: "anchors.bottom"; value: parent.parent.bottom; when: !__isVertical }

		readonly property bool hovered: area.hovered || __isActive

		sourceComponent: delegate

		MouseArea {
			anchors.fill: parent
			preventStealing: true
			property point lastPoint: Qt.point(-1, -1)
			onPressed: {
				lastPoint = root.mapFromItem(this, mouse.x, mouse.y);
				mouse.accepted = true
			}
			onReleased: {
				update(root.mapFromItem(this, mouse.x, mouse.y));
				mouse.accepted = true
			}
			onMouseXChanged: {
				update(root.mapFromItem(this, mouse.x, mouse.y));
				mouse.accepted = true
			}
			function update(newPoint) {
				if (__isVertical) {
					var diffY = newPoint.y - lastPoint.y;
					flickable.contentY += flickable.contentHeight * diffY / flickable.height;
				} else {
					var diffX = newPoint.x - lastPoint.x;
					flickable.contentX += flickable.contentWidth * diffX / flickable.width;
				}
				lastPoint = newPoint;
			}
		}
	}

	MouseArea {
		id: area
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		width: __isVertical ? 15 : parent.width
		height: __isVertical ? parent.height : 15
		hoverEnabled: true
		property bool hovered: false
		onEntered: hovered = true
		onExited: hovered = false
		onPressed: mouse.accepted = false
	}
}
