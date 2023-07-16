# Newton Method 

The Newton-Raphson method is a iterative alogorithm to find roots given a function $f(x)$, its derivative $f'(x)$ and a starting estimate $x_0$. The method is to use where the tanget line from the current estimate $x_n$ crosses the x axis as the next estimate $x_{n+1}$.

$f'(x)$ is the gradient of the tangent line $g_n(x)$ at $x_n$,

$g_n(x) = f'(x_n)x + c$,

the tangent line passes through $(x_n, f(x_n))$,

$f(x_n) = f'(x_n)x_n + c$,

$c = f(x_n) - f'(x_n)x_n$,

$g_n(x) = f'(x_n)x + f(x_n) - f'(x_n)x_n$,

$g_n(x) = f'(x_n)(x - x_n) + f(x_n)$

substituting $(x_{n+1}, 0)$,

$0 = f'(x_n)(x_{n+1} - x_n) + f(x_n)$

$x_{n+1} = x_n - \frac{f(x_n)}{f'(x_n)}$

This final formula is the Newton-Raphson method for finding a functions root. Using the amount interations until the root has been found to color and the screen as a complex number plane this can produce a fractal that describes the time taken to converge for the Newton-Raphson function at different values.