#include "sync_event_dispatcher.h"

#define BOOST_TEST_MODULE event dispatcher tests
#include <boost/test/included/unit_test.hpp>

#include <algorithm>

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

BOOST_AUTO_TEST_CASE(register_handlers_for_one_type)
{
  std::vector<std::string> processed;

  components::SyncEventDispatcher dispatcher;

  dispatcher.RegisterHandler<EventA>(
          [&processed](const EventA& event) {
            processed.push_back(std::string("EventA1-") + std::to_string(event.id));
          });

  dispatcher.RegisterHandler<EventA>(
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
}

struct EventB {
  size_t id;
};

BOOST_AUTO_TEST_CASE(register_handlers_for_two_types)
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
