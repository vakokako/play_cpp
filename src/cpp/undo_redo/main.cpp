#include "xtensor/xbuilder.hpp"
#include "xtensor/xtensor_forward.hpp"
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

#include "history_controller.hh"

class shallow_img;

struct shared_obj {
    int curr_idx = 0;
    shallow_img* curr_ptr = nullptr;
    xt::xtensor<float, 2> image;
};

struct shallow_img {
    const int idx;
    std::shared_ptr<shared_obj> mShared;
    shallow_img* next = nullptr;
    shallow_img* prev = nullptr;

    std::function<void(xt::xtensor<float, 2>&)> redo;
    std::function<void(xt::xtensor<float, 2>&)> undo;
    shallow_img(const xt::xtensor<float, 2>& image) : idx(0) {
        mShared = std::make_shared<shared_obj>(shared_obj{ idx, this, image });
    }
    shallow_img(shallow_img& image) : idx(image.idx + 1) {
        prev = &image;
        mShared = image.mShared;
        image.next = this;
    }

    const xt::xtensor<float, 2>& get() {
        while (mShared->curr_idx != idx) {
            if (mShared->curr_idx < idx) {
                mShared->curr_ptr->next->redo(mShared->image);
                mShared->curr_ptr = mShared->curr_ptr->next;
            } else {
            }
            mShared->curr_idx = mShared->curr_ptr->idx;
        }
    }
};

int main() {
    xt::xtensor<float, 2> data = xt::arange(25).reshape({ 5, 5 });
    shallow_img image1(data);

    store_changes_test::test();

    // auto draw_1 = [std::vector<std::pair<pos, color>> changes](xt:xtensor image) {

    // }
}

#if 0
class Document {
public:
    getPixel();
    void setPixel() {
        aChangeset |= aCoordinate;
    }

    xt::view getBoundingBox(bda::Box aBox) {
        // aChangeset |= aBox;
    }

    auto getImage() {
        aChangeset |= bda::Box(0, size);
    }

    void commit() {
        xt::xtensor vDiff = vOldImage(aChangeset);
        // store diff to distory
        aHistory.push_back(vDiff);
        vOldImage(aChangeset) = vNewImage(aChangeset);
    }

protected:
    xt::xtensor<T, U> vNewImage;
    bda::Box aChangeset;

    xt::xtensor<T, U> vOldImage;

    // std::vector<xtensor> aHistory;
    std::vector<action> aHistory;
}


Document vDocument;
auto vView = vDocument.getBoundingBox(bounding_box);
vView(7, 14, 3) += 1;
vDocument.commit(bounding_box);



class HistoryDocument {
public:
    void draw(vector_pixels) {
        action = createDrawAction(vector_pixels);
        history.applyAction(action);
    }
    void commitBox(bbox) {
        action = createBBoxAction(img, bbox);
        history.applyAction(action);
    }
    void commitWhole() {
        history.commit(img);
    }

    xt::xtensor<float, 2>& img() {
        return history.current();
    }

private:
    history_t<xt::xtensor<float, 2>> history;
}

HistoryDocument.loadImage();
// here we already need a second copy. current RAM is twice the document.
HistoryDocument.img.fill(0);
HistoryDocument.commitWhole();
#endif