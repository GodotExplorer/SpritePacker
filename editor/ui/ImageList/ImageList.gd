################################################################################
# 输入图片文件列表
# Created by Geequlim on 2018/01/19
################################################################################
extends VBoxContainer

# 标准分辨率下的图标尺寸
const ICON_SIZE = Vector2(32, 32)

export(NodePath) var path_list = @"图片列表"
export(NodePath) var path_btn_add = @"操作按钮/添加"
export(NodePath) var path_btn_remove = @"操作按钮/删除"
onready var list = get_node(path_list)
onready var global = get_node('/root/global')

var file_dialog = NativeFileDialog.new()

func _ready():
	file_dialog.mode = NativeFileDialog.MODE_SELECT_FILE | NativeFileDialog.MODE_MULTI_SELECT
	file_dialog.add_filter("图像文件", PoolStringArray(global.ACCEPT_EXTENTIONS).join(";"))
	file_dialog.title = "添加图片"
	file_dialog.connect("confirmed", self, "_on_files_selected")

	list.connect("selection_changed", self, "_on_selection_changed")
	list.provider = ItemProvider.new()
	list.data_source = global.config.image_files
	list.set_fixed_icon_size(ICON_SIZE)
	global.config.add_slot(self, "update_list")
	global.messenger.connect("selection_changed", self, "_on_selection_changed_form_others")
	get_node(path_btn_add).connect("pressed", self, "_on_action_button", [path_btn_add])
	get_node(path_btn_remove).connect("pressed", self, "_on_action_button", [path_btn_remove])

	global.messenger.connect("main_toolbar_click", self, "_on_toolbar_click")
	global.messenger.connect("main_menu_click", self, "_on_mainmenu_click")
# 刷新列表
func update_list():
	list.data_source = global.config.image_files

# 菜单栏点击事件响应
# - - - - - - - - - -  
# *Parameters*  
# * [menu_item: String] 菜单路径，标记是哪个菜单被按下
func _on_mainmenu_click(menu_item):
	print(menu_item)
	if menu_item == "Edit/Add sprites...":
		_on_action_button(path_btn_add)
	elif menu_item == "Edit/Remove sprites":
		_on_action_button(path_btn_remove) 

# 工具栏点击事件响应
# - - - - - - - - - -  
# *Parameters*  
# * [button: String] 按钮动作，标记是哪个按钮被按下
func _on_toolbar_click(button):
	if button == "add_sprites":
		_on_action_button(path_btn_add)
	elif button == "remove_sprites":
		_on_action_button(path_btn_remove)

# 操作按钮回调  
# - - - - - - - - - -  
# *Parameters*  
# * [which: NodePath] 按钮路径，标记是哪个按钮被按下  
func _on_action_button(which):
	# 添加图片
	if which == path_btn_add:
		file_dialog.show()
	# 移除选中图片
	elif which == path_btn_remove:
		var file_changed = false
		var files = Array(global.config.image_files)
		for f in list.get_selected_item_list():
			files.erase(f)
			file_changed = true
		if file_changed:
			global.config.image_files = files
			global.config.notify_change()

# 导入图片文件  
func _on_files_selected():
	global.append_files(file_dialog.get_selected_pathes())

# 选中图片列表中的条目
var _update_from_self = false
func _on_selection_changed(selected_items):
	_update_from_self = true
	global.ui.set_selected_images(selected_items)
	yield(get_tree(), "idle_frame")
	_update_from_self = false

# 其他地方选择了图片，更新列表选中项
func _on_selection_changed_form_others():
	if not _update_from_self:
		list.selecte_items(global.ui.selected_images)

# 列表项工厂
class ItemProvider extends "res://addons/gdutils/scene/gui/ItemListEnhanced.gd".ListItemProvider:
	func item_fit_filter(p_item, p_filter):
		return p_filter == null or p_filter.is_subsequence_ofi(p_item)

	func get_item_text(p_item):
		return p_item.get_file()

	func get_item_icon(p_item):
		return global.gdutils.resource.image.load_image_texture(p_item)
