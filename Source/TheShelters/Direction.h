#pragma once

UENUM()
enum class Direction:uint8
{
    Left,
    Up,
    Right,
    Down,
    None
};

const Direction OppositeDirection(const Direction d);