extends Control
onready var global = get_node('/root/global')

export(NodePath) var path_max_x = '布局/属性/Max Size/宽'
export(NodePath) var path_max_y = '布局/属性/Max Size/高'
export(NodePath) var path_image_path = '布局/属性/Image File/LineEdit'
export(NodePath) var path_meta_path = '布局/属性/Data File/LineEdit'
export(NodePath) var path_output_dir = '布局/属性/Output Folder/LineEdit'
export(NodePath) var path_padding = '布局/属性/Padding/SpinBox'
export(NodePath) var path_trim = '布局/属性/Trim/CheckButton'
export(NodePath) var path_squared = '布局/属性/Squared/CheckButton'
export(NodePath) var path_rotate = '布局/属性/Allow Rotate/CheckButton'
export(NodePath) var path_format_option = '布局/属性/Output Format/OptionButton'
onready var format_option = get_node(path_format_option)

var output_folder = OS.get_system_dir(OS.SYSTEM_DIR_DOCUMENTS)
var image_file = "sprite_sheet"
var meta_file = "sprite_sheet"
var file_dialog = NativeFileDialog.new()

var Formats = [
	{
		"title": "Generic XML",
		"exporter": GenericXMLExporter.new(),
	},
	# TODO: Add new exporters here
]

func _ready():
	get_node(path_max_x).value = global.config.max_size.x
	get_node(path_max_y).value = global.config.max_size.y
	get_node(path_image_path).text = image_file
	get_node(path_meta_path).text = meta_file
	get_node(path_output_dir).text = output_folder
	get_node(path_padding).value = global.config.padding
	# get_node(path_trim).pressed = global.config.trim
	# get_node(path_squared).pressed = global.config.squared
	# get_node(path_rotate).pressed = global.config.allow_rotate

	var index = 0
	for fmt in  Formats:
		format_option.add_item(fmt.title, index)
		index += 1
	_on_format_selected(0)
	# output dir selector
	file_dialog.mode = NativeFileDialog.MODE_SELECT_DIR
	file_dialog.connect('confirmed', self, 'on_dirs_selected')

func _on_out_image_changed(new_text):
	image_file = new_text
	_update_config_path()

func _on_meta_file_changed(new_text):
	meta_file = new_text
	_update_config_path()

func _on_output_dir_changed(new_text):
	output_folder = new_text
	_update_config_path()

func _update_config_path():
	global.config.save_meta = str(output_folder, '/', meta_file)
	global.config.save_image = str(output_folder, '/', image_file)

func _on_padding_value_changed(value):
	global.config.padding = int(value)
	global.config.notify_change()

func _on_max_w_value_changed(value):
	global.config.max_size.x = int(value)
	global.config.notify_change()


func _on_max_h_value_changed(value):
	global.config.max_size.y = int(value)
	global.config.notify_change()


func _on_trim_toggled(button_pressed):
	global.config.trim = button_pressed
	global.config.notify_change()


func _on_squared_toggled(button_pressed):
	global.config.squared = button_pressed
	global.config.notify_change()


func _on_allow_rotate_toggled(button_pressed):
	global.config.allow_rotate = button_pressed
	global.config.notify_change()

func _on_format_selected(ID):
	global.set_exporter(Formats[ID].exporter)

func _on_select_output_dir():
	file_dialog.show()

func on_dirs_selected():
	var dirs = file_dialog.get_selected_pathes()
	if dirs.size() > 0:
		var dir = dirs[0]
		get_node(path_output_dir).text = dir
