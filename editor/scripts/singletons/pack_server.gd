################################################################################
# 图片合并服务，在多线程中执行合并任务队列
# Created by Geequlim on 2018/01/19
################################################################################

var running = false					# 启动标记
var thread = Thread.new()			# 线程对象
var pack_command_queue = []			# 合图命令队列
var global_ref = null				# global引用


func _init(p_global):
	global_ref = weakref(p_global)

# 添加合并命令到队列中  
# - - - - - - - - - -  
# *Parameters*  
# * [config: TexturePackerConfig] 合图配置  
func request_pack(config):
	if typeof(config) == TYPE_OBJECT and config is TexturePackerConfig:
		var mutex = Mutex.new()
		mutex.lock()
		pack_command_queue.push_back(config.duplicate())
		mutex.unlock()

# 启动合图服务
func start():
	running = true
	if not thread.is_active():
		thread.start(self, "main")

# 停止合图服务
func stop():
	running = false
	if thread.is_active():
		thread.wait_to_finish()

# 一次合图完成时回调（在主线程中被调用）
func _on_pack_done(pages):
	if not pages.empty():
		global.messenger.emit_signal("pack_done", pages)

# 合图线程循环  
func main(useless=null):
	while running:
		var mutex = Mutex.new()
		mutex.lock()
		if not pack_command_queue.empty():
			var config = pack_command_queue[0]
			pack_command_queue.pop_front()
			mutex.unlock()
			var tp = TexturePacker.new()
			tp.pack_config = config
			if OK == tp.pack():
				var pages = tp.get_packed_pages()
				call_deferred("_on_pack_done", pages)
		OS.delay_msec(50)
