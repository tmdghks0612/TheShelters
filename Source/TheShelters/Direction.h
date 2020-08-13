#pragma once

UENUM()
enum class Direction
{
    Left,
    Up,
    Right,
    Down,
    None
};

const Direction OppositeDirection(const Direction d);