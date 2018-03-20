################################################################################
# 主菜单逻辑脚本
# Created by Sersoong on 2018/01/20
################################################################################
extends HBoxContainer
#json文件的位置
export var main_menu_json = "res://assets/json/ui_main_menu.json"
onready var global = get_node("/root/global")
onready var menu_json = global.gdutils.utils.json.load_json(main_menu_json)
var menu = {}

func _ready():
	global.ui.main_menu = self
	for item in menu_json:
		if item.has("title"):
			print()
			menu[item.title] = global.gdutils.scene.gui.Menu.new(item.title)
			menu[item.title].parse_dictionary(item)
			var menu_button = MenuButton.new()
			menu_button.name = item.title
			menu_button.text = item.title
			menu_button.get_popup().name = item.title
			menu[item.title].render_to_menu_button(menu_button)
			menu[item.title].connect("item_pressed",self,"_on_menu_item_pressed")
			add_child(menu_button)
			menu[item.title]
	pass

func _on_menu_item_pressed(item):
	print("----->",item.get_path())
	global.messenger.emit_signal("main_menu_click",item.get_path())