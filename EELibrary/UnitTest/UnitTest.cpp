#include "pch.h"
#include "CppUnitTest.h"

#include "events/program/ProgramLoadedEvent.h"
#include "events/EventHandler.h"
#include "events/EventID.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(EventUnitTest)
	{
	public:
		TEST_METHOD(TestBuildInEvent)
		{
			class EventListenerExample : public eelib::events::EventListener<eelib::events::ProgramLoadedEvent> {
				virtual void onScoppedEvent(eelib::events::ProgramLoadedEvent& programLoadedEvent) override
				{
					Assert::IsNotNull(&programLoadedEvent);
				}
			};

			eelib::events::EventHandler OnProgramLoaded;
			OnProgramLoaded.registerListener<eelib::events::ProgramLoadedEvent>(new EventListenerExample());
			eelib::events::EventID eventID = eelib::events::EventIDGenerator<eelib::events::Event>::getEventId<eelib::events::ProgramLoadedEvent>();
			Assert::IsTrue(OnProgramLoaded.getNbOfListeners<eelib::events::ProgramLoadedEvent>() == 1);
			OnProgramLoaded.sendEvent<eelib::events::ProgramLoadedEvent>();
		}

		TEST_METHOD(TestCustomEvent)
		{
			class CustomEvent : public eelib::events::Event {
			public:
				CustomEvent(int value) : _value(value) {}
				int getValue() { return _value; }
			private:
				int _value;
			};

			class EventListenerExample : public eelib::events::EventListener<CustomEvent> {
				virtual void onScoppedEvent(CustomEvent& customEvent) override
				{
					Assert::IsNotNull(&customEvent);
					Assert::IsTrue(customEvent.getValue() == 42);
				}
			};

			eelib::events::EventHandler OnCustomEvent;
			OnCustomEvent.registerListener<CustomEvent>(new EventListenerExample());
			eelib::events::EventID eventID = eelib::events::EventIDGenerator<eelib::events::Event>::getEventId<CustomEvent>();
			Assert::IsTrue(OnCustomEvent.getNbOfListeners<CustomEvent>() == 1);
			OnCustomEvent.sendEvent<CustomEvent>(42);
		}

		TEST_METHOD(TestUnregisterListener)
		{
			class EventListenerExample : public eelib::events::EventListener<eelib::events::ProgramLoadedEvent> {
				virtual void onScoppedEvent(eelib::events::ProgramLoadedEvent& programLoadedEvent) override
				{
					Assert::IsNotNull(&programLoadedEvent);
				}
			};

			eelib::events::EventHandler OnProgramLoaded;
			EventListenerExample* eventListenerExample = new EventListenerExample();
			OnProgramLoaded.registerListener<eelib::events::ProgramLoadedEvent>(eventListenerExample);
			Assert::IsTrue(OnProgramLoaded.getNbOfListeners<eelib::events::ProgramLoadedEvent>() == 1);
			OnProgramLoaded.unregisterListener<eelib::events::ProgramLoadedEvent>(eventListenerExample);
			Assert::IsTrue(OnProgramLoaded.getNbOfListeners<eelib::events::ProgramLoadedEvent>() == 0);
		}

		TEST_METHOD(TestUnregisterDummySend)
		{
			class EventListenerExample : public eelib::events::EventListener<eelib::events::ProgramLoadedEvent> {
				virtual void onScoppedEvent(eelib::events::ProgramLoadedEvent& programLoadedEvent) override
				{
					Assert::IsNotNull(&programLoadedEvent);
				}
			};

			eelib::events::EventHandler OnProgramLoaded;
			EventListenerExample* eventListenerExample = new EventListenerExample();
			OnProgramLoaded.registerListener<eelib::events::ProgramLoadedEvent>(eventListenerExample);
			Assert::IsTrue(OnProgramLoaded.getNbOfListeners<eelib::events::ProgramLoadedEvent>() == 1);
			OnProgramLoaded.unregisterListener<eelib::events::ProgramLoadedEvent>(eventListenerExample);
			Assert::IsTrue(OnProgramLoaded.getNbOfListeners<eelib::events::ProgramLoadedEvent>() == 0);
			OnProgramLoaded.sendEvent<eelib::events::ProgramLoadedEvent>();
		}

		TEST_METHOD(TestCustomMultipleEvent)
		{
			class CustomEvent : public eelib::events::Event {
			public:
				CustomEvent(int value) : _value(value) {}
				int getValue() { return _value; }
			private:
				int _value;
			};

			class AnotherCustomEvent : public eelib::events::Event {
			public:
				AnotherCustomEvent(int value) : _value(value) {}
				int getValue() { return _value; }
			private:
				int _value;
			};

			class EventListenerExample : public eelib::events::EventListener<CustomEvent> {
				virtual void onScoppedEvent(CustomEvent& customEvent) override
				{
					Assert::IsNotNull(&customEvent);
					Assert::IsTrue(customEvent.getValue() == 42);
				}
			};

			class AnotherEventListenerExample : public eelib::events::EventListener<AnotherCustomEvent> {
				virtual void onScoppedEvent(AnotherCustomEvent& anotherCustomEvent) override
				{
					Assert::IsNotNull(&anotherCustomEvent);
					Assert::IsTrue(anotherCustomEvent.getValue() == 4242);
				}
			};

			eelib::events::EventHandler OnCustomEvent;
			OnCustomEvent.registerListener<CustomEvent>(new EventListenerExample());
			OnCustomEvent.registerListener<AnotherCustomEvent>(new AnotherEventListenerExample());
			Assert::IsTrue(OnCustomEvent.getNbOfListeners<CustomEvent>() == 1);
			Assert::IsTrue(OnCustomEvent.getNbOfListeners<AnotherCustomEvent>() == 1);
			OnCustomEvent.sendEvent<CustomEvent>(42);
			OnCustomEvent.sendEvent<AnotherCustomEvent>(4242);
		}
	};
}
