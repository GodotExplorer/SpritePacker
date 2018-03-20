################################################################################
# 项目合图配置
# Created by Geequlim on 2018/01/19
################################################################################
extends TexturePackerConfig
const _SLOT_META_NAME   = "slots"           # 有序信号槽meta名称，使用meta避免被序列化
const _MUTE_META_NAME   = "muted"           # 闭嘴模式meta名称，使用meta避免被序列化
const _CHANGE_META_NAME = "changed"         # 修改标记meta名称，使用meta避免被序列化

func _init():
    set_meta(_SLOT_META_NAME, [])
    set_meta(_MUTE_META_NAME, false)
    set_meta(_CHANGE_META_NAME, false)

# 检查是否修改
func is_changed():
    return get_meta(_CHANGE_META_NAME)

# 设为未修改状态
func discard_change():
    set_meta(_CHANGE_META_NAME, false)

# 通知修改发生
func notify_change():
    if not is_muted():
        set_meta(_CHANGE_META_NAME, true)
        for s in get_slots():
            s.execute()

# 检查是否处于闭嘴模式
func is_muted():
    return has_meta(_MUTE_META_NAME) and get_meta(_MUTE_META_NAME)

# 设置闭嘴模式
func mute(muted = true):
    set_meta(_MUTE_META_NAME, muted)

func get_slots():
    return get_meta(_SLOT_META_NAME)

func get_slot(object, callback):
    for s in get_slots():
        if s.instance_ptr.get_ref() == object and s.callback == callback:
            return s
    return null

func add_slot(object, callback, arguments=[], push_front=false):
    if get_slot(object, callback) == null:
        if typeof(object) == TYPE_OBJECT and typeof(callback) == TYPE_STRING and object.has_method(callback):
            if push_front:
                get_slots().push_front(Slot.new(object, callback, arguments))
            else:
                get_slots().append(Slot.new(object, callback, arguments))

func remove_slot(object, callback):
    var s = get_slot(object, callback)
    if s != null:
        get_slots().erase(s)

# 添加修改状态观察者
func add_observer(object, callback, arguments = [], push_front=false):
    add_slot(object, callback, arguments, push_front)

# 删除修改状态观察者
func remove_observer(object, callback):
    remove_slot(object, callback)

# 信号槽
class Slot:
    var instance_ptr = null
    var callback     = null
    var arguments    = []
    func _init(p_obj, p_cb, p_args):
        instance_ptr = weakref(p_obj)
        callback     = p_cb
        arguments    = p_args
    func execute():
        var instance = instance_ptr.get_ref()
        if instance != null:
            instance.callv(callback, arguments)
    