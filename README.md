# Overview

The high-level approach of my code follows these steps:
1. Set up replicas and send initial hello messages
2. Elect first leader replica, and then start processing get/put requests according to assigned roles of the replicas
3. Based on request types and roles, each replica performs different operations and runs different processes
4. Keep running until the end of the simulation

## Challenges

- Implementing the Raft algorithm and keeping track of all the moving parts / changing state
- Structuring the code in a clean way, the first iteration was extremely messy compared to the current

## Testing

Mostly tested by running against against the provided simulations. Would run it multiple times to ensure consistency.
