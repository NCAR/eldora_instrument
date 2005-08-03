;GIL-2
(
(
	:type                   :base-window
	:name                   junk
	:owner                  nil
	:width                  360
	:height                 106
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
	:type                   :popup-window
	:name                   StausPopup
	:owner                  junk
	:width                  512
	:height                 349
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:label                  "Status for System 0"
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
	:name                   Ctrl1
	:owner                  StausPopup
	:help                   ""
	:x                      0
	:y                      0
	:width                  512
	:height                 128
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :setting
	:name                   Current
	:owner                  Ctrl1
	:help                   ""
	:x                      108
	:y                      16
	:width                  258
	:height                 23
	:value-x                297
	:value-y                16
	:layout-type            :horizontal
	:foreground-color       ""
	:setting-type           :exclusive
	:rows                   1
	:columns                0
	:label                  "Current Recording system:"
	:label-type             :string
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:choices                ("Yes" "No" )
	:choice-label-types     (:string :string )
	:choice-colors          ("" "" )
	:user-data              ()
)
(
	:type                   :setting
	:name                   Drives
	:owner                  Ctrl1
	:help                   ""
	:x                      136
	:y                      68
	:width                  198
	:height                 23
	:value-x                238
	:value-y                68
	:layout-type            :horizontal
	:foreground-color       ""
	:setting-type           :exclusive
	:rows                   1
	:columns                0
	:label                  "Active Drives:"
	:label-type             :string
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:choices                ("0" "1" "2" "3" )
	:choice-label-types     (:string :string :string :string )
	:choice-colors          ("" "" "" "" )
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Ctrl2
	:owner                  StausPopup
	:help                   ""
	:x                      0
	:y                      128
	:width                  512
	:height                 180
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :text-field
	:name                   Hex
	:owner                  Ctrl2
	:help                   ""
	:x                      52
	:y                      32
	:width                  187
	:height                 15
	:foreground-color       ""
	:text-type              :alphanumeric
	:label                  "Status word:"
	:label-type             :string
	:value-x                143
	:value-y                32
	:layout-type            :horizontal
	:value-length           12
	:stored-length          80
	:read-only              t
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :text-field
	:name                   Failures
	:owner                  Ctrl2
	:help                   ""
	:x                      52
	:y                      68
	:width                  194
	:height                 15
	:foreground-color       ""
	:text-type              :alphanumeric
	:label                  "Soft Failures:"
	:label-type             :string
	:value-x                150
	:value-y                68
	:layout-type            :horizontal
	:value-length           12
	:stored-length          80
	:read-only              t
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :text-field
	:name                   Attempts
	:owner                  Ctrl2
	:help                   ""
	:x                      52
	:y                      112
	:width                  225
	:height                 15
	:foreground-color       ""
	:text-type              :alphanumeric
	:label                  "Attempted Writes:"
	:label-type             :string
	:value-x                181
	:value-y                112
	:layout-type            :horizontal
	:value-length           12
	:stored-length          80
	:read-only              t
	:notify-handler         nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :control-area
	:name                   Ctrl3
	:owner                  StausPopup
	:help                   ""
	:x                      0
	:y                      308
	:width                  512
	:height                 41
	:background-color       "Aquamarine"
	:foreground-color       "Black"
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Clear
	:owner                  Ctrl3
	:help                   ""
	:x                      208
	:y                      12
	:constant-width         nil
	:button-type            :normal
	:width                  92
	:height                 19
	:foreground-color       ""
	:label                  "Clear Status"
	:label-type             :string
	:menu                   nil
	:notify-handler         ClearHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
(
	:type                   :button
	:name                   Close
	:owner                  Ctrl3
	:help                   ""
	:x                      452
	:y                      12
	:constant-width         nil
	:button-type            :normal
	:width                  51
	:height                 19
	:foreground-color       ""
	:label                  "Close"
	:label-type             :string
	:menu                   nil
	:notify-handler         CloseHandler
	:event-handler          nil
	:events                 ()
	:user-data              ()
)
)
