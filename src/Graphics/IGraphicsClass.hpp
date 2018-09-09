#pragma once

namespace Labyrinth {

    enum class GAPI {
        Vulkan, OpenGL, Undefined
    };

    class IGraphicsClass {

    protected:

        const GAPI mAPI;

    public:

                 IGraphicsClass(GAPI API) : mAPI(API) {}
        virtual ~IGraphicsClass() {}

        GAPI GetAPI() const { return mAPI; }

    };

}