#pragma once
#include "IGraphicsClass.hpp"

namespace Labyrinth {

    class IRenderer : public IGraphicsClass {

    public:

                 IRenderer(GAPI API) : IGraphicsClass(API) {};
        virtual ~IRenderer() {}

    };

}