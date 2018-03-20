#ifndef TEXTURE_PACKER_H
#define TEXTURE_PACKER_H
#include <core/reference.h>
#include <core/image.h>
#include "../RectangleBinPack/rect_bin_packer.hpp"
#include "texture_packer_config.hpp"

/**
 * @brief The PackedPage class
 *  合并结果分页
 */
class PackedPage : public Reference {
	GDCLASS(PackedPage, Reference);
	friend class TexturePacker;
public:
	PackedPage() {}
	Ref<Image> get_image() const { return out_image; }
	Dictionary get_path_region_map() const { return output_regions; }
protected:
	Ref<Image> out_image;
	Dictionary output_regions;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("get_image"), &PackedPage::get_image);
		ClassDB::bind_method(D_METHOD("get_path_region_map"), &PackedPage::get_path_region_map);
	}
};

/**
 * @brief The TexturePacker class
 * 合图打包器
 */
class TexturePacker : public Reference {
    GDCLASS(TexturePacker, Reference);

    struct RectPackResult {
        Size2i size;
        Vector<Rect2> regions;
    };

public:
    TexturePacker();

    void set_packer(const Ref<RectangleBinPack> &p_packer) { packer = p_packer; }
    Ref<RectangleBinPack> get_packer() const { return packer; }

    void set_pack_config(const Ref<TexturePackerConfig> &p_config) { config = p_config; }
    Ref<TexturePackerConfig> get_pack_config() const { return config; }

    Array get_packed_pages() const {
        Array arr;
        arr.resize(pages.size());
        for (int i=0; i<pages.size(); i++) {
            arr[i] = pages[i];
        }
        return arr;
    }

	Error pack();
	Ref<Image> trim(Ref<Image> src_image);
	Ref<Image> load_image(const String& path);
protected:
	RectPackResult pack_rects(const PoolVector2Array &p_size_arr);
    static void _bind_methods();
    Ref<RectangleBinPack> packer;
    Ref<TexturePackerConfig> config;
    Vector<Ref<PackedPage> > pages;
};

#endif // TEXTURE_PACKER_H
