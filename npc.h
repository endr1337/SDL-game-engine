#pragma once
#include <string>
#include "include/SDL.h"
#include "include/SDL_ttf.h"
#include <vector>

struct NPCDialog {
    std::string text;
    int width;
    int height;
    Uint32 duration; // milliseconds to show this dialog
};

class NPC {
public:
    NPC(const std::string& name, int x, int y, SDL_Texture* texture);
    ~NPC();

    void render(SDL_Renderer* renderer);
    void talk(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int width, int height);

    // New dialog system
    void setDialogs(const std::vector<NPCDialog>& dialogs);
    void startDialogSequence();
    void updateDialog();
    void renderDialog(SDL_Renderer* renderer, TTF_Font* font);
    void nextDialog(); // <-- Add this
    bool isDialogActive() const; // <-- Ad
    int getX() const { return x_; }
    int getY() const { return y_; }
    const std::string& getName() const { return name_; }
    const std::string& getCurrentDiag() const {  
       if (currentDialog_ < dialogs_.size()) {  
           return dialogs_[currentDialog_].text;  
       }  
       static const std::string emptyString = "";  
       return emptyString;  
    }
    size_t getCurrentDialogIndex() const {
        return currentDialog_;
    }
    int x_, y_;
private:
    std::string name_;
    SDL_Texture* texture_;

    // Dialog system
    std::vector<NPCDialog> dialogs_;
    size_t currentDialog_;
    Uint32 dialogStartTime_;
    bool dialogActive_;
};

NPC* newNPC(const std::string& name, int x, int y, SDL_Texture* texture);