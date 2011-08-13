/**
 * @file
 * @brief Canvas item belonging to an SVG drawing element
 *//*
 * Authors:
 *   Krzysztof Kosiński <tweenk.pl@gmail.com>
 *
 * Copyright (C) 2011 Authors
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#ifndef SEEN_INKSCAPE_DISPLAY_DRAWING_ITEM_H
#define SEEN_INKSCAPE_DISPLAY_DRAWING_ITEM_H

#include <list>
#include <exception>
#include <boost/operators.hpp>
#include <boost/utility.hpp>
#include <boost/intrusive/list.hpp>
#include <2geom/rect.h>
#include <2geom/affine.h>
#include "display/display-forward.h"

class SPStyle;

namespace Inkscape {

struct UpdateContext {
    Geom::Affine ctm;
};

struct CacheRecord
    : boost::totally_ordered<CacheRecord>
{
    bool operator<(CacheRecord const &other) const { return score < other.score; }
    bool operator==(CacheRecord const &other) const { return score == other.score; }
    operator DrawingItem *() const { return item; }
    double score;
    size_t cache_size;
    DrawingItem *item;
};
typedef std::list<CacheRecord> CacheList;

class InvalidItemException : public std::exception {
    virtual const char *what() const throw() {
        return "Invalid item in drawing";
    }
};

class DrawingItem
    : boost::noncopyable
{
public:
    enum RenderFlags {
        RENDER_DEFAULT = 0,
        RENDER_CACHE_ONLY = 1,
        RENDER_BYPASS_CACHE = 2
    };
    enum StateFlags {
        STATE_NONE = 0,
        STATE_BBOX = (1<<0),    // bounding boxes are up-to-date
        STATE_CACHE = (1<<1),   // cache extents and clean area are up-to-date
        STATE_PICK = (1<<2),    // can process pick requests
        STATE_RENDER = (1<<3),  // can be rendered
        STATE_ALL = (1<<4)-1
    };
    enum PickFlags {
        PICK_NORMAL = 0, // normal pick
        PICK_STICKY = (1<<0), // sticky pick - ignore visibility and sensitivity
        PICK_AS_CLIP = (1<<2) // pick with no stroke and opaque fill regardless of item style
    };

    DrawingItem(Drawing &drawing);
    virtual ~DrawingItem();

    Geom::OptIntRect geometricBounds() const { return _bbox; }
    Geom::OptIntRect visualBounds() const { return _drawbox; }
    Geom::OptRect itemBounds() const { return _item_bbox; }
    Geom::Affine ctm() const { return _ctm; }
    Geom::Affine transform() const { return _transform ? *_transform : Geom::identity(); }
    Drawing &drawing() const { return _drawing; }
    DrawingItem *parent() const;

    void appendChild(DrawingItem *item);
    void prependChild(DrawingItem *item);
    void clearChildren();

    bool visible() const { return _visible; }
    void setVisible(bool v);
    bool sensitive() const { return _sensitive; }
    void setSensitive(bool v);
    bool cached() const { return _cached; }
    void setCached(bool c, bool persistent = false);

    void setOpacity(float opacity);
    void setTransform(Geom::Affine const &trans);
    void setClip(DrawingItem *item);
    void setMask(DrawingItem *item);
    void setZOrder(unsigned z);
    void setItemBounds(Geom::OptRect const &bounds);

    void setKey(unsigned key) { _key = key; }
    unsigned key() const { return _key; }
    void setData(void *data) { _user_data = data; }
    void *data() const { return _user_data; }

    void update(Geom::IntRect const &area = Geom::IntRect::infinite(), UpdateContext const &ctx = UpdateContext(), unsigned flags = STATE_ALL, unsigned reset = 0);
    void render(DrawingContext &ct, Geom::IntRect const &area, unsigned flags = 0);
    void clip(DrawingContext &ct, Geom::IntRect const &area);
    DrawingItem *pick(Geom::Point const &p, double delta, unsigned flags = 0);

protected:
    enum ChildType {
        CHILD_ORPHAN = 0, // no parent
        CHILD_NORMAL = 1, // contained in _children of parent
        CHILD_CLIP = 2, // referenced by _clip member of parent
        CHILD_MASK = 3, // referenced by _mask member of parent
        CHILD_ROOT = 4, // root item of _drawing
        CHILD_FILL_PATTERN = 5, // not yet implemented: referenced by fill pattern of parent
        CHILD_STROKE_PATTERN = 6 // not yet implemented: referenced by stroke pattern of parent
    };

    void _renderOutline(DrawingContext &ct, Geom::IntRect const &area, unsigned flags);
    void _markForUpdate(unsigned state, bool propagate);
    void _markForRendering();
    void _setStyleCommon(SPStyle *&_style, SPStyle *style);
    double _cacheScore();
    Geom::OptIntRect _cacheRect();
    virtual unsigned _updateItem(Geom::IntRect const &area, UpdateContext const &ctx,
                                 unsigned flags, unsigned reset) { return 0; }
    virtual void _renderItem(DrawingContext &ct, Geom::IntRect const &area, unsigned flags) {}
    virtual void _clipItem(DrawingContext &ct, Geom::IntRect const &area) {}
    virtual DrawingItem *_pickItem(Geom::Point const &p, double delta, unsigned flags) { return NULL; }
    virtual bool _canClip() { return false; }

    // member variables start here

    Drawing &_drawing;
    DrawingItem *_parent;

    typedef boost::intrusive::list_member_hook<> ListHook;
    ListHook _child_hook;

    typedef boost::intrusive::list<
        DrawingItem,
        boost::intrusive::member_hook<DrawingItem, ListHook, &DrawingItem::_child_hook>
        > ChildrenList;
    ChildrenList _children;

    unsigned _key; ///< Some SPItems can have more than one DrawingItem;
                   ///  this value is a hack used to distinguish between them
    float _opacity;

    Geom::Affine *_transform; ///< Incremental transform from parent to this item's coords
    Geom::Affine _ctm; ///< Total transform from item coords to display coords
    Geom::OptIntRect _bbox; ///< Bounding box in display (pixel) coords
    Geom::OptIntRect _drawbox; ///< Bounding box enlarged by filters, shrinked by clips and masks
    Geom::OptRect _item_bbox; ///< Bounding box in item coordinates

    DrawingItem *_clip;
    DrawingItem *_mask;
    Inkscape::Filters::Filter *_filter;
    void *_user_data; ///< Used to associate DrawingItems with SPItems that created them
    DrawingCache *_cache;

    CacheList::iterator _cache_iterator;

    unsigned _state : 8;
    unsigned _child_type : 3; // see ChildType enum
    unsigned _visible : 1;
    unsigned _sensitive : 1; ///< Whether this item responds to events
    unsigned _cached : 1; ///< Whether the rendering is stored for reuse
    unsigned _cached_persistent : 1; ///< If set, will always be cached regardless of score
    unsigned _has_cache_iterator : 1; ///< If set, _cache_iterator is valid
    unsigned _propagate : 1; ///< Whether to call update for all children on next update
    //unsigned _renders_opacity : 1; ///< Whether object needs temporary surface for opacity
    unsigned _pick_children : 1; ///< For groups: if true, children are returned from pick(),
                                 ///  otherwise the group is returned

    friend class Drawing;
};

struct DeleteDisposer {
    void operator()(DrawingItem *item) { delete item; }
};

} // end namespace Inkscape

#endif // !SEEN_INKSCAPE_DISPLAY_DRAWING_ITEM_H

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
