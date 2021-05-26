#ifndef HISTORY_CONTROLLER_HH
#define HISTORY_CONTROLLER_HH

#include "xtensor/xbuilder.hpp"
#include "xtensor/xstrided_view.hpp"
#include "xtensor/xtensor_forward.hpp"
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

class dynamic_action_t {
public:
    template<typename T>
    dynamic_action_t(T&& x) : self_(new model<std::decay_t<T>>(std::forward<T>(x))) {}

    dynamic_action_t(const dynamic_action_t& x) : self_(x.self_->copy_()) {}
    dynamic_action_t(dynamic_action_t&&) noexcept = default;

    dynamic_action_t& operator=(const dynamic_action_t& x) {
        dynamic_action_t tmp(x);
        *this = std::move(tmp);
        return *this;
    }
    dynamic_action_t& operator=(dynamic_action_t&&) noexcept = default;

    // friend void draw(const dynamic_action_t& x, std::ostream& out, std::size_t position) { x.self_->draw_(out, position); }

private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual concept_t* copy_() const = 0;
        // virtual void draw_(std::ostream&, std::size_t) const = 0;
    };
    template<typename T>
    struct model : concept_t {
        model(T x) : data_(std::move(x)) {}
        concept_t* copy_() const override { return new model(*this); }
        // void draw_(std::ostream& out, size_t position) const override { draw(data_, out, position); }

        T data_;
    };

    std::unique_ptr<const concept_t> self_;
};

// using history_t = std::vector<std::function<void()>>;
template<class Document>
class history_t {
public:
    explicit history_t(Document document) {
        mStorage.push_back(std::move(document));
        mCurrent++;
    }
    // using document_t = xt::xtensor<float, 2>;

    // change to write/read to automatically commit before write
    void commit() {
        mStorage.erase(mStorage.begin() + mCurrent + 1, mStorage.end());
        mStorage.push_back(mStorage.back());
        mCurrent++;
    }

    Document& current() {
        if (mStorage.empty())
            throw std::runtime_error("history_t is empty");

        return mStorage.back();
    }

    void undo() {
        mCurrent--;
    }

    void redo() {
        if (mStorage.size() <= (mCurrent + 1)) {
            return;
        }
        mCurrent++;
    }

    const std::vector<Document>& storage() const { return mStorage; }

private:
    std::int32_t mCurrent = -1;
    std::vector<Document> mStorage;
};

template<class T>
history_t(T) -> history_t<T>;

template<class Document>
class action_base_t {
public:
    virtual void apply(Document& document) {}
    virtual std::string name() { return std::string("empty action"); }
};

template<class Document, class F>
class transform_action_t : public action_base_t<Document> {
public:
    explicit transform_action_t(F&& transform) : mTransform(std::move(transform)) {}

    void apply(Document& document) override {
        mTransform(document);
    }
    std::string name() override { return std::string("transform action"); }

private:
    F mTransform;
};


template<class Document>
class history_controller_t {
public:
    template<class T>
    history_controller_t(T&& initVersion) : mHistory(std::forward<T>(initVersion)) {}

    template<class T>
    void apply(T&& action) {
        action.apply(mHistory.current());
        mHistory.commit();
        mCommands.emplace_back(new std::decay_t<T>(std::forward<T>(action)));
    }

    history_t<Document> mHistory;
    std::vector<std::unique_ptr<action_base_t<Document>>> mCommands;
};

template<class T>
history_controller_t(T&&) -> history_controller_t<std::decay_t<T>>;

void test_history_controller() {
    using Document = xt::xtensor<float, 2>;

    history_controller_t history_controller{ xt::xtensor<float, 2>{ xt::arange(25).reshape({ 5, 5 }) } };

    auto draw = [val = 1025](auto&& image) mutable { image[{ 1, 1 }] = val++; };
    transform_action_t<Document, decltype(draw)> transform_draw{ std::move(draw) };
    history_controller.apply(transform_draw);
    history_controller.apply(transform_draw);
    history_controller.apply(transform_draw);
    std::cout << "history_controller.mHistory.current() : " << history_controller.mHistory.current() << "\n";

    xt::xtensor<float, 2> img = xt::arange(25).reshape({ 5, 5 });
    action_base_t<Document>& action = transform_draw;
    action.apply(img);

    for (int i = 0; auto&& image : history_controller.mHistory.storage()) {
        std::cout << "history " << i++ << ":\n"
                  << image << "\n";
    }

    for (int i = 0; auto&& action : history_controller.mCommands) {
        std::cout << "action: " << action->name() << "\n";
    }

    // history_controller.commit(newImage);
}

struct Pos {
    int x;
    int y;
};

/**
 *      Policy based design
 */

namespace policy_test {

class AbstractHistory {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class HistoryService {
public:
    void undo() {
        if (mCurrIdx < 0 || mCurrIdx >= mTimeline.size()) {
            return;
        }
        mTimeline[mCurrIdx]->undo();
        --mCurrIdx;
    }
    void redo() {
        if (mCurrIdx < 0 || mCurrIdx >= mTimeline.size() - 1) {
            return;
        }
        mTimeline[mCurrIdx]->redo();
        ++mCurrIdx;
    }
    void push_back(AbstractHistory* history) {
        mTimeline.erase(mTimeline.begin() + mCurrIdx + 1, mTimeline.end());
        mTimeline.push_back(history);
        mCurrIdx++;
    }

private:
    int mCurrIdx = -1;
    std::vector<AbstractHistory*> mTimeline;
};


template<class D, class Parent>
class HistoryImpl : public AbstractHistory {
public:
    inline void commit() {
        if (mParent) {
            mParent->push_back(this);
        }
        derived_cast().commitImpl();
    }

protected:
    explicit HistoryImpl(Parent* aParent) : mParent(aParent) {}

private:
    friend Parent;
    inline void undo() final {
        derived_cast().undoImpl();
    }
    inline void redo() final {
        derived_cast().redoImpl();
    }

    inline auto derived_cast() noexcept -> D& {
        return *static_cast<D*>(this);
    }

    Parent* mParent = nullptr;
};

template<class D>
class HistoryImpl<D, void> : public AbstractHistory {
public:
    inline void undo() final {
        derived_cast().undoImpl();
    }
    inline void redo() final {
        derived_cast().redoImpl();
    }
    inline void commit() {
        derived_cast().commitImpl();
    }

protected:
    explicit HistoryImpl([[maybe_unused]] void* aParent) {}

private:
    inline auto derived_cast() noexcept -> D& {
        return *static_cast<D*>(this);
    }
};


template<class Parent = void>
class History : public HistoryImpl<History<Parent>, Parent> {
public:
    using Base = HistoryImpl<History<Parent>, Parent>;
    friend Base;

    // xt::xtensor<float, 2> mdoc;
    // std::vector<Action> mActions;

    explicit History(Parent* aParent = nullptr) : Base(aParent) {}

private:
    void commitImpl() {
    }

    void undoImpl() {
        std::cout << "undoImpl called\n";
    }
    void redoImpl() {
        std::cout << "redoImpl called\n";
    }
};
// template<class Parent>
// History(Parent*) -> History<Parent>;

template<auto v>
class ptype;

template<class T>
class ActionAbstract {
public:
    virtual void redo(T& aDoc) {}
    virtual void undo(T& aDoc) {}
};

struct BBox {
    Pos begin;
    Pos end;
};

template<class T>
class SaveBBoxAction : ActionAbstract<T> {
public:
    BBox mBBox;
    T savedBBox;
    SaveBBoxAction(const BBox& aBBox) : mBBox(aBBox) {
        savedBBox.resize({ aBBox.end.y - aBBox.begin.y, aBBox.end.y - aBBox.begin.y });
    }

    void redo(T& aImage) {
        savedBBox = xt::view(aImage, xt::range(mBBox.begin.y, mBBox.end.y), xt::range(mBBox.begin.x, mBBox.end.x));
    }
    void undo(T& aImage) {
        xt::view(aImage, xt::range(mBBox.begin.y, mBBox.end.y), xt::range(mBBox.begin.x, mBBox.end.x)) = savedBBox;
    }
};

void test_policy() {
    History history;
    // history.resetDocument(newDoc);
    history.commit();
    // history.undo();

    HistoryService service;
    History history_2(&service);

    service.undo();
    history_2.commit();
    history_2.commit();
    service.undo();
    service.undo();
    service.undo();

    // ptype<sizeof(decltype(history))>{};
    // ptype<sizeof(decltype(history_2))>{};
}

} // namespace policy_test

namespace store_action_test {

using Doc = xt::xtensor<float, 2>;


class Action {
public:
    virtual void redo(Doc&) {}
    virtual void undo(Doc&) {}
};

class SaveChunkAction {
public:
    Pos start;
    Doc mChunk;

    void apply(Doc& aWorkingDoc, Doc& aHistoryDoc) {
        mChunk = xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
        xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1))) = xt::view(aWorkingDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
    }
    void redo(Doc& aWorkingDoc, Doc& aHistoryDoc) {
        xt::view(aWorkingDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1))) = mChunk;
        mChunk = xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
        xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1))) = xt::view(aWorkingDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
    }
    void undo(Doc& aWorkingDoc, Doc& aHistoryDoc) {
        xt::view(aWorkingDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1))) = mChunk;
        mChunk = xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
        xt::view(aHistoryDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1))) = xt::view(aWorkingDoc, xt::range(start.y, mChunk.shape(0)), xt::range(start.x, mChunk.shape(1)));
    }
};

class DrawAction {
public:
    std::vector<Pos> coords = {};
    std::vector<float> values;

    void redo(Doc& aWorkingDoc, Doc& aHistoryDoc) {
        // for (int i = 0; i < coords.size(); ++i) {
        //     values[i] = aHistoryDoc[coords[i]];
        // }
        // for (int i = 0; i < coords.size(); ++i) {
        //     aHistoryDoc[coords[i]] = aWorkingDoc[coords[i]];
        // }
    }
    void undo(Doc& aWorkingDoc, Doc& aHistoryDoc) {
        // for (int i = 0; i < coords.size(); ++i) {
        //     values[i] = aHistoryDoc[coords[i]];
        // }
        // for (int i = 0; i < coords.size(); ++i) {
        //     aHistoryDoc[coords[i]] = aWorkingDoc[coords[i]];
        // }
    }
};
class History {
public:
    std::vector<Action*> mActions;
};

}

namespace store_changes_test {

using Doc = xt::xtensor<float, 2>;

class Change {
public:
    virtual ~Change() = default;
};

class BBoxChange : public Change {
public:
    Pos begin;
    Doc bbox;
};

class History {
    std::vector<std::unique_ptr<Change>> mChanges;
    int mCurrIdx = -1;
    Doc mWorking;
    Doc mLastSaved;

public:
    auto& working() { return mWorking; }
    void setWorking(const Doc& aNew) {
        mWorking = aNew;
        mLastSaved = aNew;
        mChanges.clear();
        mCurrIdx = -1;
    }

    void commit(Pos aBegin, Pos aEnd) {
        auto vChange = std::make_unique<BBoxChange>();
        vChange->begin = aBegin;

        auto vLastSavedView = xt::view(mLastSaved, xt::range(aBegin.y, aEnd.y), xt::range(aBegin.x, aEnd.x));
        vChange->bbox = vLastSavedView;
        vLastSavedView = xt::view(mWorking, xt::range(aBegin.y, aEnd.y), xt::range(aBegin.x, aEnd.x));

        mChanges.emplace_back(std::move(vChange));
        ++mCurrIdx;
    }

    void undo() {
        auto ptr = mChanges[mCurrIdx].get();
        --mCurrIdx;

        if (auto change = dynamic_cast<BBoxChange*>(ptr)) {
            auto vWorkingView = xt::view(mWorking, xt::range(change->begin.y, change->begin.y + change->bbox.shape(0)), xt::range(change->begin.x, change->begin.x + change->bbox.shape(1)));
            auto vLastSavedView = xt::view(mLastSaved, xt::range(change->begin.y, change->begin.y + change->bbox.shape(0)), xt::range(change->begin.x, change->begin.x + change->bbox.shape(1)));
            vWorkingView = change->bbox;
            change->bbox = vLastSavedView;
            vLastSavedView = vWorkingView;
        }
    }

    void redo() {
        auto ptr = mChanges[mCurrIdx + 1].get();
        ++mCurrIdx;

        if (auto change = dynamic_cast<BBoxChange*>(ptr)) {
            auto vWorkingView = xt::view(mWorking, xt::range(change->begin.y, change->begin.y + change->bbox.shape(0)), xt::range(change->begin.x, change->begin.x + change->bbox.shape(1)));
            auto vLastSavedView = xt::view(mLastSaved, xt::range(change->begin.y, change->begin.y + change->bbox.shape(0)), xt::range(change->begin.x, change->begin.x + change->bbox.shape(1)));
            vWorkingView = change->bbox;
            change->bbox = vLastSavedView;
            vLastSavedView = vWorkingView;
        }
    }
};

void test() {
    History vHistory;
    vHistory.setWorking(xt::arange(25).reshape({ 5, 5 }));
    auto& vWorking = vHistory.working();
    std::cout << "vWorking : \n"
              << vWorking << "\n";

    std::cout << "commit\n";
    vWorking(1, 1) = 5000;
    vWorking(2, 2) = 6000;
    vHistory.commit({ 1, 1 }, { 3, 3 });
    std::cout << "vWorking : \n"
              << vWorking << "\n";

    std::cout << "undo\n";
    vHistory.undo();
    std::cout << "vWorking : \n"
              << vWorking << "\n";

    std::cout << "redo\n";
    vHistory.redo();
    std::cout << "vWorking : \n"
              << vWorking << "\n";
}

}

#endif