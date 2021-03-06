#include <cmath>
#include <cstdio>

#include <SDL.h>
#include "SDLBitmapFont.h"

class Application
{
public:
    Application(int argc, char** argv)
        : m_argc(argc)
        , m_argv(argv)
    {}

    ~Application()
    {}

    int32_t Initialize()
    {
        // Initialize SDL
        uint32_t initFlag = SDL_INIT_VIDEO;

        if (SDL_Init(initFlag) != 0) {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return 1;
        }

        m_window = SDL_CreateWindow("SDLBitmapFontSample", 32, 32, m_screenWidth, m_screenHeight, m_videoFlags);
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);

        // Font
        const SDL_Color colorWhite = {255, 255, 255, 255};
        const SDL_Color colorRed = {255, 0, 0, 255};

        m_fontNormal = new SDLBitmapFont();
        m_fontHeader = new SDLBitmapFont();
        m_fontNormal->BuildFromFile(m_renderer, "../font/VP16Font.bmp", colorWhite);
        m_fontHeader->BuildFromFile(m_renderer, "../font/VP16Font.bmp", colorRed);

        return 0;
    }

    void Finalize()
    {
        delete m_fontNormal;
        delete m_fontHeader;

        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool Main()
    {
        int done = 0;
        SDL_Event event;

        while (!done) {
            while (SDL_PollEvent(&event)) {

                switch (event.type) {
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = 1;
                } break;

                case SDL_QUIT:
                    done = 1;
                    break;

                default:
                    break;
                }
            }

            const SDL_Color colorClear = {0x00, 0x20, 0x60, 0x00};
            SDL_SetRenderDrawColor(m_renderer, colorClear.r, colorClear.g, colorClear.b, colorClear.a);
            SDL_RenderClear(m_renderer);

            const int32_t FW = 16; // Font width
            m_fontHeader->RenderText(m_renderer, 0, FW * 0, "         1UP      HI-SCORE      2UP");
            m_fontNormal->RenderText(m_renderer, 0, FW * 1, "        23450    1234567890    12340");

            m_fontHeader->RenderText(m_renderer, 0, FW * 3, "Alphabets");
            m_fontNormal->RenderText(m_renderer, 0, FW * 4, "A QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");
            m_fontNormal->RenderText(m_renderer, 0, FW * 5, "a quick brown fox jumps over the lazy dog.");

            m_fontHeader->RenderText(m_renderer, 0, FW * 7, "Source Code");
            m_fontNormal->RenderText(m_renderer, 0, FW * 8,
                "int main(int argc, char** argv)\n"
                "{\n"
                "    Application app(argc, argv);\n"
                "    int32_t status = app.Initialize();\n"
                "    if (status == 0) {\n"
                "        app.Main();\n"
                "        app.Finalize();\n"
                "    }\n"
                "    return status;\n"
                "}");

            m_fontHeader->RenderText(m_renderer, 0, FW * 19, "All Characters in VP16Font.bmp");
            m_fontNormal->RenderText(m_renderer, 0, FW * 20,
                " !\"#$%&'()*+,-./\n"
                "0123456789:;<=>?\n"
                "@ABCDEFGHIJKLMNO\n"
                "PQRSTUVWXYZ[\\]^_\n"
                "`abcdefghijklmno\n"
                "pqrstuvwxyz{|}~ ");

            m_fontHeader->RenderText(m_renderer, 0, FW * 27, "License");
            m_fontNormal->RenderText(m_renderer, 0, FW * 28,
                "Sample code and VP16Font are available\n"
                "under the zlib/libpng license. See \n"
                "  http://github.com/vaiorabbit/SDLBitmapFont\n"
                "for details.");

            const char msgExit[] = "<Press [ESC] to exit>";
            int32_t msgLength = FW * (sizeof(msgExit) / sizeof(msgExit[0]));
            int32_t xRightAdjust = m_screenWidth - msgLength;
            m_fontNormal->RenderText(m_renderer, xRightAdjust, FW * 32, msgExit);

            SDL_RenderPresent(m_renderer);
        }

        return 0;
    }

private:
    [[maybe_unused]] int m_argc = 0;
    [[maybe_unused]] char** m_argv = nullptr;

    int m_screenWidth = 720;
    int m_screenHeight = 540;
    unsigned int m_videoFlags = 0;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;

    SDLBitmapFont* m_fontNormal = nullptr;
    SDLBitmapFont* m_fontHeader = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    Application app(argc, argv);

    int32_t status = app.Initialize();
    if (status == 0) {
        app.Main();
        app.Finalize();
    }

    return status;
}
