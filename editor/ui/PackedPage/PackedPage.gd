################################################################################
# 合并结果的分页预览组件
# Created by Geequlim on 2018/01/19
################################################################################
extends TextureRect

export(NodePath) var path_selection_view = @"RegionSelection"

# PackedPage 类对象
# 合并结果中的分页
var page = null setget _set_page
# 设置分页数据
func _set_page(p_page):
	page = p_page
	if page != null:
		var tex = ImageTexture.new()
		tex.create_from_image(page.get_image())
		self.texture = tex
		get_node(path_selection_view).regions = page.get_path_region_map()
	else:
		get_node(path_selection_view).regions = {}
		self.texture = load("res://assets/launch_image/launch.png")

onready var global = get_node('/root/global')
func _ready():
	self.page = page
	global.messenger.connect("pack_done", self, "_on_pack_done")

func _on_pack_done(pages):
	global.ui.clear_selection()
	for page in pages:
		self.page = page
