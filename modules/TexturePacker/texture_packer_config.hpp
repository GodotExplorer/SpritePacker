#ifndef TEXTURE_PACKER_CONFIG_HPP
#define TEXTURE_PACKER_CONFIG_HPP
#include <core/resource.h>
#include <core/class_db.h>
#include <core/math/math_2d.h>
#include <core/image.h>

class TexturePackerConfig : public Resource {
	GDCLASS(TexturePackerConfig, Resource);
public:
	TexturePackerConfig() {
		trim = false;
		padding = 0;
		squared = false;
		max_size = Vector2(4096, 4096);
		save_image = "out";
		save_meta = "out";
		save_format = Image::FORMAT_BPTC_RGBA;
	}

	int get_padding() const { return padding; }
	void set_padding(const int &p_padding) { padding = p_padding; }

	bool is_trim() const { return trim; }
	void set_trim(const bool &p_trim) { trim = p_trim; }

	Vector2 get_max_size() const { return max_size; }
	void set_max_size(const Vector2 &p_max_size) { max_size = p_max_size; }

	bool is_squared() const { return squared; }
	void set_squared(const bool &p_squared) { squared = p_squared; }

	PoolStringArray get_image_files() const { return image_files; }
	void set_image_files(const PoolStringArray &p_image_files) { image_files = p_image_files; }

	String get_save_image() const { return save_image; }
	void set_save_image(const String &p_save_image) { save_image = p_save_image; }

	String get_save_meta() const { return save_meta; }
	void set_save_meta(const String &p_save_meta) { save_meta = p_save_meta; }

	bool get_allow_rotate() const { return allow_rotate; }
	void set_allow_rotate(const bool &p_allow_rotate) { allow_rotate = p_allow_rotate; }


	Image::Format get_save_format() const { return save_format; }
	void set_save_format(Image::Format p_save_format) { save_format = p_save_format; }

protected:
	bool squared;
	bool trim;
	int padding;
	bool allow_rotate;
	Vector2 max_size;
	PoolStringArray image_files;
	String save_image;
	String save_meta;
	Image::Format save_format;
protected:
	static void _bind_methods() {

		ClassDB::bind_method(D_METHOD("set_padding", "p_padding"), &TexturePackerConfig::set_padding);
		ClassDB::bind_method(D_METHOD("get_padding"), &TexturePackerConfig::get_padding);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "padding"), "set_padding", "get_padding");

		ClassDB::bind_method(D_METHOD("set_trim", "p_trim"), &TexturePackerConfig::set_trim);
		ClassDB::bind_method(D_METHOD("is_trim"), &TexturePackerConfig::is_trim);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "trim"), "set_trim", "is_trim");

		ClassDB::bind_method(D_METHOD("set_squared", "p_squared"), &TexturePackerConfig::set_squared);
		ClassDB::bind_method(D_METHOD("is_squared"), &TexturePackerConfig::is_squared);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "squared"), "set_squared", "is_squared");

		ClassDB::bind_method(D_METHOD("set_allow_rotate", "p_allow_rotate"), &TexturePackerConfig::set_allow_rotate);
		ClassDB::bind_method(D_METHOD("get_allow_rotate"), &TexturePackerConfig::get_allow_rotate);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_rotate"), "set_allow_rotate", "get_allow_rotate");

		ClassDB::bind_method(D_METHOD("set_max_size", "p_max_size"), &TexturePackerConfig::set_max_size);
		ClassDB::bind_method(D_METHOD("get_max_size"), &TexturePackerConfig::get_max_size);
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "max_size"), "set_max_size", "get_max_size");

		ClassDB::bind_method(D_METHOD("set_image_files", "p_image_files"), &TexturePackerConfig::set_image_files);
		ClassDB::bind_method(D_METHOD("get_image_files"), &TexturePackerConfig::get_image_files);
		ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "image_files"), "set_image_files", "get_image_files");

		ClassDB::bind_method(D_METHOD("set_save_image", "p_save_image"), &TexturePackerConfig::set_save_image);
		ClassDB::bind_method(D_METHOD("get_save_image"), &TexturePackerConfig::get_save_image);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "save_image"), "set_save_image", "get_save_image");

		ClassDB::bind_method(D_METHOD("set_save_meta", "p_save_meta"), &TexturePackerConfig::set_save_meta);
		ClassDB::bind_method(D_METHOD("get_save_meta"), &TexturePackerConfig::get_save_meta);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "save_meta"), "set_save_meta", "get_save_meta");

		ClassDB::bind_method(D_METHOD("set_save_format", "p_save_format"), &TexturePackerConfig::set_save_format);
		ClassDB::bind_method(D_METHOD("get_save_format"), &TexturePackerConfig::get_save_format);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "save_format"), "set_save_format", "get_save_format");

	}
};

#endif // TEXTURE_PACKER_CONFIG_HPP
