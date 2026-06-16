---
type: working_memory
status: active
priority: p0
updated: 2026-06-16
context_policy: always_retrievable
owner: project
---

# Current Project Memory

## Current Strategy

- Keep the curriculum limited to data structures: 68 foundational and 64 advanced problems.
- Build the CLI Judge before the Web interface; both must share one Judge core.
- Expand the catalog in small prerequisite-ordered chapters.

## Current Focus

- Active priority: build out the high-level data-structure curriculum.
- Current phase: Stage A3 Static Range Structures is underway. The Sparse Table
  range-query tree (`A21`), the Disjoint Sparse Table range sum (`A22`), the
  Square Root Decomposition range sum (`A23`), and block decomposition with lazy
  tags (`A24`) are implemented; Merge Sort Tree, Wavelet Tree/Matrix, and a
  bitset set remain. Stage A2 (`A09`-`A20`) and Stage A1 (`A01`-`A08`) are
  complete.
- Current owner / handoff state: `TASK.CURRICULUM.017` is active with `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Continue Stage A3: add the Merge Sort Tree (`A25`) for range order-statistic /
  count-less-than queries, then the Wavelet Tree.

## Last Validation Snapshot

- Last C++ validation: 2026-06-16.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 461 Judge integration tests passed (207.54 s; `A19`-`A24` each add
  1 catalog, +3 Judge, +3 stress). Each of `A19`-`A24` passed all four fixed
  tests and 100 differential cases (20,000 operations) with seed `20260614`.
  Their wrong fixtures receive `WA` on the first generated case (`A19`: k-th
  descend without subtracting the left count, inclusive `<=` rank; `A20`: a
  `chmin` that skips the Beats recursion, and one that scales by the whole node
  size; `A21`: a Sparse Table query that drops the right endpoint, and one that
  uses a single block; `A22`: an overlapping Sparse Table that double-counts sum,
  and one that doubles a single element; `A23`: a stale cached block sum, and
  whole-block sums that ignore partial boundaries; `A24`: partial reads that
  ignore the block lazy tag, and a full-block add that skips the cached-sum
  update).
- Judge runtime self-check (2026-06-17): `algo list` enumerates 91 problems;
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known off-by-one range, endpoint-only difference, single-tree range-query,
  upper-bound lower-bound walk, off-by-one rank, reversed coordinate mapping,
  wrong 2D update direction, point-only sparse storage, exclusive Segment Tree
  endpoints, stale parent aggregates, and invalid min/max or GCD identities
  produced reproducible `WA` verdicts; range-add and range-assign lazy variants
  that skip pushing pending tags also fail; mixed lazy variants with reversed
  assignment/add push order fail; positional searches that ignore their
  left/right boundary fail; dynamic trees that eagerly allocate both children
  fail; persistent trees that mutate old versions in place fail; order-statistic
  trees with a wrong k-th descend branch or inclusive rank fail; Segment Tree
  Beats variants that skip the second-max recursion or scale by the whole node
  size fail; Sparse Table queries that drop the right endpoint or use a single
  block fail; Disjoint Sparse Table variants that reuse overlapping blocks
  (double-counting sum) or miss the single-element case fail; Square Root
  Decomposition variants with a stale cached block sum or whole-block sums that
  ignore partial boundaries fail; block-lazy variants that ignore the block lazy
  tag on partial reads or skip the cached-sum update on a full-block add fail.
  Earlier F-series mistakes still fail.
- Known failing checks: none.
