# Life

```
n > 0: order
k > 1: multiplier

space(n) == 2 ^ (n ^ 2)
space(k * n) == (space(n)) ^ (k ^ 2)

prespace(n) == space(n + 2)

preimage(n) <= prespace(n)

proportion(n) == preimage(n) / prespace(n)
proportion(k * n) == preimage(k * n) / prespace(k * n)

preimage(k * n) <= (preimage(n)) ^ (k ^ 2)

prespace(k * n) ==
space(k * n + 2) <  
space(k * (n + 2)) ==
(space(n + 2)) ^ (k ^ 2) ==
(prespace(n)) ^ (k ^ 2)

proportion(k * n) ?=
(preimage(n) / prespace(n)) ^ (k ^ 2) ==
(proportion(n)) ^ (k ^ 2)
```
