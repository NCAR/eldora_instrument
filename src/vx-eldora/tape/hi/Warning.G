;GIL-2
(
(
	:type                   :base-window
	:name                   window1
	:owner                  nil
	:width                  400
	:height                 150
	:background-color       ""
	:foreground-color       ""
	:label                  "Base Window"
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
	:type                   :popup-window
	:name                   WarningPopup
	:owner                  window1
	:width                  400
	:height                 199
	:background-color       ""
	:foreground-color       "Black"
	:label                  "Warning!!!!!!"
	:label-type             :string
	:mapped                 nil
	:show-footer            t
	:resizable              t
	:pinned                 nil
	:done-handler           nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Ctrl
	:owner                  WarningPopup
	:help                   ""
	:x                      0
	:y                      0
	:width                  400
	:height                 199
	:background-color       "Aquamarine"
	:foreground-color       ""
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   Warn1
	:owner                  Ctrl
	:help                   ""
	:x                      156
	:y                      48
	:width                  65
	:height                 13
	:foreground-color       ""
	:label                  "Warning! "
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   Warn2
	:owner                  Ctrl
	:help                   ""
	:x                      72
	:y                      84
	:width                  260
	:height                 13
	:foreground-color       ""
	:label                  "Soft Errors exceeded  either maximum"
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :message
	:name                   Warn3
	:owner                  Ctrl
	:help                   ""
	:x                      124
	:y                      120
	:width                  132
	:height                 13
	:foreground-color       ""
	:label                  "limit or burst limit."
	:label-type             :string
	:label-bold             t
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Close
	:owner                  Ctrl
	:help                   ""
	:x                      144
	:y                      152
	:constant-width         nil
	:button-type            :normal
	:width                  100
	:height                 19
	:foreground-color       ""
	:label                  "Aknowledged"
	:label-type             :string
	:menu                   nil
	:notify-handler         CloseHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
)
