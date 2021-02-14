#pragma once

#include <SDL_rect.h>

struct SDL_RWops;
struct SDL_Renderer;
struct SDL_Texture;

class SDLBitmapFont
{
public:
    SDLBitmapFont();
    ~SDLBitmapFont();

    bool Build(SDL_Renderer* renderer, SDL_RWops* bmp_fontsheet_file, SDL_Color char_rgb = {0x00, 0x00, 0x00, 0x00}, SDL_Color background_rgb = {0xFF, 0xFF, 0xFF, 0xFF});
    bool BuildFromFile(SDL_Renderer* renderer, const char* bmp_fontsheet_path, SDL_Color char_rgb = {0x00, 0x00, 0x00, 0x00}, SDL_Color background_rgb = {0xFF, 0xFF, 0xFF, 0xFF});
    void RenderText(SDL_Renderer* renderer, int32_t x, int32_t y, const char* text, float scale = 1.0f);

private:
    SDL_Texture* m_texture;
    int m_width;
    int m_height;
    int m_newlineHeight;
    int m_spaceWidth;
    SDL_Rect m_charRects[16 * 16];
};
