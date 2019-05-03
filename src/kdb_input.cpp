#include "kdb_input.h"

using namespace kadabra;

input_buffer *
kadabra::input::InputFrontBuffer(){
    input_buffer * Result;

    Result = Buffers + FrontBufferIndex;

    return Result;
}

input_buffer *
kadabra::input::InputBackBuffer(){
    input_buffer * Result;

    u32 BackBufferIndex = (FrontBufferIndex==0) ? 1 : 0;
    Result = Buffers + BackBufferIndex;

    return Result;
}

void
kadabra::input::InputBufferPair(input_buffer **FrontBuffer, 
                         input_buffer **BackBuffer){
    *FrontBuffer = InputFrontBuffer();
    *BackBuffer  = InputBackBuffer();
}

b32
kadabra::input::IsKeyWentDown(input_key_type KeyCode){
    b32 Result = false;

    if(KeyCode < InputKey_Count){
        input_buffer *FrontBuffer, *BackBuffer;
        InputBufferPair(&FrontBuffer, &BackBuffer);

        Result = !BackBuffer->Keys[KeyCode].IsDown && 
                 FrontBuffer->Keys[KeyCode].IsDown;
    }

    return Result;
}

b32
kadabra::input::IsKeyDown(input_key_type KeyCode){
    b32 Result = false;

    if(KeyCode < InputKey_Count){
        input_buffer *In = InputFrontBuffer();
        Result = In->Keys[KeyCode].IsDown;
    }

    return Result;
}

b32
kadabra::input::IsKeyReleased(input_key_type KeyCode){
    b32 Result = false;

    if(KeyCode < InputKey_Count){
        input_buffer *FrontBuffer, *BackBuffer;
        InputBufferPair(&FrontBuffer, &BackBuffer);

        Result = BackBuffer->Keys[KeyCode].IsDown && 
                 !FrontBuffer->Keys[KeyCode].IsDown;
    }

    return Result;
}    

b32
kadabra::input::IsMouseButtonWentDown(input_mouse_button_type Button){
    b32 Result = false;

    if(Button < InputMouseButton_Count){
        input_buffer *FrontBuffer, *BackBuffer;
        InputBufferPair(&FrontBuffer, &BackBuffer);

        Result = !BackBuffer->MouseButtons[Button].IsDown && 
                 FrontBuffer->MouseButtons[Button].IsDown;
    }

    return Result;
}

b32
kadabra::input::IsMouseButtonDown(input_mouse_button_type Button){
    b32 Result = false;

    if(Button < InputMouseButton_Count){
        input_buffer *In = InputFrontBuffer();
        Result = In->MouseButtons[Button].IsDown;
    }

    return Result;
}

b32
kadabra::input::IsMouseButtonReleased(input_mouse_button_type Button){
    b32 Result = false;

    if(Button < InputMouseButton_Count){
        input_buffer *FrontBuffer, *BackBuffer;
        InputBufferPair(&FrontBuffer, &BackBuffer);

        Result = BackBuffer->MouseButtons[Button].IsDown && 
                 !FrontBuffer->MouseButtons[Button].IsDown;
    }

    return Result;
}    

f32
kadabra::input::GetMouseWheel(){
    f32 Result;
    
    input_buffer *In = InputFrontBuffer();
    Result = In->MouseWheel;

    return Result;
}

f32
kadabra::input::GetMouseWheelDelta(){
    f32 Result;

    input_buffer *FrontBuffer, *BackBuffer;
    InputBufferPair(&FrontBuffer, &BackBuffer);

    Result = FrontBuffer->MouseWheel - 
             BackBuffer->MouseWheel;

    return Result;
}

vec2
kadabra::input::GetMousePosition(){
    vec2 Result;
    
    input_buffer *In = InputFrontBuffer();
    Result = In->MousePositionClient;

    return Result;
}

vec2
kadabra::input::GetNormalisedMousePosition(){
    vec2 Result;
    
    input_buffer *In = InputFrontBuffer();
    Result = In->NormalisedMousePositionClient;

    return Result;
}

vec2
kadabra::input::GetMousePositionScreen(){
    vec2 Result;
    
    input_buffer *In = InputFrontBuffer();
    Result = In->MousePositionScreen;

    return Result;
}

vec2
kadabra::input::GetNormalisedMousePositionScreen(){
    vec2 Result;
    
    input_buffer *In = InputFrontBuffer();
    Result = In->NormalisedMousePositionScreen;

    return Result;
}

vec2
kadabra::input::GetMousePositionDelta(){
    vec2 Result;

    input_buffer *FrontBuffer, *BackBuffer;
    InputBufferPair(&FrontBuffer, &BackBuffer);

    Result = FrontBuffer->MousePositionClient - 
             BackBuffer->MousePositionClient;

    return Result;
}

vec2
kadabra::input::GetNormalisedMousePositionDelta(){
    vec2 Result;
 
    input_buffer *FrontBuffer, *BackBuffer;
    InputBufferPair(&FrontBuffer, &BackBuffer);

    Result = FrontBuffer->NormalisedMousePositionClient - 
             BackBuffer->NormalisedMousePositionClient;
   
    return Result;
}

vec2
kadabra::input::GetMousePositionScreenDelta(){
    vec2 Result;
    
    input_buffer *FrontBuffer, *BackBuffer;
    InputBufferPair(&FrontBuffer, &BackBuffer);

    Result = FrontBuffer->MousePositionScreen - 
             BackBuffer->MousePositionScreen;
   
    return Result;
}

vec2
kadabra::input::GetNormalisedMousePositionScreenDelta(){
    vec2 Result;
    
    input_buffer *FrontBuffer, *BackBuffer;
    InputBufferPair(&FrontBuffer, &BackBuffer);

    Result = FrontBuffer->NormalisedMousePositionScreen - 
             BackBuffer->NormalisedMousePositionScreen;
  
    return Result;
}
