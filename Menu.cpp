#include "Menu.h"
#include "StringUtilities.h"
#include "Settings.h"

using namespace std;

#pragma region Fields

// Colors & Fonts
const string NORMAL_COLOR = config.get("Appearance", "Normal", "200, 200, 200");
const string ACTIVE_COLOR = config.get("Appearance", "Active", "116, 230, 101");
const string FONT_FAMILY  = config.get("Appearance", "Font", "Atari.ttf");
const int FONT_SIZE       = config.get("Appearance", "FontSize", 14);

// Screen Padding & Distance between items
const int X_PAD           = config.get("Appearance", "PaddingX", 20);
const int Y_PAD           = config.get("Appearance", "PaddingY", 20);
const int X_STEP          = config.get("Appearance", "StepX", 300);
const int Y_STEP          = config.get("Appearance", "StepY", 30);

// Margin/Threshold for scrolling & Distance to scroll
const int SCROLL_MARGIN   = config.get("Behavior", "ScrollMargin", 50);
const int SCROLL_AMOUNT   = config.get("Behavior", "ScrollAmount", SCROLL_MARGIN * 2);

#pragma endregion

#pragma region Text Item

// Add child item
shared_ptr<TextItem> TextItem::Add(const string value) {
    auto result    = make_shared<TextItem>();
    result->_value = value;
    result->menu   = menu;
    result->_rect  = { 0, 0 };

    // Set position of nth child element
    auto numChildren = (int)children.size();
    result->SetPosition(
        X_PAD + X_STEP,
        Y_PAD + Y_STEP * numChildren);

    children.push_back(result);
    return result;
}

// Event triggered when text item is clicked
void TextItem::OnClick(function<void()> callback) {
    _callback = callback;
}

// Call callback (if it exists)
void TextItem::TriggerClick() {
    if (_callback != nullptr)
        _callback();
}

// Draw the item to its pre-cached textures
void TextItem::DrawText(SDL_Renderer* renderer) {
    const auto rgb_normal = split(NORMAL_COLOR, ',');
    const auto rgb_active = split(ACTIVE_COLOR, ',');

    // Draw the item to its pre-cached textures
    auto drawText = [&](bool active) {
#define RGB(p) (Uint8)stoi(p[0]), (Uint8)stoi(p[1]), (Uint8)stoi(p[2])

        auto color = active ?
            SDL_Color{ RGB(rgb_active) } :
            SDL_Color{ RGB(rgb_normal) };

        auto textSurface = TTF_RenderText_Blended(
            menu->getFont(), _value.c_str(), color);

        auto textTexture = SDL_CreateTextureFromSurface(
            renderer, textSurface);

        SDL_QueryTexture(textTexture, nullptr, nullptr, &_rect.w, &_rect.h);
        SDL_FreeSurface(textSurface);
        return textTexture;
    };

    _tex_Normal = drawText(false);
    _tex_Active = drawText(true);
    _rendered = true;
}

// Draw the item and all its immediate children to output
void TextItem::Render(SDL_Renderer* renderer, bool isActive, int offset) {
    if (_rendered == false)
        DrawText(renderer);

    // Pick the texture
    auto texture = isActive ? _tex_Active : _tex_Normal;

    // Increment by offset
    auto rect(_rect);
    rect.y += offset;

    // Render the current node
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    if (isActive)
        // Render all child nodes
        for (auto child : children)
            child->Render(renderer, child.get() == active.get(), _offset);
}

void TextItem::ResetTextures(bool disposeChildren) {
    if (_rendered) {
        SDL_DestroyTexture(_tex_Normal);
        SDL_DestroyTexture(_tex_Active);

        if (disposeChildren)
            for (auto child : children)
                child->ResetTextures(true);

        _rendered = false;
    }
}


// Cleanup
TextItem::~TextItem() {
    ResetTextures(false);
}

// Set the position of the textitem
void TextItem::SetPosition(int x, int y) {
    _rect.x = x;
    _rect.y = y;
}

#pragma endregion

#pragma region Menu

Menu::Menu() : _yOffset(Y_PAD) {
    // Load font
    LoadFont();

    // Create root text item
    _root = make_shared<TextItem>();
    _root->menu = this;

    // Create audio
    _audio = make_shared<Audio>();
}

Menu::~Menu() {
    _root->ResetTextures(true);

    if (_font != nullptr)
        TTF_CloseFont(_font);

    _root.reset();
    _audio.reset();
}

void Menu::LoadFont() {
    // Create font
    _font = TTF_OpenFont(FONT_FAMILY.c_str(), FONT_SIZE);

    if (_font == nullptr) {
        cout << "Failed to find specified font: " << TTF_GetError() << std::endl;
        exit(1);
    }
}

// Render all child nodes
void Menu::Render(SDL_Renderer* renderer) {
    auto offset(_root->getOffset());
    auto active(_root->active.get());

    for (auto child : _root->children)
        child->Render(renderer, child.get() == active, offset);
}

// Add a child item
shared_ptr<TextItem> Menu::Add(const string value) {
    auto result(_root->Add(value));

    if (!_root->active)
        _root->active = result;

    result->SetPosition(X_PAD, _yOffset);
    _yOffset += Y_STEP;

    return result;
}

// Navigate UP / DOWN / BACK
void Menu::TriggerNavigate(MoveDirection direction) {
    // Detect level
    auto parentItem(_root->active);
    if (!parentItem->active)
        parentItem = _root;

    // Find current item's index
    auto firstItem = parentItem->children.begin();
    auto matches   = find(firstItem, parentItem->children.end(), parentItem->active);
    auto index     = distance(firstItem, matches);

    switch (direction) {
        case NAV_UP: // Go to item above
            if (index > 0)
                parentItem->active = parentItem->children[index - 1];
            break;

        case NAV_DOWN: // Go to item below
            if (index < parentItem->children.size() - 1)
                parentItem->active = parentItem->children[index + 1];
            break;

        case NAV_BACK: // Go back
            if (parentItem != _root)
                parentItem->active = nullptr;
            break;
    }

    // Handle scrolling
    if (direction == NAV_UP || direction == NAV_DOWN) {
        // Check scroll down
        auto scroll_dir = CheckScroll(parentItem);

        if ((scroll_dir == -1 && direction == NAV_DOWN) ||
            (scroll_dir == 1 && direction == NAV_UP)) {
            auto offset = parentItem->getOffset()
                + SCROLL_AMOUNT * scroll_dir;

            if (offset > 0)
                offset = 0;

            parentItem->setOffset(offset);
        }
    }

    // Play 'Menu' audio
    _audio->Play(MenuAudio);
}

// Navigate Forward / Into
void Menu::TriggerClick(bool commit) {
    auto parentItem  = _root->active;
    auto currentItem = parentItem->active;

    if (currentItem) {
        if (commit) // Trigger action
            currentItem->TriggerClick();
    }
    else if (parentItem->children.size()) {
        // Navigate to first child item
        parentItem->active = parentItem->children[0];
        parentItem->setOffset(0); // Reset offset
    }
}

void Menu::ResetTextures() {
    _root->ResetTextures(true);

    // Re-load font
    if (_font != nullptr)
        TTF_CloseFont(_font);
    
    LoadFont();
}

int Menu::CheckScroll(const shared_ptr<TextItem> item) {
    int // Comparison of position of active item w/ margin
        top_margin    = Y_PAD,
        bottom_margin = _screenHeight - Y_PAD,

        // Current position of the active item on the screen
        screen_y = item->active->getPosition()
                 + item->getOffset();

    // Scroll up required
    if (screen_y < top_margin + SCROLL_MARGIN)
        return 1;

    // Scroll down required
    else if (screen_y > bottom_margin - SCROLL_MARGIN)
        return -1;

    return 0;
}

#pragma endregion