#include "npc.h"
#include <iostream>

NPC::NPC(const std::string& name, int x, int y, SDL_Texture* texture)
    : name_(name), x_(x), y_(y), texture_(texture),
    currentDialog_(0), dialogStartTime_(0), dialogActive_(false) {
}

NPC::~NPC() {
    // Do not destroy texture here; manage texture lifetime elsewhere
}   


void NPC::render(SDL_Renderer* renderer) {
    SDL_Rect dstRect = { x_, y_, 64, 64 }; // Example size
    SDL_RenderCopy(renderer, texture_, nullptr, &dstRect);
}

void NPC::talk(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int width, int height) {
    SDL_Rect boxRect = { x_, y_ - height - 10, width, height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &boxRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &boxRect);

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), textColor, width - 10);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect textRect = { x_ + 5, y_ - height - 5, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

// New dialog system methods
void NPC::setDialogs(const std::vector<NPCDialog>& dialogs) {
    dialogs_ = dialogs;
}

void NPC::startDialogSequence() {
    if (!dialogs_.empty()) {
        currentDialog_ = 0;
        dialogStartTime_ = SDL_GetTicks();
        dialogActive_ = true;
    }
}

void NPC::updateDialog() {
    if (!dialogActive_ || dialogs_.empty()) return;
    Uint32 now = SDL_GetTicks();
    if (now - dialogStartTime_ > dialogs_[currentDialog_].duration) {
        currentDialog_++;
        if (currentDialog_ < dialogs_.size()) {
            dialogStartTime_ = now;
        }
        else {
            dialogActive_ = false;
        }
    }
}

void NPC::renderDialog(SDL_Renderer* renderer, TTF_Font* font) {
    if (dialogActive_ && currentDialog_ < dialogs_.size()) {
        const NPCDialog& d = dialogs_[currentDialog_];
        talk(renderer, font, d.text, d.width, d.height);
    }
}
void NPC::nextDialog() {
    if (dialogActive_ && !dialogs_.empty()) {
        currentDialog_++;
        if (currentDialog_ >= dialogs_.size()) {
            dialogActive_ = false;
        }
    }
}
bool NPC::isDialogActive() const {
    return dialogActive_;
}
// Factory function
NPC* newNPC(const std::string& name, int x, int y, SDL_Texture* texture) {
    return new NPC(name, x, y, texture);
}
