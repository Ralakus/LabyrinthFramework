#pragma once
#include "IGraphicsClass.hpp"

#include "IWindow.hpp"

namespace Labyrinth {

    class IRenderer : public IGraphicsClass {

    public:

                 IRenderer(GAPI API) : IGraphicsClass(API) {};
        virtual ~IRenderer() {}

        virtual void     Update()    = 0;
        virtual IWindow* GetWindow() = 0;

    };

}