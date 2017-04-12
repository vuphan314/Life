# Life

```
n > 0: order
k > 1: multiplier

space(n) == 2 ^ (n ^ 2)
space(k * n) == (space(n)) ^ (k ^ 2)

postspace(n + 2) == space(n)

image(n + 2) <= postspace(n + 2)

proportion(n + 2) == image(n + 2) / postspace(n + 2)

proportion(k * n + 2) ==
image(k * n + 2) / postspace(k * n + 2)

image(k * n + 2) <= (image(n + 2)) ^ (k ^ 2)

postspace(k * n + 2) ==
space(k * n) ==
(space(n)) ^ (k ^ 2) ==
(postspace(n + 2)) ^ (k + 2)

proportion(k * n + 2) <=
(image(n + 2) / postspace(n + 2)) ^ (k ^ 2) ==
(proportion(n + 2)) ^ (k ^ 2)
```
