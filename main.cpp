#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

const int BASE_SCREEN_WIDTH = 1280;
const int BASE_SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL3 initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("SDL3 Concrete Panel Gallery Wall", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
        std::cerr << "Window/Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderLogicalPresentation(renderer, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    std::cout << "[Asset Check] Loading asset textures natively..." << std::endl;

    SDL_Texture* backgroundTexture = nullptr;
    SDL_Surface* bgSurface = SDL_LoadBMP("C:/Users/USER/source/repos/GROUP3/haunted2.bmp");

    SDL_FRect bgDestRect = { 0.0f, 0.0f, static_cast<float>(BASE_SCREEN_WIDTH), static_cast<float>(BASE_SCREEN_HEIGHT) };

    if (!bgSurface) {
        std::cout << " -> Warning: 'haunted2.bmp' not found! Using solid color fallback. Error: " << SDL_GetError() << std::endl;
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_DestroySurface(bgSurface);

        if (backgroundTexture) {
            std::cout << " -> Successfully loaded custom concrete background image!" << std::endl;

            float bgWidth = 0.0f, bgHeight = 0.0f;
            SDL_GetTextureSize(backgroundTexture, &bgWidth, &bgHeight);

            float windowAspect = static_cast<float>(BASE_SCREEN_WIDTH) / static_cast<float>(BASE_SCREEN_HEIGHT);
            float textureAspect = bgWidth / bgHeight;

            if (textureAspect > windowAspect) {
                bgDestRect.h = static_cast<float>(BASE_SCREEN_HEIGHT);
                bgDestRect.w = bgDestRect.h * textureAspect;
                bgDestRect.x = (static_cast<float>(BASE_SCREEN_WIDTH) - bgDestRect.w) / 2.0f;
                bgDestRect.y = 0.0f;
            }
            else {
                bgDestRect.w = static_cast<float>(BASE_SCREEN_WIDTH);
                bgDestRect.h = bgDestRect.w / textureAspect;
                bgDestRect.x = 0.0f;
                bgDestRect.y = (static_cast<float>(BASE_SCREEN_HEIGHT) - bgDestRect.h) / 2.0f;
            }

            SDL_SetTextureColorMod(backgroundTexture, 90, 100, 120);
        }
    }

    std::vector<std::string> imagePaths = {
        "C:/Users/USER/source/repos/GROUP3/Rendered_image.bmp",
        "C:/Users/USER/source/repos/GROUP3/SCENE 2(rendered).bmp",
        "C:/Users/USER/source/repos/GROUP3/OpenGl.bmp"
    };
    std::vector<SDL_Texture*> textures;
    for (const auto& path : imagePaths) {
        SDL_Texture* texture = nullptr;
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());

        if (!surface) {
            std::cout << " -> Warning: Could not load scene '" << path << "'. Error: " << SDL_GetError() << std::endl;
        }
        else {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
        }
        textures.push_back(texture);
    }

    float topWidth = 480.0f;
    float topHeight = 280.0f;
    float bottomSize = 280.0f;

    float outerBorderThickness = 12.0f;
    float innerMattingThickness = 6.0f;

    float topRowY = 50.0f;
    float topRowGap = 80.0f;
    float topRowTotalWidth = (topWidth * 2.0f) + topRowGap;
    float topStartX = (static_cast<float>(BASE_SCREEN_WIDTH) - topRowTotalWidth) / 2.0f;

    float bottomRowY = topRowY + topHeight + 60.0f;
    float bottomStartX = (static_cast<float>(BASE_SCREEN_WIDTH) - bottomSize) / 2.0f;

    bool quit = false;
    bool isFullscreen = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_F11 || e.key.key == SDLK_ESCAPE) {
                    isFullscreen = !isFullscreen;
                    SDL_SetWindowFullscreen(window, isFullscreen);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 22, 26, 255);
        SDL_RenderClear(renderer);

        if (backgroundTexture != nullptr) {
            SDL_RenderTexture(renderer, backgroundTexture, nullptr, &bgDestRect);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(renderer, 10, 12, 14, 240);
        for (int i = 0; i < 3; ++i) {
            float currentX = 0.0f;
            float currentY = 0.0f;
            float currentW = 0.0f;
            float currentH = 0.0f;

            if (i == 0) { currentX = topStartX; currentY = topRowY; currentW = topWidth; currentH = topHeight; }
            else if (i == 1) { currentX = topStartX + topWidth + topRowGap; currentY = topRowY; currentW = topWidth; currentH = topHeight; }
            else { currentX = bottomStartX; currentY = bottomRowY; currentW = bottomSize; currentH = bottomSize; }

            float dripStartY = currentY + currentH + outerBorderThickness;

            SDL_RenderLine(renderer, currentX + 35.0f, dripStartY, currentX + 35.0f, dripStartY + 50.0f);
            SDL_RenderLine(renderer, currentX + (currentW * 0.25f), dripStartY, currentX + (currentW * 0.25f), dripStartY + 80.0f);
            SDL_RenderLine(renderer, currentX + (currentW * 0.5f), dripStartY, currentX + (currentW * 0.5f), dripStartY + 95.0f);
            SDL_RenderLine(renderer, currentX + (currentW * 0.75f), dripStartY, currentX + (currentW * 0.75f), dripStartY + 60.0f);
            SDL_RenderLine(renderer, currentX + currentW - 35.0f, dripStartY, currentX + currentW - 35.0f, dripStartY + 45.0f);
        }

        for (int i = 0; i < 3; ++i) {
            float currentX = 0.0f;
            float currentY = 0.0f;
            float currentW = 0.0f;
            float currentH = 0.0f;

            if (i == 0) { currentX = topStartX; currentY = topRowY; currentW = topWidth; currentH = topHeight; }
            else if (i == 1) { currentX = topStartX + topWidth + topRowGap; currentY = topRowY; currentW = topWidth; currentH = topHeight; }
            else { currentX = bottomStartX; currentY = bottomRowY; currentW = bottomSize; currentH = bottomSize; }

            SDL_FRect outerFrame = {
                currentX - outerBorderThickness,
                currentY - outerBorderThickness,
                currentW + (outerBorderThickness * 2.0f),
                currentH + (outerBorderThickness * 2.0f)
            };
            SDL_SetRenderDrawColor(renderer, 18, 20, 24, 255);
            SDL_RenderFillRect(renderer, &outerFrame);

            SDL_FRect innerMolding = {
                currentX - innerMattingThickness,
                currentY - innerMattingThickness,
                currentW + (innerMattingThickness * 2.0f),
                currentH + (innerMattingThickness * 2.0f)
            };
            SDL_SetRenderDrawColor(renderer, 8, 9, 12, 255);
            SDL_RenderFillRect(renderer, &innerMolding);

            SDL_FRect imageRect = { currentX, currentY, currentW, currentH };

            if (textures[i] != nullptr) {
                SDL_SetTextureColorMod(textures[i], 255, 255, 255);
                SDL_RenderTexture(renderer, textures[i], nullptr, &imageRect);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 35, 40, 48, 255);
                SDL_RenderFillRect(renderer, &imageRect);
            }
        }

        SDL_FRect groundFog = { 0.0f, 650.0f, 1280.0f, 70.0f };
        SDL_SetRenderDrawColor(renderer, 15, 18, 22, 110);
        SDL_RenderFillRect(renderer, &groundFog);

        SDL_RenderPresent(renderer);
    }

    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    for (auto* texture : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
