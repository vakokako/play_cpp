
/** BaseFunctions */

#include <stdexcept>
#include <vector>

class BaseConfig {};
class SerializedSettings {
public:
    bool empty() const;
};

class AppContext {
public:
    template<class T>
    static T* getConfig() {
        for (BaseConfig* vConfig : mConfigs) {
            T* vCastConfig = dynamic_cast<T>(vConfig);
            if (vCastConfig != nullptr) {
                return vCastConfig;
            }
        }
    }

    static void registerConfig(BaseConfig* aConfig) { mConfigs.push_back(aConfig); }

protected:
    static std::vector<BaseConfig*> mConfigs;
};

/** BDAWidgets */
class ColorTransferFunctionControllerSettings : public BaseConfig {
public:
    ColorTransferFunctionControllerSettings(SerializedSettings aDiskSettings);
    // ...
    int getColor() const;
};

class ColorTransferFunctionController {
public:
    ColorTransferFunctionController() {
        const int vColor = AppContext::getConfig<ColorTransferFunctionControllerSettings>()->getColor();
    }
};

/** BDAImView */
class BDAImViewConfiguration {
public:
    BDAImViewConfiguration(SerializedSettings& aDiskSettings) {
        if (!aDiskSettings.empty()) {
            throw(std::runtime_error("Could not deserialize all settings"));
        }

        AppContext::registerConfig(new ColorTransferFunctionControllerSettings(aDiskSettings));
    }
};

class BDAImView {
public:
    BDAImView() {
        SerializedSettings vDiskSettings;

        BDAImViewConfiguration vConfig(vDiskSettings);
    }
};