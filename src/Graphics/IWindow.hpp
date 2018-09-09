#pragma once
#include <string>

#include "IGraphicsClass.hpp"


namespace Labyrinth {

    class IWindow : public IGraphicsClass {

        protected:

		uint32_t mWidth, mHeight;
		std::string mTitle;

		bool mValid           = false;
		bool mFullscreen      = false;
		bool mMouseLocked     = false;
		bool mCursorOnIWindow = false;

		void UpdateKeyInput(int KeyCode, int Action);
		void UpdateButtonInput(int ButtonCode, int Action);
		void UpdateMousePos(float X, float Y);

	public:


		IWindow(const std::string& Title, uint32_t Width, uint32_t Height, bool Fullscreen, GAPI API);
		IWindow(GAPI API);
		virtual ~IWindow();

		uint32_t GetHeight() const;
		uint32_t GetWidth()  const;
		virtual void Resize(uint32_t Width, uint32_t Height) {}

		virtual void SetTitle(const std::string& Title);
		virtual void SetTitle(const char* Title);
		const std::string& GetTitle() const;

		bool IsValid()           const;
		bool IsFullscreen()      const;
		bool IsMouseLocked()     const;
		bool IsCursorOnIWindow() const;

		virtual void SetFullscreen(bool Set) 										  {}
		virtual void LockMouse(bool Set)     										  {}
		virtual int  Open(const std::string& Title, uint32_t Width, uint32_t Height) = 0;
		virtual int  Open()     													 = 0;
		virtual void Update()   													 = 0;
		virtual void Close()    													 = 0;
		virtual void Terminate()													 = 0;


    };


	IWindow::IWindow(const std::string & title, uint32_t width, uint32_t height, bool Fullscreen, GAPI API) : IGraphicsClass(API), mTitle(title), mWidth(width), mHeight(height), mFullscreen(Fullscreen) {}

	IWindow::IWindow(GAPI API) : IGraphicsClass(API), mTitle("Labyrinth"), mWidth(1280), mHeight(720) {}

	IWindow::~IWindow() {}

	uint32_t IWindow::GetHeight() const { return mHeight; }

	uint32_t IWindow::GetWidth() const { return mWidth; }

	void IWindow::SetTitle(const std::string & Title) { mTitle = Title; }

	void IWindow::SetTitle(const char * Title) { mTitle = Title; }

	const std::string & IWindow::GetTitle() const { return mTitle; }

	bool IWindow::IsValid() const { return mValid; }

	bool IWindow::IsFullscreen() const { return mFullscreen; }

	bool IWindow::IsMouseLocked() const { return mMouseLocked; }

	bool IWindow::IsCursorOnIWindow() const { return mCursorOnIWindow; }

	/*void IWindow::UpdateKeyInput(int KeyCode, int Action) {
		if (InputManager) {
			InputManager->UpdateKeys(static_cast<Key>(KeyCode), static_cast<InputAction>(Action));
		}
	}

	void IWindow::UpdateButtonInput(int ButtonCode, int Action) {
		if (InputManager) {
			InputManager->UpdateButtons(static_cast<Button>(ButtonCode), static_cast<InputAction>(Action));
		}
	}

	void IWindow::UpdateMousePos(float X, float Y) {
		if (InputManager) {
			InputManager->UpdateMousePos(X, Y);
		}
	}*/

}