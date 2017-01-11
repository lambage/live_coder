#pragma once

#include <cinder/Json.h>
#include "DataImports.hpp"
#include "SlideShow.hpp"
#include "System.hpp"

namespace Settings
{
    struct Settings
    {
        explicit Settings(cinder::JsonTree tree)
            : system(tree["system"])
            , slide_show(tree["slide_show"])
            , data_imports(tree["data_imports"])
        {
        }

        System& GetSystem() { return system; }
        SlideShow& GetSlideShow() { return slide_show; }
        DataImports& GetDataImports() { return data_imports; }

    private:
        System system;
        SlideShow slide_show;
        DataImports data_imports;
    };
}

