#pragma once
namespace Greet {
  class Content;

  struct GUIMouseListener
  {
    void OnMousePressed(Content* content) const {}
    void OnMouseReleased(Content* content) const {}
    void OnMouseClicked(Content* content) const {}
  };
}
