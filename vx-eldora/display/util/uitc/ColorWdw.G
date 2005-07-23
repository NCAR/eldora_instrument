;GIL-2
(
(
	:type                   :base-window
	:name                   Base
	:owner                  nil
	:width                  558
	:height                 568
	:background-color       "Dark Turquoise"
	:foreground-color       "Black"
	:label                  "Colormap  Tool"
	:label-type             :string
	:mapped                 t
	:show-footer            t
	:resizable              t
	:icon-file              ""
	:icon-mask-file         ""
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Cntrl
	:owner                  Base
	:help                   ""
	:x                      0
	:y                      0
	:width                  558
	:height                 388
	:background-color       "Wheat"
	:foreground-color       "Black"
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :text-field
	:name                   FileName
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      8
	:width                  132
	:height                 15
	:foreground-color       ""
	:text-type              :alphanumeric
	:label                  "File:"
	:label-type             :string
	:value-x                52
	:value-y                8
	:layout-type            :horizontal
	:value-length           12
	:stored-length          256
	:read-only              nil
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   SaveButton
	:owner                  Cntrl
	:help                   ""
	:x                      192
	:y                      8
	:constant-width         nil
	:button-type            :normal
	:width                  46
	:height                 19
	:foreground-color       ""
	:label                  "Save"
	:label-type             :string
	:menu                   nil
	:notify-handler         SaveHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   LoadButton
	:owner                  Cntrl
	:help                   ""
	:x                      272
	:y                      8
	:constant-width         nil
	:button-type            :normal
	:width                  46
	:height                 19
	:foreground-color       ""
	:label                  "Load"
	:label-type             :string
	:menu                   nil
	:notify-handler         LoadHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   BrowseButton
	:owner                  Cntrl
	:help                   ""
	:x                      348
	:y                      8
	:constant-width         nil
	:button-type            :normal
	:width                  61
	:height                 19
	:foreground-color       ""
	:label                  "Browse"
	:label-type             :string
	:menu                   nil
	:notify-handler         BrowseHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Quit
	:owner                  Cntrl
	:help                   ""
	:x                      504
	:y                      8
	:constant-width         nil
	:button-type            :normal
	:width                  41
	:height                 19
	:foreground-color       ""
	:label                  "Exit"
	:label-type             :string
	:menu                   nil
	:notify-handler         QuitHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :setting
	:name                   ColorBins
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      68
	:width                  144
	:height                 23
	:value-x                88
	:value-y                68
	:layout-type            :horizontal
	:foreground-color       "Black"
	:setting-type           :exclusive
	:rows                   1
	:columns                0
	:label                  "Color Set:"
	:label-type             :string
	:notify-handler         ColorBinsHandler
	:event-handler          nil
	:events                 ()
	:choices                ("1" "2" "3" )
	:choice-label-types     (:string :string :string )
	:choice-colors          ("" "" "" )
	:user-data              ()
)
(
	:type                   :message
	:name                   RgbMsg
	:owner                  Cntrl
	:help                   ""
	:x                      12
	:y                      112
	:width                  81
	:height                 13
	:foreground-color       ""
	:label                  "RGB Values:"
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :slider
	:name                   RedSlider
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      148
	:width                  409
	:height                 26
	:value-x                52
	:value-y                148
	:slider-width           256
	:foreground-color       "Red"
	:label                  "Red:"
	:label-type             :string
	:layout-type            :horizontal
	:orientation            :horizontal
	:show-endboxes          t
	:show-range             t
	:show-value             t
	:min-value              0
	:max-value              255
	:ticks                  25
	:notify-handler         ColorHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :slider
	:name                   GreenSlider
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      180
	:width                  424
	:height                 26
	:value-x                67
	:value-y                180
	:slider-width           256
	:foreground-color       "Yellow Green"
	:label                  "Green:"
	:label-type             :string
	:layout-type            :horizontal
	:orientation            :horizontal
	:show-endboxes          t
	:show-range             t
	:show-value             t
	:min-value              0
	:max-value              255
	:ticks                  25
	:notify-handler         ColorHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :slider
	:name                   BlueSlider
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      216
	:width                  415
	:height                 26
	:value-x                58
	:value-y                216
	:slider-width           256
	:foreground-color       "Blue"
	:label                  "Blue:"
	:label-type             :string
	:layout-type            :horizontal
	:orientation            :horizontal
	:show-endboxes          t
	:show-range             t
	:show-value             t
	:min-value              0
	:max-value              255
	:ticks                  25
	:notify-handler         ColorHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   UndoButton
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      248
	:constant-width         nil
	:button-type            :normal
	:width                  50
	:height                 19
	:foreground-color       ""
	:label                  "Undo"
	:label-type             :string
	:menu                   nil
	:notify-handler         UndoHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :slider
	:name                   ColorIndex
	:owner                  Cntrl
	:help                   ""
	:x                      16
	:y                      300
	:width                  308
	:height                 20
	:value-x                122
	:value-y                300
	:slider-width           93
	:foreground-color       ""
	:label                  "Color Number:"
	:label-type             :string
	:layout-type            :horizontal
	:orientation            :horizontal
	:show-endboxes          t
	:show-range             t
	:show-value             t
	:min-value              0
	:max-value              30
	:ticks                  0
	:notify-handler         ColorIndexHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   ColorMsg
	:owner                  Cntrl
	:help                   ""
	:x                      184
	:y                      348
	:width                  191
	:height                 13
	:foreground-color       ""
	:label                  "Approximate  color values -"
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :canvas-pane
	:name                   Set1
	:owner                  Base
	:help                   ""
	:x                      0
	:y                      388
	:width                  558
	:height                 60
	:background-color       ""
	:foreground-color       ""
	:menu                   nil
	:horizontal-scrollbar   nil
	:scrollable-width       558
	:vertical-scrollbar     nil
	:scrollable-height      60
	:repaint-proc           nil
	:event-handler          nil
	:events                 ()
	:drawing-model          :xwindows
	:user-data              ()
)
(
	:type                   :canvas-pane
	:name                   Set2
	:owner                  Base
	:help                   ""
	:x                      0
	:y                      448
	:width                  558
	:height                 60
	:background-color       ""
	:foreground-color       ""
	:menu                   nil
	:horizontal-scrollbar   nil
	:scrollable-width       558
	:vertical-scrollbar     nil
	:scrollable-height      60
	:repaint-proc           nil
	:event-handler          nil
	:events                 ()
	:drawing-model          :xwindows
	:user-data              ()
)
(
	:type                   :canvas-pane
	:name                   Set3
	:owner                  Base
	:help                   ""
	:x                      0
	:y                      508
	:width                  558
	:height                 60
	:background-color       ""
	:foreground-color       ""
	:menu                   nil
	:horizontal-scrollbar   nil
	:scrollable-width       558
	:vertical-scrollbar     nil
	:scrollable-height      60
	:repaint-proc           nil
	:event-handler          nil
	:events                 ()
	:drawing-model          :xwindows
	:user-data              ()
)
)
