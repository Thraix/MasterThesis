#include "TextBox.h"

#include <input/InputDefines.h>
#include <utils/StringUtils.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(TextBox);

  TextBox::TextBox(const std::string& name, Component* parent)
    : Component(name,parent), cursorPos(0), selectionPos(0), cursorBlinkTimer(0), textOffset{0}, password{false}, ctrlDown{false}, shiftDown{false}
  {
    m_isFocusable = true;

    text = new Label("Label", this, "", "noto", 20);

    text->SetColor(Vec4(0,0,0,1))
      .SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WEIGHT);

    hintText = new Label("HintLabel", this, "", "noto",20);
    hintText->SetColor(Vec4(0,0,0,1))
      .SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WEIGHT);
  }

  TextBox::TextBox(const XMLObject& object, Component* parent)
    : Component(object,parent), cursorPos(0), selectionPos(0), cursorBlinkTimer(0), textOffset{0}, ctrlDown{false}, shiftDown{false}
  {
    m_isFocusable = true;

    text = new Label("Label",this,
        object.GetText(),
        GUIUtils::GetStringFromXML(object, "font",""),
        GUIUtils::GetFloatFromXML(object,"fontSize",20));

    text->SetColor(GUIUtils::GetColorFromXML(object, "color", Vec4(0,0,0,1)))
      .SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WEIGHT);

    hintText = new Label("HintLabel", this,
        GUIUtils::GetStringFromXML(object, "hintText", ""),
        GUIUtils::GetStringFromXML(object, "font",text->GetFont()->GetFontContainer()->GetName()),
        GUIUtils::GetFloatFromXML(object,"fontSize",text->GetFontSize()));

    hintText->SetColor(GUIUtils::GetColorFromXML(object, "hintColor", text->GetColor()))
      .SetSize(1,1,ComponentSize::Type::WEIGHT, ComponentSize::Type::WEIGHT);

    password = GUIUtils::GetBooleanFromXML(object, "password", false);
  }

  TextBox::~TextBox()
  {
    delete text;
    delete hintText;
  }

  void TextBox::OnMeasured()
  {
    text->Measure();
    hintText->Measure();
    text->MeasureFill(GetContentSize().w,GetContentSize().h, 1, true);
    hintText->MeasureFill(GetContentSize().w,GetContentSize().h, 1, true);
  }

  void TextBox::Render(GUIRenderer* renderer) const
  {
    renderer->PushViewport(pos+GetTotalPadding(), GetContentSize(), true);
    if(text->GetText().length() == 0)
    {
      Vec2 p = pos + GetTotalPadding() +  Vec2(-textOffset, 0);
      renderer->PushTranslation(p);
      hintText->Render(renderer);
    }
    else
    {
      Vec2 p = pos + GetTotalPadding() +  Vec2(-textOffset, 0);
      renderer->PushTranslation(p);
      text->Render(renderer);
    }

    renderer->PopViewport();
    renderer->PopTranslation();

    if(isFocused && cursorBlinkTimer < 0.5)
    {
      float p = text->GetFont()->GetWidthOfText(text->GetText(),0, cursorPos)-textOffset;
      Vec2 curPos = pos + GetTotalPadding() + Vec2(p, (GetContentSize().h - text->GetHeight())/2);
      renderer->SubmitRect(curPos,Vec2(1,text->GetHeight()),text->GetColor(),false);
    }
  }

  void TextBox::Update(float timeElapsed)
  {
    cursorBlinkTimer += timeElapsed;
    if(cursorBlinkTimer >= 1)
      cursorBlinkTimer -= 1;
  }

  void TextBox::OnEvent(Event& event, const Vec2& translatedPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1)
      {
        // TODO: In the future we probably want to do some smart, average character length
        // to determain around where the cursor should be.
        std::vector<float> widths{text->GetFont()->GetPartialWidths(str)};
        auto it{widths.begin()};
        float w = -textOffset + GetTotalPadding().w;
        uint index = 0;
        while(it != widths.end() && w+*it <= translatedPos.x)
        {
          ++it;
          index++;
        }
        // The cursor should be at the cursor behind
        // Maybe change to the nearest cursor?
        index--;
        MoveCursor(index - cursorPos);
        cursorBlinkTimer = 0;
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
    {
      KeyPress(static_cast<KeyPressEvent&>(event));
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
    {
      KeyReleaseEvent& e = static_cast<KeyReleaseEvent&>(event);
      if(e.GetButton() == GREET_KEY_LEFT_CONTROL)
        ctrlDown = false;
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_TYPE))
    {
      KeyTypeEvent& e = static_cast<KeyTypeEvent&>(event);
      std::string before = str;
      auto it = str.begin();
      std::advance(it, cursorPos);
      str.insert(it, (char)e.GetButton());
      SetText(str);
      MoveCursor(1);
      CallOnTextChangedCallback(before,str);
    }
  }

  void TextBox::KeyPress(KeyPressEvent& event)
  {
    // Easier to type if the blinker is turned on while typing
    cursorBlinkTimer = 0;
    if(event.GetButton() == GREET_KEY_LEFT_CONTROL)
    {
      ctrlDown = true;
    }
    else if(event.GetButton() == GREET_KEY_LEFT)
    {
      if(ctrlDown)
        MoveCursorWord(false);
      else
        MoveCursor(-1);
    }
    else if(event.GetButton() == GREET_KEY_RIGHT)
    {
      if(ctrlDown)
        MoveCursorWord(true);
      else
        MoveCursor(1);
    }
    else if(event.GetButton() == GREET_KEY_BACKSPACE)
    {
      if(cursorPos > 0)
      {
        if(ctrlDown)
        {
          int lastCursorPos = cursorPos;
          MoveCursorWord(false);
          RemoveText(cursorPos, lastCursorPos - cursorPos);
          RecenterText();
        }
        else
        {
          RemoveText(cursorPos-1,1);
          MoveCursor(-1);
        }
      }
    }
    else if(event.GetButton() == GREET_KEY_DELETE)
    {
      if(cursorPos < str.length())
      {
        if(ctrlDown)
        {
          int lastCursorPos = cursorPos;

          // Save the last offset since we dont want MoveCursor to update it
          MoveCursorWord(true);

          RemoveText(lastCursorPos,cursorPos - lastCursorPos);
          cursorPos = lastCursorPos;
          RecenterText();
        }
        else
        {
          RemoveText(cursorPos,1);
        }
      }
    }
    else if(event.GetButton() == GREET_KEY_HOME)
    {
      MoveCursor(-cursorPos);
    }
    else if(event.GetButton() == GREET_KEY_END)
    {
      MoveCursor(text->GetText().length()-cursorPos);
    }
    else if(event.GetButton() == GREET_KEY_PAGE_UP)
    {
      // TODO: When multiline is implemented move to top of page
      MoveCursor(-cursorPos);
    }
    else if(event.GetButton() == GREET_KEY_PAGE_DOWN)
    {
      // TODO: When multiline is implemented move to bottom of page
      MoveCursor(text->GetText().length()-cursorPos);
    }
  }

  const std::string& TextBox::GetText() const
  {
    return text->GetText();
  }

  TextBox& TextBox::SetText(const std::string& text)
  {
    if(password)
      this->text->SetText(StringUtils::Passwordify(text));
    else
      this->text->SetText(text);
    str = text;
    return *this;
  }

  TextBox& TextBox::SetFont(const std::string& fontName)
  {
    text->SetFont(fontName);
    return *this;
  }

  TextBox& TextBox::SetFontSize(float fontSize)
  {
    text->SetFontSize(fontSize);
    return *this;
  }

  TextBox& TextBox::SetHintFont(const std::string& fontName)
  {
    hintText->SetFont(fontName);
    return *this;
  }

  TextBox& TextBox::SetHintFontSize(float fontSize)
  {
    hintText->SetFontSize(fontSize);
    return *this;
  }



  void TextBox::RemoveText(uint start, uint n = -1)
  {
    std::string before = str;
    if(n == -1)
      n = str.length()-start;
    str.erase(start,n);
    SetText(str);
    CallOnTextChangedCallback(before,str);
  }

  void TextBox::RecenterText()
  {
    float cursorTextWidth = text->GetFont()->GetWidthOfText(text->GetText(),0,cursorPos);
    float textWidth = text->GetFont()->GetWidthOfText(text->GetText());
    if(cursorTextWidth - textOffset > GetContentSize().w)
    {
      textOffset = cursorTextWidth - GetContentSize().w;
    }
    else if(cursorTextWidth - textOffset < 0)
    {
      textOffset = cursorTextWidth;
    }
    else if(textWidth >= GetContentSize().w && textWidth - textOffset - GetContentSize().w < 0)
    {
      textOffset = textWidth - GetContentSize().w;
    }
    else if(textWidth <= GetContentSize().w)
    {
      textOffset = 0;
    }
  }

  void TextBox::MoveCursor(int delta)
  {
    cursorPos += delta;
    Math::Clamp<int>(&cursorPos, 0, text->GetText().length());
    RecenterText();
  }

  void TextBox::MoveCursorWord(bool forward)
  {
    int delta = forward ? 1 : -1;
    // Offset the cursor to make it easier to use with delta

    if(!forward)
      cursorPos+=delta;
    StringUtils::CharType type = StringUtils::GetCharType(text->GetText()[cursorPos]);
    while(type == StringUtils::GetCharType(text->GetText()[cursorPos]) && cursorPos >= 0 && cursorPos < text->GetText().length())
    {
      cursorPos += delta;
    }
    Math::Clamp<int>(&cursorPos, 0, text->GetText().length());
    if(type != StringUtils::GetCharType(text->GetText()[cursorPos]) && !forward)
      cursorPos -= delta;

    // If it was a whitespace character we keep going
    if(type == StringUtils::GetCharType(' '))
      MoveCursorWord(forward);
    RecenterText();
  }

  int TextBox::GetCursorPos() const
  {
    return cursorPos;
  }

  int TextBox::GetSelectionPos() const
  {
    return selectionPos;

  }

  void TextBox::SetOnTextChangedCallback(OnTextChangedCallback callback)
  {
    onTextChangedCallback = callback;
  }

  void TextBox::CallOnTextChangedCallback(const std::string& before, const std::string& after)
  {
    if(onTextChangedCallback)
      onTextChangedCallback(this, before, after);
  }

}
