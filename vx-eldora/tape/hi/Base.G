;GIL-2
(
(
	:type                   :base-window
	:name                   MainWdw
	:owner                  nil
	:width                  563
	:height                 438
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:label                  "Recording  Control"
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
	:name                   Ctrl1
	:owner                  MainWdw
	:help                   ""
	:x                      0
	:y                      0
	:width                  563
	:height                 144
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            t
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   Message1
	:owner                  Ctrl1
	:help                   ""
	:x                      116
	:y                      32
	:width                  310
	:height                 13
	:foreground-color       ""
	:label                  "View the current status for recording system:"
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Zero
	:owner                  Ctrl1
	:help                   ""
	:x                      172
	:y                      68
	:constant-width         nil
	:button-type            :normal
	:width                  50
	:height                 19
	:foreground-color       ""
	:label                  "   0   "
	:label-type             :string
	:menu                   nil
	:notify-handler         ZeroHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   One
	:owner                  Ctrl1
	:help                   ""
	:x                      296
	:y                      68
	:constant-width         nil
	:button-type            :normal
	:width                  50
	:height                 19
	:foreground-color       ""
	:label                  "   1   "
	:label-type             :string
	:menu                   nil
	:notify-handler         OneHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   Active
	:owner                  Ctrl1
	:help                   ""
	:x                      144
	:y                      112
	:width                  247
	:height                 13
	:foreground-color       ""
	:label                  "Currently active system is system 0"
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Ctrl2
	:owner                  MainWdw
	:help                   ""
	:x                      0
	:y                      144
	:width                  563
	:height                 136
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            t
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   RecordOn
	:owner                  Ctrl2
	:help                   ""
	:x                      164
	:y                      24
	:constant-width         nil
	:button-type            :normal
	:width                  100
	:height                 19
	:foreground-color       "Black"
	:label                  "Recording On"
	:label-type             :string
	:menu                   nil
	:notify-handler         RecordOnHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   RecordOff
	:owner                  Ctrl2
	:help                   ""
	:x                      276
	:y                      24
	:constant-width         nil
	:button-type            :normal
	:width                  99
	:height                 19
	:foreground-color       "Red"
	:label                  "Recording off"
	:label-type             :string
	:menu                   nil
	:notify-handler         RecordOffHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Log
	:owner                  Ctrl2
	:help                   ""
	:x                      72
	:y                      72
	:constant-width         nil
	:button-type            :normal
	:width                  105
	:height                 19
	:foreground-color       ""
	:label                  "Record log file"
	:label-type             :string
	:menu                   nil
	:notify-handler         LogHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Rewind
	:owner                  Ctrl2
	:help                   ""
	:x                      244
	:y                      72
	:constant-width         nil
	:button-type            :normal
	:width                  64
	:height                 19
	:foreground-color       ""
	:label                  "Rewind"
	:label-type             :string
	:menu                   nil
	:notify-handler         RewindHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Eject
	:owner                  Ctrl2
	:help                   ""
	:x                      404
	:y                      72
	:constant-width         nil
	:button-type            :normal
	:width                  47
	:height                 19
	:foreground-color       ""
	:label                  "Eject"
	:label-type             :string
	:menu                   nil
	:notify-handler         EjectHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Ctrl3
	:owner                  MainWdw
	:help                   ""
	:x                      0
	:y                      280
	:width                  563
	:height                 158
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            t
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :gauge
	:name                   Meter
	:owner                  Ctrl3
	:help                   ""
	:x                      152
	:y                      20
	:width                  224
	:height                 45
	:value-x                152
	:value-y                37
	:slider-width           200
	:foreground-color       ""
	:label                  "Percent of recording media used:"
	:label-type             :string
	:layout-type            :vertical
	:orientation            :horizontal
	:show-range             t
	:min-value              0
	:max-value              100
	:ticks                  10
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Quit
	:owner                  Ctrl3
	:help                   ""
	:x                      232
	:y                      116
	:constant-width         nil
	:button-type            :normal
	:width                  45
	:height                 19
	:foreground-color       ""
	:label                  "Quit"
	:label-type             :string
	:menu                   nil
	:notify-handler         QuitHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
)
