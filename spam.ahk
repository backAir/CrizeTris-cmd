#SingleInstance, Force  ; Recommended for performance and compatibility with future AutoHotkey releases
SendMode Input  ; Recommended for faster and more reliable input
SetKeyDelay, 50, 50  ; Adjust delay between key presses (milliseconds)

$*x::  ; Trigger when the X key is pressed
    while GetKeyState("x", "P")  ; Check if the X key is still being held
    {
        Send, {x down}  ; Simulate X key press
        Sleep, 7  ; Delay for the key press
        Send, {x up}  ; Simulate X key release
        Sleep, 7  ; Delay before the next press
    }
    Send, {x up}  ; Simulate X key release
return