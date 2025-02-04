#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <renderers/raylib/clay_renderer_raylib.c>

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
}



int main(void) {
    Clay_Dimensions ProgDimensions = {500,500};
    Clay_Color BgColor = {4,15,24, 255};
    printf("Starting Program\n");
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){
    .memory = malloc(clayRequiredMemory),
    .capacity = clayRequiredMemory,
    };
    Clay_Raylib_Initialize(ProgDimensions.width, ProgDimensions.height,"Window",FLAG_WINDOW_RESIZABLE);
    Clay_Initialize(clayMemory,ProgDimensions,(Clay_ErrorHandler) { HandleClayErrors });
    printf("Starting Loop\n");
    while (!WindowShouldClose()) {
        Clay_SetLayoutDimensions((Clay_Dimensions){GetScreenWidth(), GetScreenHeight()});
        Clay_BeginLayout();
        CLAY(
            CLAY_ID("MainDisplay"),
            CLAY_RECTANGLE({
                .color = BgColor,
            }),
            CLAY_LAYOUT({.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}, .padding = {14,14,14,14}})
        ) {
            CLAY (
                CLAY_ID("HeaderBar"),
                CLAY_RECTANGLE({.color = {40,40,200,255}, .cornerRadius = 100}),
                CLAY_LAYOUT({.sizing = {.height = CLAY_SIZING_FIXED(20), .width = CLAY_SIZING_GROW()}})
                ){}
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
    }
}