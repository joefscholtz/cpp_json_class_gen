#include <iostream>

#include "ApiEvent.hpp"
#include "ApiEventsList.hpp"
#include "nlohmann/json.hpp"

int main() {
  std::string json_string = R"({
        "kind": "calendar#events", "etag": "123", "summary": "Test Calendar",
        "description": "", "updated": "", "timeZone": "", "accessRole": "",
        "nextPageToken": "", "nextSyncToken": "abcde",
        "defaultReminders": [{"method": "popup", "minutes": 10}],
        "items": [
            { "id": "event1", "status": "confirmed", "htmlLink": "http://a",
              "created": "", "updated": "", "summary": "My Event", "description": "" }
        ]
    })";

  nlohmann::json j = nlohmann::json::parse(json_string);

  gcal::ApiEventsList event_list = j.get<gcal::ApiEventsList>();

  std::cout << "Successfully parsed event list!" << std::endl;
  std::cout << "Summary: " << event_list.summary << std::endl;
  std::cout << "Number of items: " << event_list.items.size() << std::endl;
  std::cout << "First item summary: " << event_list.items[0].summary
            << std::endl;

  return 0;
}
