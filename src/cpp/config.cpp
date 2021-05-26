#include <iostream>

template<class Derived, class Settings>
class Configurable {
public:
    Configurable() {}
    Configurable(Settings settings) : _settings(std::move(settings)) {}

    void setSettings(const Settings& settings) {
        _settings = settings;
        (*static_cast<Derived*>(this)).settingsChanged();
    }

    Settings _settings;
};

class WindowSettings {};

template<class Settings>
class Window : public Configurable<Window<Settings>, Settings> {
public:
    Window(Settings settings) : Configurable<Window<Settings>, Settings>(std::move(settings)) {}

    void settingsChanged() { std::cout << "settings changed\n"; }
};