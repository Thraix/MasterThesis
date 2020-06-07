#include "ColorPickerWindow.h"

#include <graphics/gui/Label.h>
#include <utils/LogUtils.h>

#include <functional>

namespace Greet
{
  ColorPickerWindow::ColorPickerWindow(const Vec2& pos, const Vec3<float>& color)
  {
    name = "ColorPickerWindow";
    shouldCloseUnfocus = true;
    // Would be easier to create an XMLObject for this.
    // However that would need to be integrated with the engine and
    // I have no idea how to do this.

    Vec3<float> hsv(ColorUtils::RGBtoHSV(Vec4(color.r,color.g,color.b,1)));

    Style s = Style{}.SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff263238))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorder({1,1,1,1})
      .SetPadding({10,10,10,10});

    SetVertical(false)
      .SetNormalStyle(s)
      .SetSize(1,1, ComponentSize::Type::WRAP, ComponentSize::Type::WRAP,false);

    svSlider = new SatValSlider("ColorPickerWindow#SatValSlider", this);
    svSlider->SetSize(200,200,ComponentSize::Type::PIXELS,ComponentSize::Type::PIXELS,false);
    svSlider->SetHue(hsv.h);
    svSlider->SetSat(hsv.s);
    svSlider->SetVal(hsv.v);
    AddComponent(svSlider);

    hSlider = new HueSlider("ColorPickerWindow#HueSlider", this);
    hSlider->SetVertical(true)
      .SetSize(20,1, ComponentSize::Type::PIXELS, ComponentSize::Type::WEIGHT, false);
    hSlider->GetSliderComponent()->SetSize(1,7,ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS);
    hSlider->SetValue(hsv.h);
    AddComponent(hSlider);

    Container* textBoxContainer = new Container("ColorPickerWindow#TextBoxContainer",this);
    textBoxContainer->SetSize(110,1,ComponentSize::Type::PIXELS, ComponentSize::Type::WEIGHT);
    AddComponent(textBoxContainer);

    Style textBoxStyle = Style{}.SetPadding({0, 5, 0, 5})
      .SetRadius(3)
      .SetBorder({1,1,1,1})
      .SetBorderRadius(4)
      .SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff455a64));

    {
      Container* rgbhsvContainer = new Container("ColorPickerWindow#RHTextBoxContainer",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("ColorPickerWindow#RTextBoxContainer",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
          Label* text = new Label("ColorPickerWindow#RLabel",this, "R", "noto",16);
          text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
          rgbContainer->AddComponent(text);

          rTextBox = new TextBox("ColorPickerWindow#RTextBox",this);

          rTextBox->SetFont("noto")
            .SetFontSize(16)
            .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(rTextBox);
        }
      }
      {
        Container* hsvContainer = new Container("ColorPickerWindow#HTextBoxContainer",this);
        hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
        rgbhsvContainer->AddComponent(hsvContainer);
        {
          Label* text = new Label("ColorPickerWindow#HLabel",this, "H", "noto",16);
          text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
          hsvContainer->AddComponent(text);

          hTextBox = new TextBox("ColorPickerWindow#HTextBox",this);

          hTextBox->SetFont("noto")
            .SetFontSize(16)
            .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
            .SetNormalStyle(textBoxStyle);

          hsvContainer->AddComponent(hTextBox);
        }
      }
    }
    {
      Container* rgbhsvContainer = new Container("ColorPickerWindow#GSTextBoxContainer",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("ColorPickerWindow#GTextBoxContainer",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
          Label* text = new Label("ColorPickerWindow#GLabel",this, "G", "noto",16);
          text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
          rgbContainer->AddComponent(text);

          gTextBox = new TextBox("ColorPickerWindow#GTextBox",this);

          gTextBox->SetFont("noto")
            .SetFontSize(16)
            .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(gTextBox);
        }

      }

      Container* hsvContainer = new Container("ColorPickerWindow#STextBoxContainer",this);
      hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
      rgbhsvContainer->AddComponent(hsvContainer);
      {
        Label* text = new Label("ColorPickerWindow#SLabel",this, "S", "noto",16);
        text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
        hsvContainer->AddComponent(text);

        sTextBox = new TextBox("ColorPickerWindow#STextBox",this);

        sTextBox->SetFont("noto")
          .SetFontSize(16)
          .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
          .SetNormalStyle(textBoxStyle);

        hsvContainer->AddComponent(sTextBox);
      }
    }
    {
      Container* rgbhsvContainer = new Container("ColorPickerWindow#BVTextBoxContainer",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("ColorPickerWindow#BTextBoxContainer",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
        Label* text = new Label("ColorPickerWindow#BLabel",this, "B", "noto",16);
        text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
          rgbContainer->AddComponent(text);

          bTextBox = new TextBox("ColorPickerWindow#BTextBox",this);

          bTextBox->SetFont("noto")
            .SetFontSize(16)
            .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(bTextBox);
        }

      }

      Container* hsvContainer = new Container("ColorPickerWindow#VTextBoxContainer",this);
      hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
      rgbhsvContainer->AddComponent(hsvContainer);
      {
        Label* text = new Label("ColorPickerWindow#VLabel",this, "V", "noto",16);
        text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
        hsvContainer->AddComponent(text);

        vTextBox = new TextBox("ColorPickerWindow#VTextBox",this);

        vTextBox->SetFont("noto")
          .SetFontSize(16)
          .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
          .SetNormalStyle(textBoxStyle);

        hsvContainer->AddComponent(vTextBox);
      }
    }

    Container* hexContainer = new Container("ColorPickerWindow#HexTextBoxContainer",this);
    hexContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WRAP);
    textBoxContainer->AddComponent(hexContainer);
    {
      Label* text = new Label("ColorPickerWindow#HexLabel",this, "#", "noto",16);
      text->SetSize(1,1,ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);
      hexContainer->AddComponent(text);

      hexTextBox = new TextBox("ColorPickerWindow#HexTextBox",this);

      hexTextBox->SetFont("noto")
        .SetFontSize(16)
        .SetSize(1,20, ComponentSize::Type::WEIGHT, ComponentSize::Type::PIXELS)
        .SetNormalStyle(textBoxStyle);

      hexContainer->AddComponent(hexTextBox);
    }
    colorDisplay = new Component("ColorPickerWindow#ColorDisplay",this);
    Style colorStyle = Style{}.SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff455a64))
      .SetBorder({1,1,1,1});
    colorDisplay->SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WEIGHT)
      .SetNormalStyle(colorStyle);
    textBoxContainer->AddComponent(colorDisplay);

    using namespace std::placeholders;

    hSlider->SetOnValueChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    svSlider->SetOnSatChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    svSlider->SetOnValChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    rTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    gTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    bTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    hTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    sTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    vTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    hexTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HexTextBoxChanged, std::ref(*this)));

    // Make textboxes and other stuff update
    SliderChanged();
    SetPosition(pos);

  }
  ColorPickerWindow::ColorPickerWindow()
    : ColorPickerWindow {{0,0}, {1,1,1}}
  {
  }

  void ColorPickerWindow::UpdateColor(float hue, float sat, float val, InputChangeType type)
  {
    Style s = colorDisplay->GetNormalStyle();
    Vec3<float> prevRGB = color;
    Vec4 rgb = ColorUtils::HSVtoRGB(hue,sat,val,1);
    color = Vec3<float>(rgb);
    s.SetBackgroundColor(rgb);
    colorDisplay->SetNormalStyle(s);
    svSlider->SetHue(hue);

    if(type != InputChangeType::RGB_TEXTBOX)
    {
      rTextBox->SetText(std::to_string((int)(255*rgb.r)));
      gTextBox->SetText(std::to_string((int)(255*rgb.g)));
      bTextBox->SetText(std::to_string((int)(255*rgb.b)));
    }
    if(type != InputChangeType::HSV_TEXTBOX)
    {
      hTextBox->SetText(std::to_string((int)(255*hue)));
      sTextBox->SetText(std::to_string((int)(255*sat)));
      vTextBox->SetText(std::to_string((int)(255*val)));
    }
    if(type != InputChangeType::SLIDER)
    {
      hSlider->SetValue(hue);
      svSlider->SetSat(sat);
      svSlider->SetVal(val);
    }
    if(type != InputChangeType::HEX_TEXTBOX)
    {
      hexTextBox->SetText(LogUtils::DecToHex(((int)(255*rgb.r) << 16) | ((int)(255*rgb.g) << 8)  | (int)(255*rgb.b),6));
    }
    if(Vec3<float>(rgb) != prevRGB)
      CallOnColorChangeCallback(prevRGB, Vec3<float>(rgb));
  }

  void ColorPickerWindow::SliderChanged()
  {
    UpdateColor(hSlider->GetValue(), svSlider->GetSat(),svSlider->GetVal(), InputChangeType::SLIDER);
  }

  void ColorPickerWindow::RGBTextBoxChanged(Component* textBox)
  {
    float r = atof(rTextBox->GetText().c_str())/255.0f;
    float g = atof(gTextBox->GetText().c_str())/255.0f;
    float b = atof(bTextBox->GetText().c_str())/255.0f;

    Math::Clamp(&r,0.0f,1.0f);
    Math::Clamp(&g,0.0f,1.0f);
    Math::Clamp(&b,0.0f,1.0f);

    Vec4 hsv = ColorUtils::RGBtoHSV(r,g,b,1);
    UpdateColor(hsv.h,hsv.s,hsv.v,InputChangeType::RGB_TEXTBOX);
  }

  void ColorPickerWindow::HSVTextBoxChanged(Component* textBox)
  {
    float h = atoi(hTextBox->GetText().c_str())/255.0f;
    float s = atoi(sTextBox->GetText().c_str())/255.0f;
    float v = atoi(vTextBox->GetText().c_str())/255.0f;
    Math::Clamp(&h,0.0f,1.0f);
    Math::Clamp(&s,0.0f,1.0f);
    Math::Clamp(&v,0.0f,1.0f);
    UpdateColor(h,s,v,InputChangeType::HSV_TEXTBOX);
  }

  void ColorPickerWindow::HexTextBoxChanged()
  {
    Vec4 rgba = ColorUtils::HexToVec4(LogUtils::HexToDec(hexTextBox->GetText()));
    Vec4 hsv = ColorUtils::RGBtoHSV(rgba.r,rgba.g,rgba.b,1);
    UpdateColor(hsv.h,hsv.s,hsv.v,InputChangeType::HEX_TEXTBOX);
  }

  void ColorPickerWindow::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
  }

  void ColorPickerWindow::CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(previous, current);
  }
}
