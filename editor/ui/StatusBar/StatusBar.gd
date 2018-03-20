extends ColorRect

func _ready():
	global.messenger.connect("error_info", self, "_show_error_info")
	pass


func _on_scale_down_pressed():
	global.messenger.emit_signal("main_statusbar_click","scale_down")
	pass # replace with function body


func _on_scale_up_pressed():
	global.messenger.emit_signal("main_statusbar_click","scale_up")
	pass # replace with function body


func _on_scale_original_pressed():
	global.messenger.emit_signal("main_statusbar_click","scale_original")
	pass # replace with function body


func _on_scale_auto_pressed():
	global.messenger.emit_signal("main_statusbar_click","scale_auto")
	pass # replace with function body


func _on_display_outlines_pressed():
	global.messenger.emit_signal("main_statusbar_click","display_outlines")
	pass # replace with function body


func _show_error_info(msg):
	pass # replace with function body