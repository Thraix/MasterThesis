#include "RadioButton.h"

#include <graphics/gui/RadioGroup.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(RadioButton);

  RadioButton::RadioButton(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), active(false)
  {
    m_isFocusable = true;
    activeStyle.Load("active", xmlObject, &normalStyle);
  }

  void RadioButton::PostConstruction()
  {
    radioParent = FindRadioGroupParent();
    if(!radioParent)
      Log::Warning("RadioButton is not inside a RadioGroup");
  }

  void RadioButton::CallOnClickCallback()
  {
    if(radioParent)
      radioParent->NotifyRadioChange(this);
    Select(true);
    Component::CallOnClickCallback();
  }

  void RadioButton::Select(bool select)
  {
    active = select;
    if(active)
      currentStyle = &activeStyle;
    else
      currentStyle = &normalStyle;
  }

  RadioGroup* RadioButton::FindRadioGroupParent() const
  {
    Component* p = parent;
    RadioGroup* rp = dynamic_cast<RadioGroup*>(p);
    while(!rp && p->GetParent())
    {
      p = p->GetParent();
      rp = dynamic_cast<RadioGroup*>(p);
    }
    return rp;
  }
}
