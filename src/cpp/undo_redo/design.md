## Interface design

Initially we will have an interface, where you can change the image and commit the bounding box that
was changed.
```cpp
using Image = xt::xtensor<float, 2>;
History<Image> vHistory(vImage);
auto& vImage = vHistory.current();
vImage(1, 1) = 5.f;
vImage(2, 2) = 5.f;
vHistory.commit(BBox{{1, 1}, {2, 2}});
```

This underneath will be implemented using actions, so that we can expand it later to include other
types of actions (e.g. reversible ones).
```cpp
void History::commit(BBox aBbox) {
    SaveBBoxAction<Image> vAction(aBbox);
    mActionStorage.push_back(vAction);
}
```

Later we can expose additional interface in `History` for reversible actions:
```cpp
void History::apply_reversible(ReversibleAction* aAction) {
    mActionStorage.push_back(aAction);
}
```

If the image was changed completely (e.g. gauss blur):
```cpp
bda::gaussFilter(vImage);
vHistory.commitWholeDocument();
```

### Examples
Cropping
```cpp
auto& vImage = vHistory.current();
vImage = xt::view(vImage, xt::range(5, 15), xt::range(5, 15));
vHistory.commitWholeDocument();
```


### Alternatives
What if `commit` would always accept some kind of action:
```cpp
// to save bbox to vHistory:
vHistory.commit(BBox{{1, 1}, {2, 2}});

// vHistory.commit(save_bbox_action_t{BBox{}});

// apply reversible action:
vHistory.commit(ReversibleAction{});

// save whole image:
vHistory.commit(save_whole_image_action_t{"gauss blur"});
```

Problem: does `commit` perform the action?
- yes: `ReversibleAction` (maybe we can apply it manually and then commit?)
- no: `save_bbox`, `save_whole`

Maybe we can rename `commit` to `saveChanges` and it'll not perform any actions on image?


## Single undo on multiple objects