# Unit tests to be implemented

Skip sections and subsections marked as `[DONE]` or `[IGNORE]` in the code, as they have already been implemented. When a subsection is marked as `[DONE]` or `[IGNORE]`, it means that all the tests in that subsection and its sub-subsections have been implemented or must be ignored

## Block class

- Test that any method thow an exception when the current block has not been parsed yet

Note that any equality or inequality test should be performed within a tolerance of `1e-4` to account for floating point errors.

### Line interpolation

Use as a reference this:

```iso
N1 g00 x0 y0 z0
N2 g01 x100 y100 z0 f1000
```

- Test that the block is correctly parsed and the correct values are stored in the block object
- set `dt = block.profile.dt()`
- ensure that the `interpolate(data_t time, data_t &lambda, data_t &speed)` returns:
  - `lambda = 0` and `speed = 0` when `time = 0`
  - `lambda = 1` and `speed = 0` when `time = dt`
  - `lambda = 0.5` when `time = dt/2`
  - `lambda = 0` when `time < 0` and `lambda = 1` when `time > dt`
- ensure that the block length is correctly calculated as `hypot(100^2, 100^2)` within a tolerance of `1e-4`
- Use the `walk` method to iterate over the block and ensure that the maximum speed is not bigger than `1000` and not smaller than `990`

### Arc interpolation

Upon failure, the test shall print the expected value and the obtained value, as well as the time at which the failure occurred (if applicable).

#### With center point

Use as a reference this:

```iso
N1 g00 x0 y0 z0
N2 g02 x100 y0 z0 i50 j50 f1000
```

- set `dt = block.profile.dt()`
- set `r = block.r()`
- Verify that the block length is 3/4 the perimeter of a circle with radius `r`, within a tolerance of `1e-4`
- use the `walk` method to iterate over the block and ensure that:
  - `Y` is never negative
  - `X` is smaller than 50 when time is smaller than `dt/2` and bigger than 50 when time is bigger than `dt/2`

Use as a reference this:

```iso
N1 g00 x0 y0 z0
N2 g03 x100 y0 z0 i50 j50 f1000
```

- set `dt = block.profile.dt()`
- set `r = block.r()`
- Verify that the block length is 1/4 the perimeter of a circle with radius `r`, within a tolerance of `1e-4`
- use the `walk` method to iterate over the block and ensure that:
  - `Y` is never positive
  - `X` is smaller than 50 when time is smaller than `dt/2` and bigger than 50 when time is bigger than `dt/2`

#### With radius

Use as a reference this:

```iso
N1 g00 x0 y0 z0
N2 g02 x100 y0 z0 R60 f1000
```

- Verify that the block length is less than 1/2 of the perimeter of a circle of radius `r`
- use the `walk` method to iterate over the block and ensure that:
  - `Y` is never negative, but smaller or equal than `30`
  - `X` is smaller than 50 when time is smaller than `dt/2` and bigger than 50 when time is bigger than `dt/2`

Use as a reference this:

```iso
N1 g00 x0 y0 z0
N2 g03 x100 y0 z0 R-60 f1000
```

- Verify that the block length is more than 1/2 of the perimeter of a circle of radius `r`
- use the `walk` method to iterate over the block and ensure that:
  - `Y` is never positive, with some values larger than `-30`
  - `X` is smaller than 50 when time is smaller than `dt/2` and bigger than 50 when time is bigger than `dt/2`