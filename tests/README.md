This place is an absolute mess. Almost nothing in here is actually compiled or
tested by the build; it isn't even obvious how this should be done. Massive
cleanup is required.

The only thing that does work somewhat is the qxelarator subdir; it runs with
pytest, and I ported a single Python test to C++ to test the build system.
They don't actually check results though, so the only thing they do is check
that QX doesn't invoke `CRASH_AND_BURN()`.
