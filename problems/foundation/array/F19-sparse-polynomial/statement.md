# Sparse Polynomial Terms

Store nonzero `(exponent, coefficient)` terms in descending exponent order.

Input starts with `capacity operationCount`. Operations: `set exponent coefficient`
(`0` removes a term), `coeff exponent`, `degree`, `leading`, `term i`, `terms`,
and `clear`.

Negative exponents print `OUT_OF_RANGE`; inserting a new nonzero term into a
full representation prints `FULL`. Empty `degree` is `-1`; empty `leading`
prints `EMPTY`. Do not use maps or `std::vector`.
