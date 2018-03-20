################################################################################
# 选中图片区域展示、合图交互组件
# Created by Geequlim on 2018/01/19
################################################################################

extends Control
# 选择的背景颜色
const SELECTION_COLOR = Color(0, 1, 0, 0.35)

onready var global = get_node('/root/global')

# 合图的经理区域表
var regions = {} setget _set_regions
func _set_regions(p_regions):
	regions = p_regions
	update()

func _ready():
	self.regions = regions
	global.messenger.connect("selection_changed", self, "_on_selection_changed")

# 选择项改变时重新绘制
func _on_selection_changed():
	update()

# 绘制选中矩形背景
func _draw():
	for path in global.ui.selected_images:
		if path in regions:
			var rect = regions[path]
			draw_rect(rect, SELECTION_COLOR)

# 获取鼠标位置下的图片名称
func _get_path_under_pos():
	var pos = get_local_mouse_position()
	for path in regions:
		var rect = regions[path]
		if rect.has_point(pos):
			return path
	return null

# 处理鼠标交互
func _gui_input(event):
	if event is InputEventMouseButton:
		if event.pressed and event.button_index == BUTTON_LEFT:
			var path = _get_path_under_pos()
			if path != null:
				if event.shift or event.control:
					if path in global.ui.selected_images:
						global.ui.remove_selected_image(path)
					else:
						global.ui.add_selected_image(path)
				else:
					global.ui.set_selected_images([path])
			elif not (event.shift or event.control):
				global.ui.clear_selection()
			

