# Computer Vision Labs

A small C++20 project for computer vision labs and experiments. Reusable mathematical code lives in `library/`; each existing exercise has its own directory and executable under `labs/`.

## Labs and experiments

| Directory | Topic | Relation to geometry-labs-2021.pdf | Status |
|---|---|---|---|
| `labs/projective-geometry` | Homogeneous coordinates, joins, meets, incidence, homographies | Additional exercise; not PDF lab 1 | Implemented, with unit tests |
| `labs/panorama-stitching` | ORB matching, custom and OpenCV homography, warping and blending | PDF lab 3 | Partial: panorama pipeline exists; the PDF's full interpolation/quality requirements have not been audited |
| `labs/camera-projection` | Pinhole projection and camera pose | Additional exercise | Implemented, with unit tests for the math |
| `labs/camera-calibration` | Chessboard calibration, reprojection checks, parameter sensitivity | Additional calibration experiment | Implemented for image-directory and video input; saved experimental frames/results are in `data/` |

The other PDF assignments are not implemented yet: lab 1 stereo rectification, lab 2 SGM, lab 4 random fields for panorama stitching, lab 5 fundamental matrix estimation, lab 6 essential matrix estimation, and lab 7 stereo rectification. The PDF also mentions additional tasks such as SIFT and displacement maps; these are not presented as completed features here.

## Reusable library

`library/` builds the `cv_geometry` C++ library. It contains projective geometry operations, 2D transforms, homographies and homography estimation, plus camera projection and rotation math. Experiment-specific image processing and visualization remain in their lab directories.

## Requirements

- CMake 3.16 or newer
- A C++20 compiler
- Eigen 3
- OpenCV (the labs request the components they use)
- GoogleTest to build tests

## Configure and build

Configure from the repository root:

```sh
cmake -S . -B build -DBUILD_TESTING=ON
```

Build one lab or the whole project:

```sh
cmake --build build --target lab_projective_geometry
cmake --build build --target lab_panorama_stitching
cmake --build build --target lab_camera_projection
cmake --build build --target lab_camera_calibration
cmake --build build
```

Run the tests with:

```sh
ctest --test-dir build --output-on-failure
```

## Run labs

Projective geometry writes images to `projective-geometry-output/`; camera projection writes `camera_projection.jpg` in the current working directory:

```sh
./build/labs/projective-geometry/lab_projective_geometry
./build/labs/camera-projection/lab_camera_projection
```

Panorama stitching takes two image paths and writes `pano_my.jpg` and `pano_cv.jpg` in the current working directory:

```sh
./build/labs/panorama-stitching/lab_panorama_stitching \
  labs/panorama-stitching/images/img1.jpg \
  labs/panorama-stitching/images/img2.jpg
```

Camera calibration accepts an image directory or video file. It writes detected-corner images to `calibration-output/` in the current working directory:

```sh
./build/labs/camera-calibration/lab_camera_calibration images labs/camera-calibration/data/selected_1
./build/labs/camera-calibration/lab_camera_calibration video path/to/video.mp4
```

The image command uses included sample frames. For video input, replace `path/to/video.mp4` with an existing video file.

## Source layout

```text
library/                 Reusable C++ math and geometry library
labs/<topic>/            Independent lab executable, notes, and topic data
labs/camera-calibration/ Calibration-only helpers, tests, and preserved experiment data
```
