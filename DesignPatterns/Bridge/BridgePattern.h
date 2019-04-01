#pragma once
#include <string>

namespace bridge_pattern
{
	namespace problem
	{
		// our game engine class to show pop-ups to players
		class SystemDialog
		{
		public:
			virtual ~SystemDialog() {}
			virtual void render() = 0;

			// other methods...
		};

		// implemented traditionally for different platforms
		// ps4
		class SystemDialog_PS4: public SystemDialog
		{
			void render() override
			{
				// call ps4 system function to show the dialogue
			}
		};

		// xone
		class SystemDialog_X1 : public SystemDialog
		{
			void render() override
			{
				// call X1 system function to show the dialogue
			}
		};

		// nintendo switch - NX
		class SystemDialog_NX : public SystemDialog
		{
			void render() override
			{
				// call NX system function to show the dialogue
			}
		};

		// now say we want to implement a new kind of dialogue which plays an animation and has 2 buttons Ok / cancel
		// we will start with subclassing SystemDialog to SystemDialogWithAnim. Secondly we will need to provide 3 more 
		// implimentations one for each platform. Now imagine if we were to add a new platform say Google's stadia then
		// it becomes too complicated or messed up....
		// solution follows below 
	} //------------------------------------------------------------------------------------------------------------------

	class SystemDialogImpl
	{
	public:
		struct AnimData {};

		virtual void render() = 0;
		virtual void setAnimData(const AnimData& data) = 0;
	};

	// it seems a bridge design pattern has better options for us
	class SystemDialog
	{
	public:

		virtual ~SystemDialog() {}

		enum class Align { Left, Center, Right };

		virtual void open(Align align) { alignment = align; }
		virtual void close() { kill = true; }
		void setText(const char* txt) { text = txt; }
		void setXYWH(float _x, float _y, float _w, float _h);

		virtual void render();

	protected:
		bool kill;
		Align alignment;
		float x, y, w, h;
		std::string text;

		SystemDialogImpl* impl; // impl connecting SystemDialog to SystemDialogImpl marks the bridge
	};

	class SystemDialogWithAnim: public SystemDialog
	{
		void open()
		{
			SystemDialog::open(Align::Center);
			SystemDialogImpl::AnimData data;
			impl->setAnimData(data);
		}
	};
}