#include "texture_packer.h"
#include <core/class_db.h>
#include <core/os/file_access.h>
#include <core/image.h>

TexturePacker::TexturePacker() {
	Ref<RectangleBinPack> bin = memnew(MaxRectsBinPack);
    packer = bin;
    config.instance();
}

Error TexturePacker::pack() {
    pages.clear();

    Ref<PackedPage> ret;
    ret.instance();

    ERR_FAIL_COND_V((packer.is_null()), ERR_INVALID_DATA);
    ERR_FAIL_COND_V((config.is_null()), ERR_INVALID_DATA);
    const PoolStringArray &input_files = config->get_image_files();
    ERR_FAIL_COND_V(input_files.size() == 0, ERR_INVALID_DATA);

    Vector<Ref<Image>> input_images;
    PoolVector2Array input_sizes;
    Map<Image*, String> image_path_map;

    for (int i=0; i < input_files.size(); i++) {
		Ref<Image> img = load_image(input_files[i]);
		if (img.is_valid()) {
            Vector2 size = img->get_size();
            if (size.x * size.y > 0) {
                input_images.push_back(img);
                input_sizes.push_back(size);
                image_path_map[img.ptr()] = input_files[i];
            } else {
                ret->output_regions[input_files[i]] = Rect2();
            }
        } else {
            ret->output_regions[input_files[i]] = Rect2();
        }
    }

	RectPackResult pack_ret = pack_rects(input_sizes);
	const Size2i & target_size = pack_ret.size;
    const Vector<Rect2> & ret_rects = pack_ret.regions;

    // TODO: handle this situation
    ERR_FAIL_COND_V(ret_rects.size() != input_sizes.size() , ERR_BUG);

    Ref<Image> out_image = memnew(Image);
    out_image->create(target_size.x, target_size.y, false, Image::FORMAT_RGBA8);
	out_image->lock();

	for(int i = 0; i < ret_rects.size(); i++) {
        Ref<Image> src_img = input_images[i];
		src_img->lock();
		Size2 src_size = input_sizes[i];
		Rect2 tar_region = ret_rects[i];
		ret->output_regions[image_path_map[src_img.ptr()]] = tar_region;
		if (tar_region.get_area() != src_size.x * src_size.y) {
			WARN_PRINTS(String(tar_region) + " is in invalid from size " + String(src_size));
			ret->output_regions[image_path_map[src_img.ptr()]] = Rect2();
		}
		// 写入目标图片像素
		bool rotated = src_size == Size2(tar_region.size.y, tar_region.size.x);
		for (int x = 0; x < tar_region.size.x; x++) {

			if (rotated && x >= src_img->get_height())
				break;

			for (int y = 0; y < tar_region.size.y; y++) {
				if (rotated) {

					if (y >= src_img->get_width())
						break;

					out_image->set_pixel(
								(tar_region.size.x - x - 1) + tar_region.position.x,
								y + tar_region.position.y,
								src_img->get_pixel(y, x)
					);
				} else {
					out_image->set_pixel(
								x + tar_region.position.x,
								y + tar_region.position.y,
								src_img->get_pixel(x, y)
					);
				}
			}
		}
		src_img->unlock();
    }
	out_image->unlock();

	// 裁剪输出图片
	if(!config->is_squared()) {
		out_image = trim(out_image);
	}

	// 添加输出结果
	ret->out_image = out_image;
	pages.push_back(ret);

    return OK;
}

TexturePacker::RectPackResult TexturePacker::pack_rects(const PoolVector2Array &p_size_arr) {
	// Set filp allowed or not before do anything
	packer->set_flip_allowd(config->get_allow_rotate());

    Array unordered_rects;
    real_t min_area = 0;
    for (int i=0; i < p_size_arr.size(); i++) {
        min_area += p_size_arr[i].x * p_size_arr[i].y;
    }
	Size2 size(ceilf(sqrt(min_area)), ceilf(sqrt(min_area)));
    while (true) {
		packer->initialize(size);
		unordered_rects = packer->insert_array(p_size_arr);
        if(unordered_rects.size() < p_size_arr.size()) {
            size.x += 1;
            size.y += 1;
        } else {
            break;
        }
    }

	Vector<Rect2> rects;
	rects.resize(unordered_rects.size());
	Set<int> found;
	Set<int> used;

	// Try to find the rects that perfect matched
	for(int i=0; i<p_size_arr.size(); i++) {
		Size2 s = p_size_arr[i];
		for(int j = 0; j < unordered_rects.size(); j++) {
			if (used.has(j)) continue;
			Rect2 r = unordered_rects[j];
			if (r.size == s) {
				used.insert(j);
				found.insert(i);
				rects[i] = r;
				break;
			}
		}
	}

	// Try to find the rects that may match or rotated
	for(int i=0; i<p_size_arr.size(); i++) {
		if (found.has(i)) continue;
		Size2 s = p_size_arr[i];
		for(int j = 0; j < unordered_rects.size(); j++) {
			if (used.has(j)) continue;
			Rect2 r = unordered_rects[j];
			if (r.size == Size2(s.y, s.x) || r.size == s) {
				used.insert(j);
				found.insert(i);
				rects[i] = r;
				break;
			}
		}
	}

	RectPackResult ret {size, rects};
	return ret;
}

Ref<Image> TexturePacker::trim(Ref<Image> src_image) {
	ERR_FAIL_COND_V(src_image.is_null(), src_image);
	src_image->lock();
	int left = 0;
	for (int x=0; x < src_image->get_width(); x++) {
		bool all_empty = true;
		for (int y=0; y< src_image->get_height(); y++) {
			if (src_image->get_pixel(x, y).a > 0) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) {
			break;
		}
		left++;
	}

	int right = 0;
	for (int x=src_image->get_width()-1; x >= 0; x--) {
		bool all_empty = true;
		for (int y=0; y< src_image->get_height(); y++) {
			if (src_image->get_pixel(x, y).a > 0) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) {
			break;
		}
		right++;
	}

	int top = 0;
	for (int y=0; y< src_image->get_height(); y++) {
		bool all_empty = true;
		for (int x=0; x < src_image->get_width(); x++) {
			if (src_image->get_pixel(x, y).a > 0) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) {
			break;
		}
		top++;
	}

	int bottom = 0;
	for (int y=src_image->get_height()-1; y >= 0; y--) {
		bool all_empty = true;
		for (int x=0; x< src_image->get_width(); x++) {
			if (src_image->get_pixel(x, y).a > 0) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) {
			break;
		}
		bottom++;
	}
	src_image->unlock();

	ERR_FAIL_COND_V(bottom + top >= src_image->get_height(), src_image);
	ERR_FAIL_COND_V(left + right >= src_image->get_width(), src_image);

	Rect2 region(left, top, src_image->get_width() - right - left, src_image->get_height()- bottom - top);
	if (region.get_area() == 0) {
		return src_image;
	}
	return src_image->get_rect(region);
}

Ref<Image> TexturePacker::load_image(const String &path) {

	Ref<Image> raw_img;
	raw_img.instance();

	ERR_FAIL_COND_V( OK != raw_img->load(path), NULL);

	if (config->get_padding() == 0 && !config->is_trim()) {
		return raw_img;
	}

	Ref<Image> out_img = raw_img;

	if(config->is_trim()) {
		out_img = trim(raw_img);
	}

	// Add padding space to top side and left side
	if(config->get_padding() > 0) {
		Ref<Image> img;
		img.instance();
		img->create(
					config->get_padding() * 2 + out_img->get_width(),
					config->get_padding() * 2 + out_img->get_height(),
					false,
					Image::FORMAT_RGBA8
		);
		img->blit_rect(
					out_img,
					Rect2(Point2(), out_img->get_size()),
					Point2(config->get_padding(), config->get_padding())
		);
		out_img = img;
	}
	return out_img;
}

void TexturePacker::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_packer", "p_packer"), &TexturePacker::set_packer);
    ClassDB::bind_method(D_METHOD("get_packer"), &TexturePacker::get_packer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packer"), "set_packer", "get_packer");

    ClassDB::bind_method(D_METHOD("set_pack_config", "p_config"), &TexturePacker::set_pack_config);
    ClassDB::bind_method(D_METHOD("get_pack_config"), &TexturePacker::get_pack_config);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pack_config"), "set_pack_config", "get_pack_config");

	ClassDB::bind_method(D_METHOD("get_packed_pages"), &TexturePacker::get_packed_pages);

	ClassDB::bind_method(D_METHOD("pack"), &TexturePacker::pack);
	ClassDB::bind_method(D_METHOD("trim", "p_image"), &TexturePacker::trim);
	ClassDB::bind_method(D_METHOD("load_image", "p_path"), &TexturePacker::load_image);
}
