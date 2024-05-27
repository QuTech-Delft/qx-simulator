==========================
Internals of the simulator
==========================


Sparse state vector
-------------------

QX-simulator internally represents a quantum state using a hash table mapping kets (e.g. ``|0010110>``) to
their associated complex amplitudes, and omits zero (or near-zero) amplitudes.

You can read about this approach in `this paper <https://dl.acm.org/doi/10.1145/3491248>`
by Samuel Jaques and Thomas Häner. Note however that QX-simulator was developed independently and the internal implementation differs.

This way to represent a quantum state is, in a lot of cases, very beneficial in terms of simulation runtime and memory usage.