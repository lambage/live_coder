#pragma once

#include <cinder/Json.h>

namespace Settings
{
    struct SlideShow
    {
        explicit SlideShow(cinder::JsonTree sub_tree)
        {
            directory = sub_tree["directory"].getValue();
            fade_time = sub_tree["fade_time"].getValue<float>();
        }

        std::string GetDirectory() const { return directory; }
        float GetFadeTime() const { return fade_time; }

    private:
        std::string directory;
        float fade_time;
    };

    inline bool operator==(const SlideShow& lhs, const SlideShow& rhs)
    {
        return lhs.GetDirectory() == rhs.GetDirectory()
            && lhs.GetFadeTime() == rhs.GetFadeTime();
    }
    inline bool operator!=(const SlideShow& lhs, const SlideShow& rhs)
    {
        return !operator==(lhs, rhs);
    }
}
