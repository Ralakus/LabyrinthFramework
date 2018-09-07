#include "IGraphicsClass.hpp"

#include <string>

namespace Labyrinth {

    class IWindow : public IGraphicsClass {

        protected:

		uint32_t m_Width, m_Height;
		std::string m_Title;

		bool m_Valid          = false;
		bool m_Fullscreen     = false;
		bool m_MouseLocked    = false;
		bool m_CursorOnIWindow = false;

		void UpdateKeyInput(int KeyCode, int Action);
		void UpdateButtonInput(int ButtonCode, int Action);
		void UpdateMousePos(float X, float Y);

	public:


		IWindow(const std::string& title, uint32_t width, uint32_t height, bool Fullscreen, GAPI API);
		IWindow(GAPI API);
		virtual ~IWindow();

		uint32_t GetHeight() const;
		uint32_t GetWidth()  const;
		virtual void Resize(uint32_t Width, uint32_t Height) = 0;

		virtual void SetTitle(const std::string& Title);
		virtual void SetTitle(const char* Title);
		const std::string& GetTitle() const;

		bool IsValid()          const;
		bool IsFullscreen()     const;
		bool IsMouseLocked()    const;
		bool IsCursorOnIWindow() const;

		virtual void SetFullscreen(bool Set) = 0;
		virtual void LockMouse(bool Set)     = 0;
		virtual void Update()    = 0;
		virtual void Close()     = 0;
		virtual void Terminate() = 0;


    };


	IWindow::IWindow(const std::string & title, uint32_t width, uint32_t height, bool Fullscreen, GAPI API) : IGraphicsClass(API), m_Title(title), m_Width(width), m_Height(height), m_Fullscreen(Fullscreen) {}

	IWindow::IWindow(GAPI API) : IGraphicsClass(API), m_Title("Labyrinth Default"), m_Width(1280), m_Height(720) {}

	IWindow::~IWindow() {}

	uint32_t IWindow::GetHeight() const { return m_Height; }

	uint32_t IWindow::GetWidth() const { return m_Width; }

	void IWindow::SetTitle(const std::string & Title) { m_Title = Title; }

	void IWindow::SetTitle(const char * Title) { m_Title = Title; }

	const std::string & IWindow::GetTitle() const { return m_Title; }

	bool IWindow::IsValid() const { return m_Valid; }

	bool IWindow::IsFullscreen() const { return m_Fullscreen; }

	bool IWindow::IsMouseLocked() const { return m_MouseLocked; }

	bool IWindow::IsCursorOnIWindow() const { return m_CursorOnIWindow; }

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