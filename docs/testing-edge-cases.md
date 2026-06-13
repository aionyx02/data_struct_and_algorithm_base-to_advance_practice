---
type: testing_reference
status: active
priority: p2
updated: 2026-06-13
context_policy: retrieve_when_debugging
owner: project
---

# Testing Edge Cases

| ID | Area | Edge case | Expected behavior | Regression check |
|---|---|---|---|---|
| `EDGE.001` | Catalog | Problem path contains spaces or Traditional Chinese characters | Metadata and files load without path corruption | Catalog integration test |
| `EDGE.002` | Runner | Learner program never terminates | Child process is terminated and reported as `TLE` | Timeout fixture |
| `EDGE.003` | Runner | Learner program writes unbounded output | Capture stops at the configured bound and the process is handled safely | Output-limit fixture |
| `EDGE.004` | Linked structures | Operation sequence creates a cycle accidentally | Invariant check reports `INV` without hanging | Cycle fixture |
| `EDGE.005` | Hash table | All keys collide | Correctness remains testable and long probes are measured | Collision-heavy generator |
| `EDGE.006` | Balanced tree | Sorted insertion sequence | Height invariant remains within the structure's bound | Adversarial test |
| `EDGE.007` | Persistence | New version mutates shared nodes incorrectly | Older version remains logically unchanged | Version isolation test |
| `EDGE.008` | Progress | Process stops during write | Previous valid progress remains readable | Atomic-write interruption test |
