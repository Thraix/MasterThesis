#include "RadioGroup.h"

#include <graphics/gui/RadioButton.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(RadioGroup);

  RadioGroup::RadioGroup(const XMLObject& xmlObject, Component* parent)
    : Container(xmlObject, parent), activeButton(nullptr)
  {
  }

  void RadioGroup::NotifyRadioChange(RadioButton* radioButton)
  {
    if(activeButton)
      activeButton->Select(false);
    activeButton = radioButton;
    CallOnRadioChangeCallback(radioButton);
  }

  void RadioGroup::SetOnRadioChangeCallback(OnRadioChangeCallback callback)
  {
    onRadioChangeCallback = callback;
  }

  void RadioGroup::CallOnRadioChangeCallback(RadioButton* button)
  {
    if(onRadioChangeCallback)
      onRadioChangeCallback(button);
  }
}
