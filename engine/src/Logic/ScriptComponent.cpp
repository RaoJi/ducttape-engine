
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/ScriptComponent.hpp>
#include <Logic/ScriptManager.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

namespace dt {

ScriptComponent::ScriptComponent(const QString& script_name, const QString& name, bool is_update_enabled)
    : Component(name),
      mScriptName(script_name),
      mValid(true),
      mIsUpdateEnabled(is_update_enabled) {
    if(!ScriptManager::Get()->HasScript(mScriptName)) {
        Logger::Get().Error("Cannot create ScriptComponent for script \"" + mScriptName + "\": script not loaded.");
        mValid = false;
    }
}

void ScriptComponent::OnInitialize() {
    if(mValid) {
        // create our QScriptValue, that represents the script object
        mScriptObject = ScriptManager::Get()->GetScriptObject(mScriptName, this);

        mValid = !mScriptObject.isUndefined();
    }

    _CallScriptFunction("OnInitialize");
}

void ScriptComponent::OnDeinitialize() {
    _CallScriptFunction("OnDeinitialize");
}

void ScriptComponent::OnEnable() {}
void ScriptComponent::OnDisable() {}

void ScriptComponent::OnUpdate(double time_diff) {
    if(mIsUpdateEnabled) {
        dt::ScriptManager::Get()->UpdateContext(mScriptObject);
        _CallScriptFunction("OnUpdate", QScriptValueList() << time_diff);
    }
}

QScriptValue ScriptComponent::_CallScriptFunction(QString name, QScriptValueList params) {
    if(!mValid) {
        return QScriptValue::UndefinedValue;
    }

    QScriptValue function = mScriptObject.property(name);
    if(!function.isValid()) {
        // no such function, do not force.
        // Logger::Get().Error("Cannot call function \"" + name + "\" in script \"" + mScriptName + "\": no such function.");
        return QScriptValue::UndefinedValue;
    }

    if(!function.isFunction()) {
        Logger::Get().Error("Cannot call function \"" + name + "\" in script \"" + mScriptName + "\": not a function.");
        return QScriptValue::UndefinedValue;
    }

    QScriptValue value = function.call(mScriptObject, params);
    if(!ScriptManager::Get()->HandleErrors(mScriptName)) {
        Logger::Get().Warning("Error while calling script function. Disabling ScriptComponent \"" + mName + "\".");
        Disable();
    }
    return value;
}

bool ScriptComponent::IsUpdateEnabled() const {
    return mIsUpdateEnabled;
}

void ScriptComponent::SetUpdateEnabled(bool is_enabled) {
    if(is_enabled != mIsUpdateEnabled)
        mIsUpdateEnabled = is_enabled;
}
}
