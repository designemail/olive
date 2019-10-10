/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "widget/timelineview/timelineview.h"

TimelineView::RazorTool::RazorTool(TimelineView* parent) :
  Tool(parent)
{
}

void TimelineView::RazorTool::MousePress(TimelineViewMouseEvent *event)
{
  split_tracks_.clear();

  MouseMove(event);
}

void TimelineView::RazorTool::MouseMove(TimelineViewMouseEvent *event)
{
  if (!dragging_) {
    drag_start_ = event->GetCoordinates();
    dragging_ = true;
  }

  // Split at the current cursor track
  int split_track = event->GetCoordinates().GetTrack();

  if (!split_tracks_.contains(split_track)) {
    split_tracks_.append(split_track);
  }
}

void TimelineView::RazorTool::MouseRelease(TimelineViewMouseEvent *event)
{
  Q_UNUSED(event)

  // Always split at the same time
  rational split_time = drag_start_.GetFrame();

  QUndoCommand* command = new QUndoCommand();

  foreach (int track, split_tracks_) {
    new TrackSplitAtTimeCommand(parent()->timeline_node_->TrackAt(track), split_time, command);
  }

  split_tracks_.clear();

  olive::undo_stack.pushIfHasChildren(command);

  dragging_ = false;
}
