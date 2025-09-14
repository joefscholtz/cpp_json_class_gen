#include <iostream>
#include <stdexcept>
#include <string>

#include "ApiEvent.hpp"
#include "ApiEventsList.hpp"
#include "BaseEvent.hpp"
#include "nlohmann/json.hpp"

void print_event_details(const gcal::ApiEvent &event) {
  std::cout << "\n--- Event Details ---" << std::endl;
  std::cout << "  Summary: " << event.summary << std::endl;
  std::cout << "  ID: " << event.id << std::endl;
  std::cout << "  Status: " << event.status << std::endl;
  std::cout << "  Creator Email: " << event.creator.email << std::endl;
  std::cout << "  Start Time: " << event.start.dateTime << std::endl;
}

int main() {
  std::string json_string = R"({
        "kind": "calendar#events", "etag": "etag12345", "summary": "Project Apollo",
        "description": "Planning meeting for the next phase.", "updated": "2025-09-11T18:30:00Z", 
        "timeZone": "America/Sao_Paulo", "accessRole": "owner",
        "nextPageToken": null, "nextSyncToken": "syncToken67890",
        "items": [
            { 
                "id": "event1", "status": "confirmed", "htmlLink": "http://example.com/event1",
                "created": "2025-09-11T15:00:00Z", "updated": "2025-09-11T15:30:00Z",
                "summary": "Phase 1 Kick-off", "description": "Discussing the project timeline.",
                "creator": { "email": "project.lead@example.com", "displayName": "Alex Ray" },
                "organizer": { "email": "project.lead@example.com", "displayName": "Alex Ray" },
                "start": { "dateTime": "2025-10-05T10:00:00-03:00", "timeZone": "America/Sao_Paulo" },
                "end": { "dateTime": "2025-10-05T11:00:00-03:00", "timeZone": "America/Sao_Paulo" }
            }
        ]
    })";

  try {
    nlohmann::json j = nlohmann::json::parse(json_string);
    gcal::ApiEventsList event_list = j.get<gcal::ApiEventsList>();

    std::cout << "Successfully parsed event list!" << std::endl;
    std::cout << "Calendar Summary: " << event_list.summary << std::endl;
    std::cout << "Number of items: " << event_list.items.size() << std::endl;

    if (!event_list.items.empty()) {
      // Get a reference to the original event
      gcal::ApiEvent &original_event = event_list.items[0];
      print_event_details(original_event);

      // --- TESTING THE CLONE METHOD ---
      std::cout << "\n--- Testing Polymorphic Clone ---" << std::endl;

      // 1. Create a pointer to the base class of the original event.
      events::BaseApiEvent *base_ptr_to_original = &original_event;

      // 2. Call clone() to create a new, deep copy.
      //    The result is a smart pointer to the base class.
      std::unique_ptr<events::BaseApiEvent> cloned_base_ptr =
          base_ptr_to_original->clone();

      std::cout << "Successfully created a clone." << std::endl;

      // 3. To use the clone as a GCalApiEvent, you must dynamic_cast it.
      gcal::ApiEvent *cloned_gcal_ptr =
          dynamic_cast<gcal::ApiEvent *>(cloned_base_ptr.get());

      if (cloned_gcal_ptr) {
        // 4. Modify the clone.
        std::cout << "Modifying the clone's summary..." << std::endl;
        cloned_gcal_ptr->summary = "CLONED - Planning Session";

        // 5. Show that the original and the clone are now different.
        std::cout << "\nOriginal Event Summary: " << original_event.summary
                  << std::endl;
        std::cout << "Cloned Event Summary:   " << cloned_gcal_ptr->summary
                  << std::endl;
      } else {
        std::cerr << "Error: dynamic_cast to GCalApiEvent failed!" << std::endl;
      }
      print_event_details(original_event);

      std::cout << "\n--- Serializing Modified Event ---" << std::endl;

      gcal::ApiEvent modified_event = original_event;
      modified_event.summary = "Phase 1 Kick-off (Rescheduled)";
      modified_event.status = "tentative";

      nlohmann::json j_modified = modified_event;

      std::cout << j_modified.dump(4) << std::endl;
    }

  } catch (const nlohmann::json::exception &e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
