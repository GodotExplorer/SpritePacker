extends HBoxContainer

onready var global = get_node("/root/global")

func _ready():
	
	pass


func _on_new_project_pressed():
	global.messenger.emit_signal("main_toolbar_click","new_project")
	pass # replace with function body


func _on_open_project_pressed():
	pass # replace with function body


func _on_save_project_pressed():
	pass # replace with function body


func _on_add_sprites_pressed():
	global.messenger.emit_signal("main_toolbar_click","add_sprites")
	pass # replace with function body


func _on_remove_sprites_pressed():
	global.messenger.emit_signal("main_toolbar_click","remove_sprites")
	pass # replace with function body


func _on_add_smart_folder_pressed():
	pass # replace with function body


func _on_sprite_settings_pressed():
	pass # replace with function body


func _on_anim_preview_pressed():
	pass # replace with function body


func _on_publish_sprite_sheet_pressed():
	global.export_pages()
