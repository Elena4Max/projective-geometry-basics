# Camera calibration experiment

This additional exercise calibrates a camera from a directory of chessboard images or a video file. It detects a 6 × 9 inner-corner pattern, estimates intrinsics and distortion, checks reprojection error, compares the custom projection math with OpenCV, and prints a small parameter sensitivity analysis.

The previously captured image sets and corner/reprojection visualizations are preserved in `data/`. They are experiment artifacts; the executable does not treat them as output fixtures.

## Build and run

From the repository root:

```sh
cmake -S . -B build
cmake --build build --target lab_camera_calibration
./build/labs/camera-calibration/lab_camera_calibration images labs/camera-calibration/data/selected_1
./build/labs/camera-calibration/lab_camera_calibration video path/to/chessboard-video.mp4
```

The first command uses the sample frames included in this repository. For your own images, replace that directory with the directory containing your chessboard images. The video command also needs an existing video file in place of the example path. A missing or invalid input path produces an error message and exits normally.

The program writes detected-corner images into `calibration-output/` in the current working directory, prints calibration matrices and reprojection results to the terminal, and leaves input data untouched. Tests for calibration YAML I/O run with the repository's full test suite.
