#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <SDL2/SDL_ttf.h>

#include "Audio.h"

class Menu;

enum MoveDirection {
    NAV_UP,
    NAV_DOWN,
    NAV_BACK
};

class TextItem {

public:
    // Members
    std::vector<std::shared_ptr<TextItem>> children;
    std::shared_ptr<TextItem> active;
    Menu* menu;

    // Constructor
    ~TextItem();

    // Methods
    std::shared_ptr<TextItem> Add(const std::string value);
    void OnClick(std::function<void()> callback);
    void Render(SDL_Renderer* renderer, bool isActive, int offset = 0);
    void SetPosition(int x, int y);
    void TriggerClick();
    void ResetTextures(bool disposeChildren = true);
    int getHeight() { return _rect.h; }
    int getPosition() { return _rect.y; }
    void setOffset(int value) { _offset = value; }
    int getOffset() { return _offset; }

private:
    SDL_Rect _rect;
    std::function<void()> _callback;
    std::string _value;
    SDL_Texture *_tex_Normal, *_tex_Active;
    bool _rendered{ false };
    int _offset{ 0 };

    void DrawText(SDL_Renderer* renderer);

};

class Menu {

public:
    Menu();
    ~Menu();

    // Load font resource
    void LoadFont();

    // Render all items
    void Render(SDL_Renderer* renderer);

    // Create and return new menu item
    std::shared_ptr<TextItem> Add(const std::string value);

    // Trigger navigate from active item
    void TriggerNavigate(MoveDirection direction);

    // Trigger click on active item
    void TriggerClick(bool commit);

    // Reset textures
    void ResetTextures();

    // Check if active item is close to a margin
    // -1 = Scroll down
    // 0  = Don't scroll
    // 1  = Scroll up
    int CheckScroll(const std::shared_ptr<TextItem> item);

    TTF_Font* getFont() { return _font; }
    void setScreenHeight(int value) { _screenHeight = value; }

    std::shared_ptr<Audio> getAudio() { return _audio; }

private:
    std::shared_ptr<Audio> _audio;
    TTF_Font* _font;
    std::shared_ptr<TextItem> _root;
    int _yOffset, _screenHeight;

};