/*
 * A container class for filter slots. Allows for simple getting and
 * setting images in filter slots without having to bother with
 * table indexes and such.
 *
 * Author:
 *   Niko Kiirala <niko@kiirala.com>
 *
 * Copyright (C) 2006,2007 Niko Kiirala
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#include <assert.h>
#include <string.h>

#include "display/nr-arena-item.h"
#include "display/nr-filter-types.h"
#include "display/nr-filter-gaussian.h"
#include "display/nr-filter-slot.h"
#include "display/nr-filter-getalpha.h"
#include "display/nr-filter-units.h"
#include "display/pixblock-scaler.h"
#include "display/pixblock-transform.h"
#include "libnr/nr-pixblock.h"
#include "libnr/nr-blit.h"

__attribute__ ((const))
inline static int _max4(const double a, const double b,
                        const double c, const double d) {
    double ret = a;
    if (b > ret) ret = b;
    if (c > ret) ret = c;
    if (d > ret) ret = d;
    return (int)round(ret);
}

__attribute__ ((const))
inline static int _min4(const double a, const double b,
                        const double c, const double d) {
    double ret = a;
    if (b < ret) ret = b;
    if (c < ret) ret = c;
    if (d < ret) ret = d;
    return (int)round(ret);
}

__attribute__ ((const))
inline static int _max2(const double a, const double b) {
    if (a > b)
        return (int)round(a);
    else
        return (int)round(b);
}

__attribute__ ((const))
inline static int _min2(const double a, const double b) {
    if (a > b)
        return (int)round(b);
    else
        return (int)round(a);
}

namespace Inkscape {
namespace Filters {

FilterSlot::FilterSlot(int slots, NRArenaItem const *item)
    : _last_out(-1),
      filterquality(FILTER_QUALITY_BEST),
      blurquality(BLUR_QUALITY_BEST),
      _arena_item(item)
{
    _slots.reserve((slots > 0) ? slots : 2);
}

FilterSlot::~FilterSlot()
{
    for (unsigned int i = 0 ; i < _slots.size() ; i++) {
        if (_slots[i].owned) {
            nr_pixblock_release(_slots[i].pb);
            delete _slots[i].pb;
        }
    }
}

FilterSlot::slot_entry_t::~slot_entry_t()
{
    // It's a bad idea to destruct pixblocks here, as this will also be called upon resizing _slots
}

NRPixBlock *FilterSlot::get(int slot_nr)
{
    int index = _get_index(slot_nr);
    assert(index >= 0);

    /* If we didn't have the specified image, but we could create it
     * from the other information we have, let's do that */
    if (_slots[index].pb == NULL
        && (slot_nr == NR_FILTER_SOURCEALPHA
            || slot_nr == NR_FILTER_BACKGROUNDIMAGE
            || slot_nr == NR_FILTER_BACKGROUNDALPHA
            || slot_nr == NR_FILTER_FILLPAINT
            || slot_nr == NR_FILTER_STROKEPAINT))
    {
        /* If needed, fetch background */
        if (slot_nr == NR_FILTER_BACKGROUNDIMAGE) {
            NRPixBlock *pb;
            pb = nr_arena_item_get_background(_arena_item);
            if (pb) {
                pb->empty = false;
                this->set(NR_FILTER_BACKGROUNDIMAGE, pb, false);
            } else {
                NRPixBlock *source = this->get(NR_FILTER_SOURCEGRAPHIC);
                pb = new NRPixBlock();
                if (!pb) return NULL; // Allocation failed
                nr_pixblock_setup_fast(pb, source->mode,
                                       source->area.x0, source->area.y0,
                                       source->area.x1, source->area.y1, true);
                if (pb->size != NR_PIXBLOCK_SIZE_TINY && pb->data.px == NULL) {
                    // allocation failed
                    delete pb;
                    return NULL;
                }
                pb->empty = FALSE;
                this->set(NR_FILTER_BACKGROUNDIMAGE, pb);
            }
        } else if (slot_nr == NR_FILTER_SOURCEALPHA) {
            /* If only a alpha channel is needed, strip it from full image */
            NRPixBlock *src = get(NR_FILTER_SOURCEGRAPHIC);
            NRPixBlock *sa = filter_get_alpha(src);
            set(NR_FILTER_SOURCEALPHA, sa);
        } else if (slot_nr == NR_FILTER_BACKGROUNDALPHA) {
            NRPixBlock *src = get(NR_FILTER_BACKGROUNDIMAGE);
            NRPixBlock *ba = filter_get_alpha(src);
            set(NR_FILTER_BACKGROUNDALPHA, ba);
        } else if (slot_nr == NR_FILTER_FILLPAINT) {
            /* When a paint is needed, fetch it from arena item */
            // TODO
        } else if (slot_nr == NR_FILTER_STROKEPAINT) {
            // TODO
        }
    }

    if (_slots[index].pb) {
        _slots[index].pb->empty = false;
    }

    assert(slot_nr == NR_FILTER_SLOT_NOT_SET ||_slots[index].number == slot_nr);
    return _slots[index].pb;
}

void FilterSlot::get_final(int slot_nr, NRPixBlock *result) {
    NRPixBlock *final_usr = get(slot_nr);
    Geom::Matrix trans = units.get_matrix_pb2display();

    int size = (result->area.x1 - result->area.x0)
        * (result->area.y1 - result->area.y0)
        * NR_PIXBLOCK_BPP(result);
    memset(NR_PIXBLOCK_PX(result), 0, size);

    if (fabs(trans[1]) > 1e-6 || fabs(trans[2]) > 1e-6) {
        if (filterquality == FILTER_QUALITY_BEST) {
            NR::transform_bicubic(result, final_usr, trans);
        } else {
            NR::transform_nearest(result, final_usr, trans);
        }
    } else if (fabs(trans[0] - 1) > 1e-6 || fabs(trans[3] - 1) > 1e-6) {
        NR::scale_bicubic(result, final_usr, trans);
    } else {
        nr_blit_pixblock_pixblock(result, final_usr);
    }
}

void FilterSlot::set(int slot_nr, NRPixBlock *pb, bool takeOwnership)
{
    /* Unnamed slot is for saving filter primitive results, when parameter
     * 'result' is not set. Only the filter immediately after this one
     * can access unnamed results, so we don't have to worry about overwriting
     * previous results in filter chain. On the other hand, we may not
     * overwrite any other image with this one, because they might be
     * accessed later on. */
    int index = ((slot_nr != NR_FILTER_SLOT_NOT_SET)
                 ? _get_index(slot_nr)
                 : _get_index(NR_FILTER_UNNAMED_SLOT));
    assert(index >= 0);
    // Unnamed slot is only for Inkscape::Filters::FilterSlot internal use.
    assert(slot_nr != NR_FILTER_UNNAMED_SLOT);
    assert(slot_nr == NR_FILTER_SLOT_NOT_SET ||_slots[index].number == slot_nr);

    if (slot_nr == NR_FILTER_SOURCEGRAPHIC || slot_nr == NR_FILTER_BACKGROUNDIMAGE) {
        Geom::Matrix trans = units.get_matrix_display2pb();
        if (fabs(trans[1]) > 1e-6 || fabs(trans[2]) > 1e-6) {
            NRPixBlock *trans_pb = new NRPixBlock;
            int x0 = pb->area.x0;
            int y0 = pb->area.y0;
            int x1 = pb->area.x1;
            int y1 = pb->area.y1;
            int min_x = _min4(trans[0] * x0 + trans[2] * y0 + trans[4],
                              trans[0] * x0 + trans[2] * y1 + trans[4],
                              trans[0] * x1 + trans[2] * y0 + trans[4],
                              trans[0] * x1 + trans[2] * y1 + trans[4]);
            int max_x = _max4(trans[0] * x0 + trans[2] * y0 + trans[4],
                              trans[0] * x0 + trans[2] * y1 + trans[4],
                              trans[0] * x1 + trans[2] * y0 + trans[4],
                              trans[0] * x1 + trans[2] * y1 + trans[4]);
            int min_y = _min4(trans[1] * x0 + trans[3] * y0 + trans[5],
                              trans[1] * x0 + trans[3] * y1 + trans[5],
                              trans[1] * x1 + trans[3] * y0 + trans[5],
                              trans[1] * x1 + trans[3] * y1 + trans[5]);
            int max_y = _max4(trans[1] * x0 + trans[3] * y0 + trans[5],
                              trans[1] * x0 + trans[3] * y1 + trans[5],
                              trans[1] * x1 + trans[3] * y0 + trans[5],
                              trans[1] * x1 + trans[3] * y1 + trans[5]);

            nr_pixblock_setup_fast(trans_pb, pb->mode,
                                   min_x, min_y,
                                   max_x, max_y, true);
            if (trans_pb->size != NR_PIXBLOCK_SIZE_TINY && trans_pb->data.px == NULL) {
                /* TODO: this gets hit occasionally. Worst case scenario:
                 * images are exported in horizontal stripes. One stripe
                 * is not too high, but can get thousands of pixels wide.
                 * Rotate this 45 degrees -> _huge_ image */
                g_warning("Memory allocation failed in Inkscape::Filters::FilterSlot::set (transform)");
                if (takeOwnership) {
                    nr_pixblock_release(pb);
                    delete pb;
                }
                return;
            }
            if (filterquality == FILTER_QUALITY_BEST) {
                NR::transform_bicubic(trans_pb, pb, trans);
            } else {
                NR::transform_nearest(trans_pb, pb, trans);
            }
            if (takeOwnership) {
                nr_pixblock_release(pb);
                delete pb;
            }
            takeOwnership = true;
            pb = trans_pb;
        } else if (fabs(trans[0] - 1) > 1e-6 || fabs(trans[3] - 1) > 1e-6) {
            NRPixBlock *trans_pb = new NRPixBlock;

            int x0 = pb->area.x0;
            int y0 = pb->area.y0;
            int x1 = pb->area.x1;
            int y1 = pb->area.y1;
            int min_x = _min2(trans[0] * x0 + trans[4],
                              trans[0] * x1 + trans[4]);
            int max_x = _max2(trans[0] * x0 + trans[4],
                              trans[0] * x1 + trans[4]);
            int min_y = _min2(trans[3] * y0 + trans[5],
                              trans[3] * y1 + trans[5]);
            int max_y = _max2(trans[3] * y0 + trans[5],
                              trans[3] * y1 + trans[5]);

            nr_pixblock_setup_fast(trans_pb, pb->mode,
                                   min_x, min_y, max_x, max_y, true);
            if (trans_pb->size != NR_PIXBLOCK_SIZE_TINY && trans_pb->data.px == NULL) {
                g_warning("Memory allocation failed in Inkscape::Filters::FilterSlot::set (scaling)");
                if (takeOwnership) {
                    nr_pixblock_release(pb);
                    delete pb;
                }
                return;
            }
            NR::scale_bicubic(trans_pb, pb, trans);
            if (takeOwnership) {
                nr_pixblock_release(pb);
                delete pb;
            }
            takeOwnership = true;
            pb = trans_pb;
        }
    }

    if(_slots[index].owned) {
        nr_pixblock_release(_slots[index].pb);
        delete _slots[index].pb;
    }
    _slots[index].pb = pb;
    _slots[index].owned = takeOwnership;
    _last_out = index;
}

int FilterSlot::get_slot_count()
{
    return _slots.size();
}

NRArenaItem const* FilterSlot::get_arenaitem()
{
        return _arena_item;
}

int FilterSlot::_get_index(int slot_nr)
{
    assert(slot_nr >= 0 ||
           slot_nr == NR_FILTER_SLOT_NOT_SET ||
           slot_nr == NR_FILTER_SOURCEGRAPHIC ||
           slot_nr == NR_FILTER_SOURCEALPHA ||
           slot_nr == NR_FILTER_BACKGROUNDIMAGE ||
           slot_nr == NR_FILTER_BACKGROUNDALPHA ||
           slot_nr == NR_FILTER_FILLPAINT ||
           slot_nr == NR_FILTER_STROKEPAINT ||
           slot_nr == NR_FILTER_UNNAMED_SLOT);

    if (slot_nr == NR_FILTER_SLOT_NOT_SET) {
        return _last_out;
    }

    /* Search, if the slot already exists */
    for (int i = 0 ; i < (int)_slots.size() ; i++) {
        if (_slots[i].number == slot_nr) {
            return i;
        }
    }

    /* If the slot doesn't already exist, create it */
    slot_entry_t entry;
    entry.number = slot_nr;
    _slots.push_back(entry);
    return (int)_slots.size()-1;
}

void FilterSlot::set_units(FilterUnits const &units) {
    this->units = units;
}

void FilterSlot::set_quality(FilterQuality const q) {
    filterquality = q;
}

void FilterSlot::set_blurquality(int const q) {
    blurquality = q;
}

int FilterSlot::get_blurquality(void) {
    return blurquality;
}

} /* namespace Filters */
} /* namespace Inkscape */

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :
