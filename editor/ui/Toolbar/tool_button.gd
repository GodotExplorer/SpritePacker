################################################################################
# 工具栏按钮
# Created by Geequlim on 2018/01/21
################################################################################
tool
extends Button

export(NodePath) var path_icon = @"边距/容器/图标"
export(NodePath) var path_label = @"边距/容器/标签"
export(NodePath) var path_container = @"边距"

export(String) var label = "Tool Button Tool Button Tool Button" setget set_text
func set_text(p_text):
	label = p_text
	if is_inside_tree():
		get_node(path_label).text = p_text
		yield(get_tree().create_timer(0.2), "timeout")
		var size = get_node(path_container).rect_size
		self.rect_min_size = size
		self.rect_size = size
		
export(Texture) var icon_texture = preload("res://icon.png") setget set_icon
func set_icon(p_icon):
	icon_texture = p_icon
	if is_inside_tree():
		get_node(path_icon).texture = p_icon

func _ready():
	self.icon_texture = icon_texture
	self.label = label
