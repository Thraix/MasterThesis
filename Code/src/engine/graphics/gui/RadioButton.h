#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/RadioGroup.h>

namespace Greet
{
  class RadioButton : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(RadioButton);
    public:
      friend class RadioGroup;
    protected:
      // Simpler than doing dynamic cast whenever we need RadioGroup
      RadioGroup* radioParent;
      bool active;
      Style activeStyle;
    public:
      RadioButton(const XMLObject& xmlObject, Component* component);
      void PostConstruction() override;

      void CallOnClickCallback() override;

    private:
      void Select(bool select);
      RadioGroup* FindRadioGroupParent() const;
  };

}
