# Virtual Tree Representation

Preprocess a tree rooted at 1. Each query supplies `command k nodes...`; duplicates are removed. Add LCAs of consecutive node-id-sorted requested nodes, then connect each virtual node to its deepest included ancestor. `virtual_size` prints node count, `steiner_edges` prints the total original-tree edge length, and `build` prints sorted nodes followed by `parent-child` edges sorted by child (or `EMPTY`).
