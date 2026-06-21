# Link-Cut Tree Dynamic Forest

Maintain a dynamic forest with node values. Input is `n q`, then n values. Commands: `link a b` (`LINKED`/`CYCLE`), `cut a b` (`CUT`/`NO_EDGE`, only direct edges), `connected a b`, inclusive `path_sum a b`, `path_add a b delta`, `set v value`, and `value v`. Disconnected path operations print `DISCONNECTED`. Implement makeroot/access/splay with lazy path addition.
