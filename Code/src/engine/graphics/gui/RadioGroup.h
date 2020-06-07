#pragma once

#include <graphics/gui/Container.h>

namespace Greet
{
  class RadioButton;

  class RadioGroup : public Container
  {
    private:
      REGISTER_COMPONENT_DECLARATION(RadioGroup);
    public:
      friend class RadioButton;
      typedef std::function<void(RadioButton*)> OnRadioChangeCallback;
    protected:
      RadioButton* activeButton;
      OnRadioChangeCallback onRadioChangeCallback;
    public:
      RadioGroup(const XMLObject& xmlObject, Component* parent);
      void SetOnRadioChangeCallback(OnRadioChangeCallback callback);
    protected:
      void NotifyRadioChange(RadioButton* radioButton);
      void CallOnRadioChangeCallback(RadioButton* radioButton);
  };
}
