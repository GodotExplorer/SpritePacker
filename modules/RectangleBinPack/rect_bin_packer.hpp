#ifndef RECT_BIN_PACKER_HPP
#define RECT_BIN_PACKER_HPP

#include <core/reference.h>
#include <core/math/math_2d.h>

#include "MaxRectsBinPack.h"
#include "GuillotineBinPack.h"
#include "ShelfBinPack.h"
#include "ShelfNextFitBinPack.h"
#include "SkylineBinPack.h"

class RectangleBinPack : public Reference {
    GDCLASS(RectangleBinPack, Reference);
public:
    virtual void initialize(const Size2 &p_size) = 0;
    virtual Rect2 insert(const Size2 &p_rect_size) = 0;
    virtual Array insert_array(const PoolVector2Array& p_rect_size) = 0;
    virtual real_t occupancy() const = 0;
	virtual void set_flip_allowd(bool p_allowd) = 0;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("initialize", "p_size"), &RectangleBinPack::initialize);
		ClassDB::bind_method(D_METHOD("insert", "p_rect_size"), &RectangleBinPack::insert);
		ClassDB::bind_method(D_METHOD("insert_array", "p_rect_size_arr"), &RectangleBinPack::insert_array);
		ClassDB::bind_method(D_METHOD("occupancy"), &RectangleBinPack::occupancy);
		ClassDB::bind_method(D_METHOD("set_flip_allowd"), &RectangleBinPack::set_flip_allowd);
	}
};

class MaxRectsBinPack : public RectangleBinPack {
	GDCLASS(MaxRectsBinPack, RectangleBinPack);
public:
	/// Specifies the different heuristic rules that can be used when deciding where to place a new rectangle.
	enum FreeRectChoiceHeuristic
	{
		RectBestShortSideFit, ///< -BSSF: Positions the rectangle against the short side of a free rectangle into which it fits the best.
		RectBestLongSideFit, ///< -BLSF: Positions the rectangle against the long side of a free rectangle into which it fits the best.
		RectBestAreaFit, ///< -BAF: Positions the rectangle into the smallest free rect into which it fits.
		RectBottomLeftRule, ///< -BL: Does the Tetris placement.
		RectContactPointRule ///< -CP: Choosest the placement where the rectangle touches other rects as much as possible.
	};

	virtual void initialize(const Size2 &p_size) { bin.Init(p_size.x, p_size.y); }

	virtual void set_flip_allowd(bool p_allowd) { bin.SetFlipAllowed(p_allowd); }

    virtual Rect2 insert(const Size2 &p_rect_size) {
        auto rect = bin.Insert(
                        p_rect_size.x,
                        p_rect_size.y,
                        (rbp::MaxRectsBinPack::FreeRectChoiceHeuristic)choice_method
        );
        return Rect2(rect.x, rect.y, rect.width, rect.height);
    }

    virtual Array insert_array(const PoolVector2Array& p_rect_size) {

        std::vector<rbp::Rect> dst;
        std::vector<rbp::RectSize> sizes(p_rect_size.size());
        for(int i= 0; i<p_rect_size.size(); i++) {
            sizes[i] = rbp::RectSize {int(p_rect_size[i].x), int(p_rect_size[i].y)};
        }
        bin.Insert(
                    sizes,
                    dst,
                    (rbp::MaxRectsBinPack::FreeRectChoiceHeuristic)choice_method
        );

        Array arr;
        arr.resize(dst.size());
        for (int i=0; i<dst.size(); i++) {
            arr[i] = Rect2(dst[i].x, dst[i].y, dst[i].width, dst[i].height);
        }
        return arr;
    }

    virtual real_t occupancy() const { return bin.Occupancy(); }

    FreeRectChoiceHeuristic get_choice_method() const { return choice_method; }
    void set_choice_method(FreeRectChoiceHeuristic method) { choice_method = method;}

protected:
	static void _bind_methods() {

		ClassDB::bind_method(D_METHOD("set_choice_method", "p_methid"), &MaxRectsBinPack::set_choice_method);
		ClassDB::bind_method(D_METHOD("get_choice_method"), &MaxRectsBinPack::get_choice_method);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "choice_method"), "set_choice_method", "get_choice_method");

		BIND_ENUM_CONSTANT(RectBestShortSideFit);
		BIND_ENUM_CONSTANT(RectBestLongSideFit);
		BIND_ENUM_CONSTANT(RectBestAreaFit);
		BIND_ENUM_CONSTANT(RectBottomLeftRule);
		BIND_ENUM_CONSTANT(RectContactPointRule);
	}

private:
    rbp::MaxRectsBinPack bin;
    FreeRectChoiceHeuristic choice_method = RectBestShortSideFit;
};
VARIANT_ENUM_CAST(MaxRectsBinPack::FreeRectChoiceHeuristic);


class GuillotineBinPack : public RectangleBinPack {
	GDCLASS(GuillotineBinPack, RectangleBinPack);
public:
	/// Specifies the different choice heuristics that can be used when deciding which of the free subrectangles
	/// to place the to-be-packed rectangle into.
	enum FreeRectChoiceHeuristic
	{
		RectBestAreaFit, ///< -BAF
		RectBestShortSideFit, ///< -BSSF
		RectBestLongSideFit, ///< -BLSF
		RectWorstAreaFit, ///< -WAF
		RectWorstShortSideFit, ///< -WSSF
		RectWorstLongSideFit ///< -WLSF
	};

	/// Specifies the different choice heuristics that can be used when the packer needs to decide whether to
	/// subdivide the remaining free space in horizontal or vertical direction.
	enum GuillotineSplitHeuristic
	{
		SplitShorterLeftoverAxis, ///< -SLAS
		SplitLongerLeftoverAxis, ///< -LLAS
		SplitMinimizeArea, ///< -MINAS, Try to make a single big rectangle at the expense of making the other small.
		SplitMaximizeArea, ///< -MAXAS, Try to make both remaining rectangles as even-sized as possible.
		SplitShorterAxis, ///< -SAS
		SplitLongerAxis ///< -LAS
	};

	virtual void initialize(const Size2 &p_size) { bin.Init(p_size.x, p_size.y); }
	virtual void set_flip_allowd(bool p_allowd) { bin.SetFlipAllowed(p_allowd); }

    virtual Rect2 insert(const Size2 &p_rect_size) {
        auto rect = bin.Insert(
                        p_rect_size.x,
                        p_rect_size.y,
                        merge,
                        (rbp::GuillotineBinPack::FreeRectChoiceHeuristic)choice_method,
                        (rbp::GuillotineBinPack::GuillotineSplitHeuristic)split_method
        );
        return Rect2(rect.x, rect.y, rect.width, rect.height);
    }

    virtual Array insert_array(const PoolVector2Array& p_rect_size) {

        std::vector<rbp::RectSize> sizes(p_rect_size.size());
        for(int i= 0; i<p_rect_size.size(); i++) {
            sizes[i] = rbp::RectSize {int(p_rect_size[i].x), int(p_rect_size[i].y)};
        }
        bin.Insert(
                    sizes,
                    merge, (rbp::GuillotineBinPack::FreeRectChoiceHeuristic)choice_method,
                    (rbp::GuillotineBinPack::GuillotineSplitHeuristic)split_method
        );
        std::vector<rbp::Rect> dst = bin.GetUsedRectangles();
        Array arr;
        arr.resize(dst.size());
        for (int i=0; i<dst.size(); i++) {
            arr[i] = Rect2(dst[i].x, dst[i].y, dst[i].width, dst[i].height);
        }
        return arr;
    }

    virtual real_t occupancy() const { return bin.Occupancy(); }

    FreeRectChoiceHeuristic get_choice_method() const { return choice_method; }
    void set_choice_method(FreeRectChoiceHeuristic method) { choice_method = method; }

    GuillotineSplitHeuristic get_split_method() const { return split_method; }
    void set_split_method(GuillotineSplitHeuristic method) { split_method = method; }

    bool is_merge() const { return merge; }
    void set_merge(bool p_merge) { merge = p_merge; }

protected:
	static void _bind_methods() {

		ClassDB::bind_method(D_METHOD("set_choice_method", "p_methid"), &GuillotineBinPack::set_choice_method);
		ClassDB::bind_method(D_METHOD("get_choice_method"), &GuillotineBinPack::get_choice_method);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "choice_method"), "set_choice_method", "get_choice_method");

		ClassDB::bind_method(D_METHOD("set_split_method", "p_methid"), &GuillotineBinPack::set_split_method);
		ClassDB::bind_method(D_METHOD("get_split_method"), &GuillotineBinPack::get_split_method);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "split_method"), "set_split_method", "get_split_method");

		ClassDB::bind_method(D_METHOD("set_merge", "p_merge"), &GuillotineBinPack::set_merge);
		ClassDB::bind_method(D_METHOD("is_merge"), &GuillotineBinPack::is_merge);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "merge"), "set_merge", "is_merge");

		BIND_ENUM_CONSTANT(RectBestAreaFit);
		BIND_ENUM_CONSTANT(RectBestShortSideFit);
		BIND_ENUM_CONSTANT(RectBestLongSideFit);
		BIND_ENUM_CONSTANT(RectWorstAreaFit);
		BIND_ENUM_CONSTANT(RectWorstShortSideFit);
		BIND_ENUM_CONSTANT(RectWorstLongSideFit);

		BIND_ENUM_CONSTANT(SplitShorterLeftoverAxis);
		BIND_ENUM_CONSTANT(SplitLongerLeftoverAxis);
		BIND_ENUM_CONSTANT(SplitMinimizeArea);
		BIND_ENUM_CONSTANT(SplitMaximizeArea);
		BIND_ENUM_CONSTANT(SplitShorterAxis);
		BIND_ENUM_CONSTANT(SplitLongerAxis);
	}

private:
    rbp::GuillotineBinPack bin;
    bool merge = false;
    FreeRectChoiceHeuristic choice_method = RectBestAreaFit;
    GuillotineSplitHeuristic split_method = SplitShorterLeftoverAxis;
};
VARIANT_ENUM_CAST(GuillotineBinPack::FreeRectChoiceHeuristic);
VARIANT_ENUM_CAST(GuillotineBinPack::GuillotineSplitHeuristic);


class ShelfBinPack : public RectangleBinPack {
	GDCLASS(ShelfBinPack, RectangleBinPack);
public:
	/// Defines different heuristic rules that can be used in the packing process.
	enum ShelfChoiceHeuristic
	{
		ShelfNextFit, ///< -NF: We always put the new rectangle to the last open shelf.
		ShelfFirstFit, ///< -FF: We test each rectangle against each shelf in turn and pack it to the first where it fits.
		ShelfBestAreaFit, ///< -BAF: Choose the shelf with smallest remaining shelf area.
		ShelfWorstAreaFit, ///< -WAF: Choose the shelf with the largest remaining shelf area.
		ShelfBestHeightFit, ///< -BHF: Choose the smallest shelf (height-wise) where the rectangle fits.
		ShelfBestWidthFit, ///< -BWF: Choose the shelf that has the least remaining horizontal shelf space available after packing.
		ShelfWorstWidthFit, ///< -WWF: Choose the shelf that will have most remainining horizontal shelf space available after packing.
	};

	virtual void initialize(const Size2 &p_size) { bin.Init(p_size.x, p_size.y, use_waste_map); }
	virtual void set_flip_allowd(bool p_allowd) {
		// FIXME: implement this!
	}

    virtual Rect2 insert(const Size2 &p_rect_size) {
        auto rect = bin.Insert(
                        p_rect_size.x,
                        p_rect_size.y,
                        (rbp::ShelfBinPack::ShelfChoiceHeuristic)choice_method
        );
        return Rect2(rect.x, rect.y, rect.width, rect.height);
    }

    virtual Array insert_array(const PoolVector2Array& p_rect_size) {

        Array arr;
        for (int i=0; i<p_rect_size.size(); i++) {
            auto r = insert(p_rect_size[i]);
            if (r.get_area() <= 0)
                continue;
            arr.append(r);
        }
        return arr;
    }

    virtual real_t occupancy() const { return bin.Occupancy(); }

    ShelfChoiceHeuristic get_choice_method() const { return choice_method; }
    void set_choice_method(ShelfChoiceHeuristic method) { choice_method = method; }

    bool is_use_waste_map() const { return use_waste_map; }
    void set_use_waste_map(bool p_use_waste_map) { use_waste_map = p_use_waste_map; }

protected:
	static void _bind_methods() {

		ClassDB::bind_method(D_METHOD("set_choice_method", "p_methid"), &ShelfBinPack::set_choice_method);
		ClassDB::bind_method(D_METHOD("get_choice_method"), &ShelfBinPack::get_choice_method);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "choice_method"), "set_choice_method", "get_choice_method");

		ClassDB::bind_method(D_METHOD("set_use_waste_map", "p_merge"), &ShelfBinPack::set_use_waste_map);
		ClassDB::bind_method(D_METHOD("is_use_waste_map"), &ShelfBinPack::is_use_waste_map);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_waste_map"), "set_use_waste_map", "is_use_waste_map");

		BIND_ENUM_CONSTANT(ShelfNextFit);
		BIND_ENUM_CONSTANT(ShelfFirstFit);
		BIND_ENUM_CONSTANT(ShelfBestAreaFit);
		BIND_ENUM_CONSTANT(ShelfWorstAreaFit);
		BIND_ENUM_CONSTANT(ShelfBestHeightFit);
		BIND_ENUM_CONSTANT(ShelfBestWidthFit);
		BIND_ENUM_CONSTANT(ShelfWorstWidthFit);
	}

private:
    rbp::ShelfBinPack bin;
    bool use_waste_map = false;
    ShelfChoiceHeuristic choice_method = ShelfNextFit;
};
VARIANT_ENUM_CAST(ShelfBinPack::ShelfChoiceHeuristic);

class SkylineBinPack : public RectangleBinPack {
	GDCLASS(SkylineBinPack, RectangleBinPack);
public:
	/// Defines the different heuristic rules that can be used to decide how to make the rectangle placements.
	enum LevelChoiceHeuristic
	{
		LevelBottomLeft,
		LevelMinWasteFit
	};

	virtual void initialize(const Size2 &p_size) { bin.Init(p_size.x, p_size.y, use_waste_map); }
	virtual void set_flip_allowd(bool p_allowd) { bin.SetFlipAllowed(p_allowd); }

    virtual Rect2 insert(const Size2 &p_rect_size) {
        auto rect = bin.Insert(
                        p_rect_size.x,
                        p_rect_size.y,
						(rbp::SkylineBinPack::LevelChoiceHeuristic)choice_method
        );
        return Rect2(rect.x, rect.y, rect.width, rect.height);
    }

    virtual Array insert_array(const PoolVector2Array& p_rect_size) {

		std::vector<rbp::RectSize> sizes(p_rect_size.size());
		for(int i= 0; i<p_rect_size.size(); i++) {
			sizes[i] = rbp::RectSize {int(p_rect_size[i].x), int(p_rect_size[i].y)};
		}
		std::vector<rbp::Rect> dst;
		bin.Insert(sizes, dst, (rbp::SkylineBinPack::LevelChoiceHeuristic)choice_method);
		Array arr;
		arr.resize(dst.size());
		for (int i=0; i<dst.size(); i++) {
			arr[i] = Rect2(dst[i].x, dst[i].y, dst[i].width, dst[i].height);
		}
		return arr;
    }

    virtual real_t occupancy() const { return bin.Occupancy(); }

    LevelChoiceHeuristic get_choice_method() const { return choice_method; }
    void set_choice_method(LevelChoiceHeuristic method) { choice_method = method; }

    bool is_use_waste_map() const { return use_waste_map; }
    void set_use_waste_map(bool p_use_waste_map) { use_waste_map = p_use_waste_map; }

protected:
	static void _bind_methods() {

		ClassDB::bind_method(D_METHOD("set_choice_method", "p_methid"), &SkylineBinPack::set_choice_method);
		ClassDB::bind_method(D_METHOD("get_choice_method"), &SkylineBinPack::get_choice_method);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "choice_method"), "set_choice_method", "get_choice_method");

		ClassDB::bind_method(D_METHOD("set_use_waste_map", "p_merge"), &SkylineBinPack::set_use_waste_map);
		ClassDB::bind_method(D_METHOD("is_use_waste_map"), &SkylineBinPack::is_use_waste_map);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_waste_map"), "set_use_waste_map", "is_use_waste_map");

		BIND_ENUM_CONSTANT(LevelBottomLeft);
		BIND_ENUM_CONSTANT(LevelMinWasteFit);
	}

private:
    rbp::SkylineBinPack bin;
    bool use_waste_map = false;
    LevelChoiceHeuristic choice_method = LevelBottomLeft;
};
VARIANT_ENUM_CAST(SkylineBinPack::LevelChoiceHeuristic);

#endif // RECT_BIN_PACKER_HPP
