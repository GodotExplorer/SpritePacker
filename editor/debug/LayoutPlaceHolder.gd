################################################################################
# 布局占位符
# Created by Geequlim on 2018/01/18
################################################################################
tool
extends ColorRect

func _ready():
	self.size_flags_vertical = SIZE_EXPAND_FILL
	self.size_flags_horizontal = SIZE_EXPAND_FILL
	$Control/Label.text = get_parent().get_name()
