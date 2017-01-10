#pragma once

#include <cinder/Json.h>

namespace Settings
{
    struct System
    {
        explicit System(cinder::JsonTree sub_tree) 
        {
            width = sub_tree["width"].getValue<int>();
            height = sub_tree["height"].getValue<int>();
        }

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }

    private:
        int width;
        int height;
    };
}
