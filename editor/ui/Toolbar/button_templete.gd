extends Button

export (NodePath) var button_text

var text_width
func _ready():
	text_width = get_node(button_text).rect_size.x
	rect_min_size.x = text_width + 20
	pass
