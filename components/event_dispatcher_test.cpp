#include "sync_event_dispatcher.h"
#include "async_event_dispatcher.h"

#define BOOST_TEST_MODULE event dispatcher tests
#include <boost/test/included/unit_test.hpp>

struct EventA {
  size_t id;
};

template<typename Container>
bool contains(const Container& container, const typename Container::value_type& value)
{
  return std::any_of(container.begin(), container.end(),
                     [&value](const typename Container::value_type& item) {
                        return item == value;
  });
}

BOOST_AUTO_TEST_CASE(register_handlers_for_one_type_sync_event_dispatcher)
{
  std::vector<std::string> processed;

  components::SyncEventDispatcher dispatcher;

  auto handler1 = dispatcher.RegisterHandler<EventA>(
          [&processed](const EventA& event) {
            processed.push_back(std::string("EventA1-") + std::to_string(event.id));
          });

  auto handler2 = dispatcher.RegisterHandler<EventA>(
          [&processed](const EventA& event) {
            processed.push_back(std::string("EventA2-") + std::to_string(event.id));
          });

  BOOST_TEST(processed.size() == 0);

  dispatcher.EmitEvent(EventA{.id = 1});

  BOOST_TEST(processed.size() == 2);
  BOOST_TEST(contains(processed, "EventA1-1"));
  BOOST_TEST(contains(processed, "EventA2-1"));

  dispatcher.EmitEvent(EventA{.id = 2});

  BOOST_TEST(processed.size() == 4);
  BOOST_TEST(contains(processed, "EventA1-2"));
  BOOST_TEST(contains(processed, "EventA2-2"));

  dispatcher.UnregisterHandler(handler1);

  dispatcher.EmitEvent(EventA{.id = 3});

  BOOST_TEST(processed.size() == 5);
  BOOST_TEST(!contains(processed, "EventA1-3"));
  BOOST_TEST(contains(processed, "EventA2-3"));

  dispatcher.UnregisterHandler(handler2);

  dispatcher.EmitEvent(EventA{.id = 4});

  BOOST_TEST(processed.size() == 5);
}

#include <ranges>

BOOST_AUTO_TEST_CASE(register_handlers_for_one_type_async_event_dispatcher)
{
  std::vector<std::string> processed;

  size_t start = 1;
  size_t end = 1000;

  {
    util::stop_source stop_source;
    components::ASyncEventDispatcher dispatcher(stop_source.get_token());

    dispatcher.RegisterHandler<EventA>(
            [&processed](const EventA &event) {
              processed.push_back(std::string("EventA1-") + std::to_string(event.id));
            });

    dispatcher.RegisterHandler<EventA>(
            [&processed](const EventA &event) {
              processed.push_back(std::string("EventA2-") + std::to_string(event.id));
            });

    for (size_t i = start; i < end; ++ i)
      dispatcher.EmitEvent(EventA{.id = i});

    stop_source.request_stop();
  }

  BOOST_TEST(processed.size() == (end - start) * 2);
  for (size_t i = start; i < end; ++ i) {
    BOOST_TEST(contains(processed, "EventA1-" + std::to_string(i)));
    BOOST_TEST(contains(processed, "EventA2-" + std::to_string(i)));
  }
}

struct EventB {
  size_t id;
};

BOOST_AUTO_TEST_CASE(register_handlers_for_two_types_sync_event_dispatcher)
{
  std::vector<std::string> processed;

  components::SyncEventDispatcher dispatcher;

  dispatcher.RegisterHandler<EventA>(
          [&processed](const EventA& event) {
            processed.push_back(std::string("EventA-") + std::to_string(event.id));
          });

  dispatcher.RegisterHandler<EventB>(
          [&processed](const EventB& event) {
            processed.push_back(std::string("EventB-") + std::to_string(event.id));
          });

  BOOST_TEST(processed.size() == 0);

  dispatcher.EmitEvent(EventA{.id = 1});
  dispatcher.EmitEvent(EventA{.id = 2});
  dispatcher.EmitEvent(EventB{.id = 1});

  BOOST_TEST(processed.size() == 3);
  BOOST_TEST(contains(processed, "EventA-1"));
  BOOST_TEST(contains(processed, "EventA-2"));
  BOOST_TEST(contains(processed, "EventB-1"));
}

BOOST_AUTO_TEST_CASE(register_handlers_for_two_types_async_event_dispatcher)
{
  std::vector<std::string> processed;

  {
    util::stop_source stop_source;
    components::ASyncEventDispatcher dispatcher(stop_source.get_token());

    dispatcher.RegisterHandler<EventA>(
            [&processed](const EventA &event) {
              processed.push_back(std::string("EventA-") + std::to_string(event.id));
            });

    dispatcher.RegisterHandler<EventB>(
            [&processed](const EventB &event) {
              processed.push_back(std::string("EventB-") + std::to_string(event.id));
            });

    dispatcher.EmitEvent(EventA{.id = 1});
    dispatcher.EmitEvent(EventA{.id = 2});
    dispatcher.EmitEvent(EventB{.id = 1});

    stop_source.request_stop();
  }

  BOOST_TEST(processed.size() == 3);
  BOOST_TEST(contains(processed, "EventA-1"));
  BOOST_TEST(contains(processed, "EventA-2"));
  BOOST_TEST(contains(processed, "EventB-1"));
}
