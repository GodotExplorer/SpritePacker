################################################################################
# UI交互与状态管理对象
# Created by Geequlim on 2018/01/19
################################################################################

var global_ref = null						# global引用
var selected_images = []					# 选中的图片列表
var theme = null							# GUI主题
var main_menu = null						#注册主菜单

func _init(p_global):
	global_ref = weakref(p_global)

# 获取messenger
func get_messenger():
	return global_ref.get_ref().messenger

# 清空选择项
func clear_selection():
	selected_images.clear()
	get_messenger().emit_signal("selection_changed")

# 设置选中项
func set_selected_images(images):
	selected_images = images
	get_messenger().emit_signal("selection_changed")

# 添加选择项
func add_selected_image(image):
	if not image in selected_images:
		selected_images.append(image)
		get_messenger().emit_signal("selection_changed")

# 移除选中项
func remove_selected_image(image):
	if image in selected_images:
		selected_images.erase(image)
	get_messenger().emit_signal("selection_changed")

# 设置主题
func set_theme(p_theme):
	theme = p_theme
	yield(global_ref.get_ref().get_tree(), "idle_frame")
	var scene = global_ref.get_ref().get_tree().get_current_scene()
	if scene != null and scene is Control:
		scene.set_theme(p_theme)

# 更新主题显示
func refresh_theme():
	set_theme(theme)
