# Projective Geometry Basics (C++)

## Goal

Understand how basic geometric operations used in computer vision
can be expressed using linear algebra and homogeneous coordinates.

---

## Key Idea

In projective geometry:

* a **point** is a vector `(x, y, w)`
* a **line** is a vector `(a, b, c)`

Their relationship is defined by:

[
a x + b y + c w = 0
]

This allows all geometric operations to be expressed using:

* **dot product** → incidence (point lies on line)
* **cross product** → construction (join / meet)

---

## Representation

```cpp
struct Vec3 {
    double x, y, z;
};

using Point = Vec3; // (x, y, w)
using Line  = Vec3; // (a, b, c)
```

---

## Core Operations

| Operation | Formula         | Meaning                 |
| --------- | --------------- | ----------------------- |
| Incidence | `dot(l, p) = 0` | Point lies on line      |
| Join      | `l = p1 × p2`   | Line through two points |
| Meet      | `p = l1 × l2`   | Intersection of lines   |

---

## Key Insight

All basic geometric operations reduce to:

* dot product
* cross product

This makes geometry:

> a linear algebra problem

---

## Visualization

### Line from two points (join)

Two points define a line via cross product.

![join](images/join.png)

---

### Intersection of lines (meet)

The intersection is computed as:

[
p = l_1 \times l_2
]

![intersection](images/intersection.png)

---

### Parallel lines → infinity

Two parallel lines:

```
y = 100
y = 200
```

* In Euclidean geometry → no intersection
* In projective geometry → intersection exists

[
p = l_1 \times l_2 \quad \Rightarrow \quad w \approx 0
]

![parallel](images/parallel.png)

---

### Euclidean vs Projective

![comparison](images/euclidean_vs_projective.png)

Parallel lines:

* Euclidean space → do not intersect
* Projective space → intersect at infinity

This is a key idea behind many computer vision algorithms.

---

## Tests

Unit tests verify geometric properties:

* incidence correctness
* join/meet consistency
* symmetry of cross product
* behavior of parallel lines
* normalization of homogeneous coordinates

Example:

```cpp
EXPECT_TRUE(incidence(p, l));
EXPECT_TRUE(isAtInfinity(p));
```

---

## Why it matters

Projective geometry is the foundation of:

* homography
* camera models
* vanishing points
* multi-view geometry

Understanding these primitives makes higher-level CV concepts much clearer.

---

## Project Structure

```
src/
  geometry.h / .cpp   → core math
viz/
  visualize.cpp       → image generation (OpenCV)
tests/
  test_geometry.cpp   → unit tests (GoogleTest)
```

---

## How to build

```bash
mkdir build
cd build
cmake ..
make
```

Run tests:

```bash
./tests
```

Generate visualizations:

```bash
./visualize
```

---

## Next Steps

* Homography transformations
* Camera projection model
* Epipolar geometry

---

## Summary

This project shows that:

> Geometry in computer vision is not about shapes —
> it is about linear algebra and representation.

