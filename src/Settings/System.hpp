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
            color_order = sub_tree["color_order"].getValue();
        }

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        std::string GetColorOrder() const { return color_order; }

    private:
        int width;
        int height;
        std::string color_order;
    };

    inline bool operator==(const System& lhs, const System& rhs) 
    { 
        return lhs.GetWidth() == rhs.GetWidth()
            && lhs.GetHeight() == rhs.GetHeight()
            && lhs.GetColorOrder() == rhs.GetColorOrder();
    }
    inline bool operator!=(const System& lhs, const System& rhs) 
    { 
        return !operator==(lhs, rhs); 
    }
}
