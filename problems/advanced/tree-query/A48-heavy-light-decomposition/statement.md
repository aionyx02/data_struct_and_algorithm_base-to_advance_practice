# Heavy-Light Decomposition

Construct a deterministic Heavy-Light Decomposition of a tree rooted at 1. The heavy child has the largest subtree; ties use the smaller node id. Heavy paths are assigned before light children, whose ids are processed increasingly. Queries: `head v`, zero-based `position v`, `heavy v` (`NONE` for a leaf), `chain v` (head through v), and `order` (nodes by HLD position).
