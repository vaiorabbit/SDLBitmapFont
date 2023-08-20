#include <SDL_rwops.h>
#include <SDL_render.h>

#include "SDLBitmapFont.h"

#include <cmath>
#include <cstring>

SDLBitmapFont::SDLBitmapFont()
    : m_texture(nullptr)
    , m_width(0)
    , m_height(0)
    , m_newlineHeight(0)
    , m_spaceWidth(0)
    , m_charRects()
{}

SDLBitmapFont::~SDLBitmapFont()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

/*
  - width of bmp_file must be multiple of 16
  - bmp_file must contain 16 characters in one line of cells
  - cells for each character in bmp_file must be square-shaped
  [Example]
 ＼ ０１２３４５６７８９ＡＢＣＤＥＦ
0 ｜　　　　　　　　　　　　　　　　
1 ｜　　　　　　　　　　　　　　　　
2 ｜　！”＃＄％＆’（）＊＋，−．／
3 ｜０１２３４５６７８９：；＜＝＞？
4 ｜＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯ
5 ｜ＰＱＲＳＴＵＶＷＸＹＺ［＼］＾＿
6 ｜‘ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏ
7 ｜"ｐｑｒｓｔｕｖｗｘｙｚ｛｜｝〜　
*/
bool SDLBitmapFont::Build(SDL_Renderer* renderer, SDL_RWops* bmp_fontsheet_file, SDL_Color char_rgb, SDL_Color background_rgb)
{
    // Create texture from BMP surface
    SDL_Surface* surface = SDL_LoadBMP_RW(bmp_fontsheet_file, 0);
    if (!surface) {
        return false;
    }

    SDL_Surface* converted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!converted_surface) {
        // Failed to convert into appropreate format
        SDL_FreeSurface(surface);
        return false;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, converted_surface->w, converted_surface->h);
    if (!texture) {
        // Failed to craete texture from surface
        SDL_FreeSurface(converted_surface);
        SDL_FreeSurface(surface);
        return false;
    }

    m_width = converted_surface->w;
    m_height = converted_surface->h;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Copy font image into texture pixels
    void* texture_pixels = nullptr;
    int texture_pitch = 0;
    SDL_LockTexture(texture, &converted_surface->clip_rect, &texture_pixels, &texture_pitch);

    std::memcpy(texture_pixels, converted_surface->pixels, converted_surface->pitch * converted_surface->h);

    // Make background transparent
    uint32_t foreground_color = SDL_MapRGB(converted_surface->format, char_rgb.r, char_rgb.g, char_rgb.b);
    uint32_t background_color = SDL_MapRGB(converted_surface->format, background_rgb.r, background_rgb.g, background_rgb.b);
    uint32_t transparent_color = SDL_MapRGBA(converted_surface->format, 0x00, 0x00, 0x00, 0x00);

    uint32_t* pixels = (uint32_t*)texture_pixels;
    int32_t pixel_count = (texture_pitch / 4) * m_height;

    for (int i = 0; i < pixel_count; ++i) {
        if (pixels[i] == background_color) {
            pixels[i] = transparent_color;
        } else {
            pixels[i] = foreground_color;
        }
    }

    SDL_UnlockTexture(texture);

    SDL_FreeSurface(converted_surface);
    SDL_FreeSurface(surface);

    m_texture = texture;

    // Build array of SDL_Rect
    int rect_width = m_width / 16;
    int rect_height = m_height / 8;
    m_newlineHeight = 16;
    m_spaceWidth = 16;

    int char_code = 0;
    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            m_charRects[char_code].x = rect_width * c;
            m_charRects[char_code].y = rect_height * r;
            m_charRects[char_code].w = rect_width;
            m_charRects[char_code].h = rect_height;
            ++char_code;
        }
    }

    return false;
}

bool SDLBitmapFont::BuildFromFile(SDL_Renderer* renderer, const char* bmp_fontsheet_path, SDL_Color char_rgb, SDL_Color background_rgb)
{
    SDL_RWops* bmp_fontsheet_file = SDL_RWFromFile(bmp_fontsheet_path, "rb");
    bool result = Build(renderer, bmp_fontsheet_file, char_rgb, background_rgb);
    SDL_RWclose(bmp_fontsheet_file);
    return result;
}

void SDLBitmapFont::RenderText(SDL_Renderer* renderer, int32_t x, int32_t y, const char* text, float scale, SDL_Color color)
{
    if (!m_texture) {
        return;
    }

    SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);

    int current_x = x;
    int current_y = y;

    size_t length = strlen(text);
    for (size_t i = 0; i < length; ++i) {
        switch (text[i]) {
        case ' ':
            current_x += (int)std::round(m_spaceWidth * scale);
            break;
        case '\n':
            current_x = x;
            current_y += (int)std::round(m_newlineHeight * scale);
            break;
        default:
            int32_t code = text[i];
            SDL_Rect r = {current_x, current_y, m_charRects[code].w, m_charRects[code].h};
            r.w = (int)std::round(r.w * scale);
            r.h = (int)std::round(r.h * scale);
            SDL_RenderCopy(renderer, m_texture, &m_charRects[code], &r);
            current_x += (int)std::round(m_charRects[code].w * scale);
            break;
        }
    }
}
