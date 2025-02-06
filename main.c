#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <renderers/raylib/clay_renderer_raylib.c>

const int FONT_CLOCK = 0;
const int FONT_INFO = 1;
Clay_Color ClockColour = {215,125,69,255};
Clay_Color InfoColour = {255,255,255,255};
Clay_Color SecondaryColour = {150,150,150,255};
Clay_Color BgColor = {4,15,24, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
}



int main(void) {
    Clay_Dimensions ProgDimensions = {500,500};
    printf("Starting Program\n");
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){
    .memory = malloc(clayRequiredMemory),
    .capacity = clayRequiredMemory,
    };
    Clay_Raylib_Initialize(ProgDimensions.width, ProgDimensions.height,"Window",FLAG_WINDOW_RESIZABLE);
    Clay_Initialize(clayMemory,ProgDimensions,(Clay_ErrorHandler) { HandleClayErrors });
    Raylib_fonts[FONT_CLOCK] = (Raylib_Font) {
        .font = LoadFontEx("assets/fonts/Lexend_Deca/static/LexendDeca-Bold.ttf", 256, 0, 0),
        .fontId = FONT_CLOCK
    };
    Raylib_fonts[FONT_INFO] = (Raylib_Font) {
        .font = LoadFontEx("assets/fonts/Lexend_Deca/static/LexendDeca-Light.ttf", 128, 0, 0),
        .fontId = FONT_INFO
    };
    Clay_SetMeasureTextFunction(Raylib_MeasureText,0);
    Image Icon_batteryOutline = LoadImage("assets/icons/batteryOutline.png");

    printf("Starting Loop\n");
    // printf("working directory: %s", GetWorkingDirectory());

    while (!WindowShouldClose()) {
        Clay_SetLayoutDimensions((Clay_Dimensions){GetScreenWidth(), GetScreenHeight()});
        Clay_BeginLayout();
        int Height = GetScreenHeight();
        int DebugOpacity = 255;
        int HeaderPadding = Height*0.05;
        int HeaderHeight = Height*0.155;
        int FooterHeight = Height*0.151;
        int FooterPadding = Height*0.08;
        CLAY(CLAY_ID("Display"),CLAY_RECTANGLE({.color = BgColor}),CLAY_LAYOUT({.childAlignment = { .x = CLAY_ALIGN_Y_CENTER, .y = CLAY_ALIGN_Y_CENTER }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}})
        ) {
            CLAY (
                CLAY_ID("HeaderBar"),
                CLAY_RECTANGLE({.color = {48,32,24,DebugOpacity}}),
                CLAY_LAYOUT({.padding = {0,0,0,0}, .sizing = {.height = CLAY_SIZING_FIXED(HeaderHeight), .width = CLAY_SIZING_GROW()}})
                ) {
                CLAY (
                    CLAY_ID("TemperatureDisplay"),
                    CLAY_RECTANGLE({.color = {100,32,24,DebugOpacity}}),
                    CLAY_LAYOUT({.childAlignment = { .x = CLAY_ALIGN_Y_CENTER, .y = CLAY_ALIGN_Y_CENTER }, .sizing = {.height = CLAY_SIZING_FIXED(HeaderHeight), .width = CLAY_SIZING_GROW()}})
                    ) {
                    CLAY_TEXT(CLAY_STRING("14*c"),CLAY_TEXT_CONFIG({
                        .fontId = FONT_CLOCK,
                        .fontSize =  42,
                        .textColor = InfoColour
                    }));
                }
                CLAY (
                    CLAY_ID("BatteryDisplayContext"),
                    CLAY_RECTANGLE({.color = {48,100,24,DebugOpacity}}),
                    CLAY_LAYOUT({.childAlignment = { .x = CLAY_ALIGN_Y_CENTER, .y = CLAY_ALIGN_Y_CENTER }, .sizing = {.height = CLAY_SIZING_FIXED(HeaderHeight), .width = CLAY_SIZING_GROW()}})
                    ) {
                    CLAY(CLAY_ID("BatteryDisplay"),CLAY_LAYOUT({})) {
                        CLAY_TEXT(CLAY_STRING("100%"),CLAY_TEXT_CONFIG({.fontId = FONT_CLOCK, .fontSize =  42, .textColor = InfoColour}));
                        CLAY(
                            CLAY_ID("BatteryOutlineImage"),
                            CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_FIXED(33), .height = CLAY_SIZING_FIXED(16) }}),
                            CLAY_IMAGE({ .imageData = &Icon_batteryOutline, .sourceDimensions = {33, 16}, })
                            ){};
                    }
                }
            }
            CLAY (
                CLAY_ID("Main"),
                CLAY_RECTANGLE(),
                CLAY_LAYOUT( {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW()}})
                ) {
                CLAY(
                    CLAY_ID("TopMain"),
                    CLAY_RECTANGLE({.color = {20,46,65,DebugOpacity}}),
                    CLAY_LAYOUT({.childAlignment = { .x = CLAY_ALIGN_Y_CENTER, .y = CLAY_ALIGN_Y_CENTER },.sizing = {.width = CLAY_SIZING_GROW() ,.height = CLAY_SIZING_PERCENT(0.6)}})
                    ) {
                    CLAY_TEXT(CLAY_STRING("16:48"),CLAY_TEXT_CONFIG({
                        .fontId = FONT_CLOCK,
                        .fontSize =  195,
                        .textColor = ClockColour
                    }));
                }
                CLAY(
                    CLAY_ID("BottomMain"),
                    CLAY_RECTANGLE({.color = {23,56,65,DebugOpacity}}),
                    CLAY_LAYOUT({.childAlignment = { .x = CLAY_ALIGN_Y_CENTER, .y = CLAY_ALIGN_Y_CENTER }, .sizing = {.width = CLAY_SIZING_GROW(),.height = CLAY_SIZING_GROW()}})
                    ) {
                        CLAY_TEXT(CLAY_STRING("7/12/24 SUN"),CLAY_TEXT_CONFIG({
                        .fontId = FONT_INFO,
                        .fontSize =  80,
                        .textColor = SecondaryColour
                }));
                }
            }
            CLAY (
                CLAY_ID("Footer"),
                CLAY_RECTANGLE({.color = {47,24,24,DebugOpacity}}),
                CLAY_LAYOUT({.padding = {0,0,0,0}, .sizing = {.height = CLAY_SIZING_FIXED(FooterHeight), .width = CLAY_SIZING_GROW()}})
                ){}
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
    }
}